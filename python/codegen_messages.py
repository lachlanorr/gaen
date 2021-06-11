#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen_messages.py - Generate c++ message classes
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2021 Lachlan Orr
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

import sys
import posixpath
import os
import dirs
from ruamel.yaml import YAML
yaml = YAML()

def read_file_data(path):
    if not os.path.exists(path):
        return None
    with open(path, 'r') as f:
        return f.read()

def replace_file_if_different(path, data):
    if read_file_data(path) != data:
        print 'Writing ' + path
        with open(path, 'wb') as out_f:
            out_f.write(data)
            return True
    return False

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
        if not i.startswith('<') and not i.startswith('"'):
            i = '"' + i + '"'
        inc.append('#include %s\n' % i)
    return ''.join(inc)

def camel_to_underscores(s):
    s = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', s)
    s = re.sub(r'([a-z\d])([A-Z])', r'\1_\2', s)
    s = s.strip('_')
    return s.lower()

class Templates:
    MessageClass   = read_file_data(posixpath.join(python_path(), 'templates/message_template.cpp'))
    CmakeFileList  = read_file_data(posixpath.join(python_path(), 'templates/filelist_template.cmake'))

def output_path():
    return posixpath.join(sys.argv[1], 'messages')

def messages_def_path():
    return posixpath.join(gaen_path(), 'src/engine')

def gen_message_cmake(messages):
    lines = []
    for message in messages:
        lines.append('  "${CMAKE_CURRENT_BINARY_DIR}/messages/%s.h"' % message.name)
    lines.sort()
    return template_subst(Templates.CmakeFileList, {'files'       : '\n'.join(lines),
                                                    'autogen_type': 'messages'})

def read_yaml(path):
    with open(path) as f:
        return yaml.load(f)

class Field:
    def __str__(self):
        return repr(self.__dict__)
    def __repr__(self):
        return repr(self.__dict__)
class Message:
    def __str__(self):
        return repr(self.__dict__)

DEFAULT_INCLUDES = ['engine/MessageWriter.h']
CELLS_PER_BLOCK = 4 # cells per block, blocks are 16 bytes

def find_next_fit(fields, curr_cell):
    for item in fields:
        if item.is_assigned:
            continue
        if item.cell_count > 2 and curr_cell != 0:
            continue
        elif item.cell_count == 2 and curr_cell >= 3:
            continue

        return item
    return None

def upper_first(s):
    return s[0].upper() + s[1:]

def lower_first(s):
    return s[0].lower() + s[1:]

def block_accessor(field):
    if field.cell_count == 1:
        return 'cells[%d]' % field.cell_index
    elif field.cell_count == 2:
        return 'dCells[%d]' % (field.cell_index / 2,)
    elif field.cell_count == 3:
        return 'tCellPad.tCell'
    elif field.cell_count == 4:
        return 'qCell'
    else:
        raise Exception('Invalid field size for block accessor')

def fixup_fields(d, field_types):
    fields = []
    includes = list(DEFAULT_INCLUDES)
    has_payload = False
    unassigned_item_count = len(d)
    for k, v in d.iteritems():
        newfield = Field()
        newfield.name = k
        newfield.is_assigned = False
        if type(v) == str:
            newfield.type = v
        else: # dict
            newfield.__dict__.update(v)
            newfield.type = v['type']
            includes += v.get('includes', [])
        # pull in defaults
        field = Field()
        ft = field_types[newfield.type]
        includes += ft.get('includes', [])
        field.__dict__.update(field_types[newfield.type])
        field.__dict__.update(newfield.__dict__)

        if 'type_name' not in field.__dict__:
            field.type_name = field.type

        fields.append(field)

        if field.type == 'pointer':
            n = field.name
            if n.startswith('p'):
                n = n[1:]
            field.getter_name = lower_first(n)
        else:
            field.getter_name = field.name
        field.setter_name = 'set' + upper_first(field.getter_name)

        field.block_count = field.cell_count / CELLS_PER_BLOCK

        # handle payload
        if not has_payload and field.cell_count == 1:
            has_payload = True
            field.is_payload = True
            field.is_assigned = True
            unassigned_item_count -= 1
        else:
            field.is_payload = False

    fields.sort(reverse = True, key = lambda x: x.cell_count)

    # this algorithm is largely based (and should remain so) upon the
    # block_pack_items function within gaen/src/compose/codegen_utils.cpp
    curr_block = 0
    curr_cell = 0
    while unassigned_item_count > 0:
        item = find_next_fit(fields, curr_cell)
        if item is None:
            curr_block += 1
            curr_cell = 0
        else:
            item.is_assigned = True
            unassigned_item_count -= 1
            item.block_index = curr_block
            item.cell_index = curr_cell

            curr_block += item.cell_count / CELLS_PER_BLOCK
            curr_cell += item.cell_count % CELLS_PER_BLOCK

            if curr_cell >= CELLS_PER_BLOCK:
                curr_block += 1
                curr_cell = 0

    for field in fields:
        if not field.is_payload:
            if field.block_count > 1:
                field.member_var  = 'm'  + upper_first(field.name)
                field.member_pvar = 'mp' + upper_first(field.name)
            else: # field.block_count <= 1
                field.block_accessor = block_accessor(field)

    if curr_cell == 0:
        block_count = curr_block
    else:
        block_count = curr_block + 1

    return block_count, sorted(set(includes)), fields

def parse_messages_def():
    message_defs = read_yaml(posixpath.join(messages_def_path(), 'messages_def.yaml'))
    field_types = read_yaml(posixpath.join(messages_def_path(), 'field_types.yaml'))
    messages = []
    for k, v in message_defs.iteritems():
        m = Message()
        m.name = k
        m.block_count, m.includes, m.fields = fixup_fields(v, field_types)
        messages.append(m)
    return messages

def gen_reader_member_init(message):
    lines = []
    for f in message.fields:
        if f.block_count > 1:
            lines.append('        if (&msgAcc[%d] > &msgAcc[%d])' % (f.block_index + f.block_count - 1, f.block_index))
            lines.append('        {')
            lines.append('            // field is contiguous in ring buffer')
            lines.append('            %s = reinterpret_cast<const %s*>(&msgAcc[%d]);' % (f.member_pvar, f.type_name, f.block_index))
            lines.append('        }')
            lines.append('        else')
            lines.append('        {')
            lines.append('            // field wraps ring buffer, copy it into our datamember')
            lines.append('            for (u32 i = 0; i < %d; ++i)' % f.block_count)
            lines.append('            {')
            lines.append('                block_at(&%s, i) = msgAcc[i + %d];' % (f.member_var, f.block_index))
            lines.append('            }')
            lines.append('            %s = &%s;' % (f.member_pvar, f.member_var))
            lines.append('        }')
            lines.append('')

    return '\n'.join(lines)

def gen_reader_getters(message):
    lines = []
    for f in message.fields:
        if f.is_payload:
            lines.append('    %s %s() const { return (%s)mMsgAcc.message().payload.%s; }' % (f.type_name, f.name, f.type_name, f.union_type))
        elif f.type == 'pointer':
            lines.append('    %s %s() const { return static_cast<%s>(mMsgAcc[%d].%s.p); }' % (f.type_name, f.getter_name, f.type_name, f.block_index, f.block_accessor))
        elif f.type == 'vec3':
            lines.append('    const %s & %s() const { return *reinterpret_cast<const %s*>(&mMsgAcc[%d].%s); }' % (f.type_name, f.name, f.type_name, f.block_index, f.block_accessor))
        elif f.block_count <= 1:
            lines.append('    %s %s() const { return mMsgAcc[%d].%s.%s; }' % (f.type_name, f.name, f.block_index, f.block_accessor, f.union_type))
        else:
            lines.append('    const %s & %s() const { return *%s; }' % (f.type_name, f.name, f.member_pvar))
    return '\n'.join(lines)

def gen_reader_data_members(message):
    lines = []
    for f in message.fields:
        if f.block_count > 1:
            lines.append('    %s %s;' % (f.type_name, f.member_var))
            lines.append('    const %s * %s;' % (f.type_name, f.member_pvar))

    if len(lines) > 0:
        lines.insert(0, '    // These members provide storage for fields larger than a block that wrap the ring buffer')

    return '\n'.join(lines)

def gen_payload_decl(message):
    for f in message.fields:
        if f.is_payload:
            return ',\n%s      %s %s' % (' ' * (len(message.name)+1), f.type_name, f.name)
    return ''

def gen_payload_value(message):
    for f in message.fields:
        if f.is_payload:
            if f.type_name in ('u32', 'i32'):
                return f.name
            else:
                # add explicit casting as it's probably necessary for this type
                return "*reinterpret_cast<const u32*>(&%s)" % f.name
    return '0'


def gen_writer_setters(message):
    lines = []
    for f in message.fields:
        if f.is_payload:
            lines.append('    void %s(%s val) { mMsgAcc.message().payload.%s = (%s)val; }' % (f.setter_name, f.type_name, f.union_type, f.type))
        elif f.type == 'pointer':
            lines.append('    void %s(%s pVal) { mMsgAcc[%d].%s.p = (void*)pVal; }' % (f.setter_name, f.type_name, f.block_index, f.block_accessor))
        elif f.type == 'vec3':
            lines.append('    void %s(const %s & val) { mMsgAcc[%d].%s = *reinterpret_cast<const tcell*>(&val); }' % (f.setter_name, f.type_name, f.block_index, f.block_accessor))
        elif f.block_count <= 1:
            lines.append('    void %s(%s val) { mMsgAcc[%d].%s.%s = val; }' % (f.setter_name, f.type_name, f.block_index, f.block_accessor, f.union_type))
        else:
            lines.append('    void %s(const %s & val)' % (f.setter_name, f.type_name))
            lines.append('    {')
            lines.append('        for (u32 i = 0; i < %d; ++i)' % f.block_count)
            lines.append('        {')
            lines.append('            mMsgAcc[i + %d] = block_at(&val, i);' % f.block_index)
            lines.append('        }')
            lines.append('    }')
    return '\n'.join(lines)

def gen_message_class(message):
    repl = {'message_name_caps'      : message.name.upper(),
            'message_name'           : message.name,
            'message_name_indent'    : ' ' * (len(message.name) + 2),
            'includes'               : gen_includes(message.includes),
            'reader_data_member_init': gen_reader_member_init(message),
            'reader_getters'         : gen_reader_getters(message),
            'reader_data_members'    : gen_reader_data_members(message),
            'payload_decl'           : gen_payload_decl(message),
            'payload_value'          : gen_payload_value(message),
            'block_count'            : str(message.block_count),
            'writer_setters'         : gen_writer_setters(message),
    }

    return template_subst(Templates.MessageClass, repl)

def gen_message_classes():
    messages = parse_messages_def()

    if not os.path.exists(output_path()):
        os.makedirs(output_path())

    for message in messages:
        cpp_data = gen_message_class(message)
        cpp_path = posixpath.join(output_path(), message.name + '.h')
        replace_file_if_different(cpp_path, cpp_data)
    cmake_data = gen_message_cmake(messages)
    cmake_path = posixpath.join(output_path(), 'messages.cmake')
    if replace_file_if_different(cmake_path, cmake_data):
        # touch the engine/CMakeLists.txt file since we generated
        # messages.cmake and want to poke cmake to reprocess
        os.utime(posixpath.join(dirs.GAEN_SRC_DIR, 'engine/CMakeLists.txt'), None)


if __name__ == '__main__':
    gen_message_classes()


