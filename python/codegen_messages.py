#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen_messages.py - Generate c++ message classes
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2015 Lachlan Orr
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#   1. The origin of this software must not be misrepresented; you must not
#   claim that you wrote the original software. If you use this software
#   in a product, an acknowledgment in the product documentation would be
#   appreciated but is not required.
#
#   2. Altered source versions must be plainly marked as such, and must not be
#   misrepresented as being the original software.
#
#   3. This notice may not be removed or altered from any source
#   distribution.
#-------------------------------------------------------------------------------

import posixpath
import os

TYPE_TO_UNION = { 'u32': 'u',
                  'i32': 'i',
                  'f32': 'f',
                  'bool': 'b' }

def read_file_data(path):
    if not os.path.exists(path):
        return None
    with open(path, 'r') as f:
        return f.read()

def replace_file_if_different(path, data):
    if read_file_data(path) != data:
        print 'Writing ' + path
        with open(path, 'w') as out_f:
            out_f.write(data)

def python_path():
    scriptdir = os.path.split(os.path.abspath(__file__))[0].replace('\\', '/')
    return scriptdir

def gaen_path():
    return posixpath.split(python_path())[0]

def template_subst(template, replacements):
    for k, v in replacements.iteritems():
        template = template.replace('<<%s>>' % k, v)
    return template

def gen_includes(includes):
    inc = []
    for i in includes:
        inc.append('#include "%s"\n' % i)
    return ''.join(inc)

def camel_to_underscores(s):
    s = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', s)
    s = re.sub(r'([a-z\d])([A-Z])', r'\1_\2', s)
    s = s.strip('_')
    return s.lower()

class Templates:
    MessageClass   = read_file_data(posixpath.join(python_path(), 'templates/message_template.cpp'))
    CmakeFileList  = read_file_data(posixpath.join(python_path(), 'templates/filelist_template.cmake'))
    
def messages_def_path():
    return posixpath.join(gaen_path(), 'src/engine/messages')

def gen_message_cmake(field_handlers):
    lines = []
    for field_handler in field_handlers:
        lines.append('  messages/%s.h' % field_handler.object_name)
    lines.sort()
    return template_subst(Templates.CmakeFileList, {'files'       : '\n'.join(lines),
                                                    'autogen_type': 'messages'})

def parse_messages_def():
    context = globals()
    context['FIELD_HANDLERS'] = []
    with open(posixpath.join(python_path(), 'field_handler.py')) as f:
        exec f in context
    def_path = posixpath.join(messages_def_path(), 'messages.def')
    with open(def_path) as f:
        exec f in context
    return context['FIELD_HANDLERS']

def gen_reader_member_init(field_handler):
    lines = []
    for f in field_handler.fields:
        if f.block_count > 1:
            lines.append('        if (&msgAcc[%d] > &msgAcc[%d])' % (f.block_start + f.block_count - 1, f.block_start))
            lines.append('        {')
            lines.append('            // field is contiguous in ring buffer')
            lines.append('            %s = reinterpret_cast<const %s*>(&msgAcc[%d]);' % (f.member_pvar, f.type_name, f.block_start))
            lines.append('        }')
            lines.append('        else')
            lines.append('        {')
            lines.append('            // field wraps ring buffer, copy it into our datamember')
            lines.append('            for (u32 i = 0; i < %d; ++i)' % f.block_count)
            lines.append('            {')
            lines.append('                block_at(&%s, i) = msgAcc[i + %d];' % (f.member_var, f.block_start))
            lines.append('            }')
            lines.append('            %s = &%s;' % (f.member_pvar, f.member_var))
            lines.append('        }')
            lines.append('')
            
    return '\n'.join(lines)

def gen_reader_getters(field_handler):
    lines = []
    for f in field_handler.fields:
        if f.payload:
            if TYPE_TO_UNION[f.raw_type] == f.union_type:
                lines.append('    %s %s() const { return mMsgAcc.message().payload.%s; }' % (f.type_name, f.name, f.union_type))
            else:
                lines.append('    %s %s() const { return *reinterpret_cast<const %s*>(&mMsgAcc.message().payload.%s); }' % (f.type_name, f.name, f.type_name, f.union_type))
        elif type(f) == PointerField:
            lines.append('    %s %s() const { return static_cast<%s>(mMsgAcc[%d].%s.p); }' % (f.type_name, f.getter_name, f.type_name, f.block_start, f.block_accessor))
        elif f.type_name == 'Vec3':
            lines.append('    const %s & %s() const { return *reinterpret_cast<const %s*>(&mMsgAcc[%d].%s); }' % (f.type_name, f.name, f.type_name, f.block_start, f.block_accessor))
        elif f.block_count <= 1:
            lines.append('    %s %s() const { return mMsgAcc[%d].%s.%s; }' % (f.type_name, f.name, f.block_start, f.block_accessor, f.union_type))
        else:
            lines.append('    const %s & %s() const { return *%s; }' % (f.type_name, f.name, f.member_pvar))
    return '\n'.join(lines)
    
def gen_reader_data_members(field_handler):
    lines = []
    for f in field_handler.fields:
        if f.block_count > 1:
            lines.append('    %s %s;' % (f.type_name, f.member_var))
            lines.append('    const %s * %s;' % (f.type_name, f.member_pvar))

    if len(lines) > 0:
        lines.insert(0, '    // These members provide storage for fields larger than a block that wrap the ring buffer')

    return '\n'.join(lines)

def gen_payload_decl(field_handler):
    for f in field_handler.fields:
        if f.payload:
            return ',\n%s      %s %s' % (' ' * (len(field_handler.object_name)+1), f.type_name, f.name)
    return ''

def gen_payload_value(field_handler):
    for f in field_handler.fields:
        if f.payload:
            if f.type_name in ('u32', 'i32'):
                return f.name
            else:
                # add explicit casting as it's probably necessary for this type
                return "*reinterpret_cast<const u32*>(&%s)" % f.name
    return '0'
    

def gen_writer_setters(field_handler):
    lines = []
    for f in field_handler.fields:
        if f.payload:
            if TYPE_TO_UNION[f.raw_type] == f.union_type:
                lines.append('    void %s(%s val) { mMsgAcc.message().payload.%s = val; }' % (f.setter_name, f.type_name, f.union_type))
            else:
                lines.append('    void %s(%s val) { mMsgAcc.message().payload.%s = *reinterpret_cast<const %s*>(&val); }' % (f.setter_name, f.type_name, f.union_type, f.type_name))
        elif type(f) == PointerField:
            lines.append('    void %s(%s pVal) { mMsgAcc[%d].%s.p = pVal; }' % (f.setter_name, f.type_name, f.block_start, f.block_accessor))
        elif f.type_name == 'Vec3':
            lines.append('    void %s(const %s & val) { mMsgAcc[%d].%s = *reinterpret_cast<const tcell*>(&val); }' % (f.setter_name, f.type_name, f.block_start, f.block_accessor))
        elif f.block_count <= 1:
            lines.append('    void %s(%s val) { mMsgAcc[%d].%s.%s = val; }' % (f.setter_name, f.type_name, f.block_start, f.block_accessor, f.union_type))
        else:
            lines.append('    void %s(const %s & val)' % (f.setter_name, f.type_name))
            lines.append('    {')
            lines.append('        for (u32 i = 0; i < %d; ++i)' % f.block_count)
            lines.append('        {')
            lines.append('            mMsgAcc[i + %d] = block_at(&val, i);' % f.block_start)
            lines.append('        }')
            lines.append('    }')
    return '\n'.join(lines)
    
def gen_message_class(field_handler):
    repl = {'message_name_caps'      : field_handler.object_name.upper(),
            'message_name'           : field_handler.object_name,
            'message_name_indent'    : ' ' * (len(field_handler.object_name) + 2),
            'includes'               : gen_includes(field_handler.includes),
            'reader_data_member_init': gen_reader_member_init(field_handler),
            'reader_getters'         : gen_reader_getters(field_handler),
            'reader_data_members'    : gen_reader_data_members(field_handler),
            'payload_decl'           : gen_payload_decl(field_handler),
            'payload_value'          : gen_payload_value(field_handler),
            'block_count'            : str(field_handler.block_count),
            'writer_setters'         : gen_writer_setters(field_handler),
    }

    return template_subst(Templates.MessageClass, repl)

def gen_message_classes():
    field_handlers = parse_messages_def()
    for field_handler in field_handlers:
        cpp_data = gen_message_class(field_handler)
        cpp_path = posixpath.join(messages_def_path(), field_handler.object_name + '.h')
        replace_file_if_different(cpp_path, cpp_data)
    cmake_data = gen_message_cmake(field_handlers)
    cmake_path = posixpath.join(messages_def_path(), 'messages.cmake')
    replace_file_if_different(cmake_path, cmake_data)
        

if __name__ == '__main__':
    gen_message_classes()


