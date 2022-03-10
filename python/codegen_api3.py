#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen_api.py - Update system_api_meta.cpp with definitions from system_api.h
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2022 Lachlan Orr
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

import os
import sys
import posixpath
import re

import dirs3
DIRS = None

class ParseUtil(object):
    '''
    Read types from compiler.cpp and prepare regular expressions
    '''
    def __init__(self):
        d = DIRS.compose_compiler_cpp_file.read_bytes()
        lines = [line.rstrip() for line in d.split(b'\n')]

        self.cpp_types = []
        self.cmp_types = []
        i = 0
        in_cpp_type_str_func = False
        for line in lines:
            if not in_cpp_type_str_func:
                if b'void register_basic_types(ParseData * pParseData)' in line:
                    in_cpp_type_str_func = True
            else:
                if b'}' in line:
                    if len(self.cpp_types) == 0 or len(self.cpp_types) != len(self.cmp_types):
                        raise Exception('Failed to parse types in %s' % DIRS.compose_compiler_cpp_file)
                    break
                m = re.search(rb'register_basic_type\([^,]+,\s+\"([^\"]+)\",\s+\"([^\"]+)\",.*', line)
                if m:
                    self.cmp_types.append(m.group(1))
                    self.cpp_types.append(m.group(2))
        if not in_cpp_type_str_func:
            raise Exception('Failed to find types in %s' % DIRS.compose_compiler_cpp_file)

        self.cpp_to_cmp_type = dict(zip(self.cpp_types, self.cmp_types))
        self.cmp_to_cpp_type = dict(zip(self.cmp_types, self.cpp_types))

        self.api_line_re = re.compile(rb'.*^namespace\s+gaen\s*{.*^namespace\s+system_api\s*{([^}]*)}.*', re.MULTILINE | re.DOTALL)

        cpp_type_match = b'(' + b'|'.join(self.cpp_types) + b')'
        self.const_start_re = re.compile(rb'^[\s]*static[\s]+const[\s]+.*$')
        self.const_parse_re = re.compile(rb'^[\s]*static[\s]+const[\s]+' + cpp_type_match + rb'[\s]+([^\s]+)[\s]+=[\s]+([^\s;]+);[\s]*$')
        self.func_start_re = re.compile(rb'^[\s]*' + cpp_type_match + rb'[\s]+([a-zA-Z0-9_]+)\(.*$')
        self.func_parse_re = re.compile(cpp_type_match + rb'[\s]+([^\s]+)[\s]*\(([^\)]*)\);')

def get_api_lines_in_file(path, parseUtil):
    data = path.read_bytes()

    # Only pull out the lines within the gaen::system_api namespace.
    # This regex will not be perfect, but if users adhere to the coding standards
    # of the rest of the gaen library, this will work well enough. The alternative
    # is parsing C++, which is overkill really.
    m = parseUtil.api_line_re.match(data)

    if not m:
        return []

    data = m.group(1).strip()
    return [line.rstrip() for line in data.splitlines()]

def get_api_lines_in_dir(path, lines, includes, parseUtil):
    for fpath in path.rglob('*.h'):
        api_lines = get_api_lines_in_file(fpath, parseUtil)
        if len(api_lines):
            lines += api_lines
            includes.append(fpath.relative_to(path).as_posix())

def get_api_lines(parseUtil):
    lines = []
    includes = []
    get_api_lines_in_dir(DIRS.gaen_src_dir, lines, includes, parseUtil)
    if DIRS.is_project:
        get_api_lines_in_dir(DIRS.project_src_dir, lines, includes, parseUtil)
    return lines, includes


def append_if_api(line, api_strs, parseUtil):
    if line.endswith(b', Entity * pCaller);') or line.endswith(b'(Entity * pCaller);'):
        api_strs.append(line)
    else:
        print("Apparent API not being passed an 'Entity * pCaller' as last parameter: " + line.decode('utf-8'))

def get_api_strs(lines, parseUtil):
    in_call = False
    api_strs = []
    curr_line = b''
    i = 0
    for line in lines:
        if in_call:
            line = b' ' + line.strip()
            curr_line += line
            if line.endswith(b');'):
                append_if_api(curr_line, api_strs, parseUtil)
                in_call = False
        else: # not in_call
            line = line.strip()
            if parseUtil.const_start_re.match(line):
                api_strs.append(line)
                curr_line = b''
            elif parseUtil.func_start_re.match(line):
                if line.endswith(b');'):
                    append_if_api(line, api_strs, parseUtil)
                    curr_line = b''
                else:
                    in_call = True
                    curr_line = line
    return api_strs

def parse_param(param_str, parseUtil):
    items = param_str.split(b'&') # for const params
    is_ref = len(items) == 2
    if not is_ref: # param isn't a reference
        items = param_str.rsplit(b' ', 1)
    items = items[0].split() + items[1].split()
    const = b''
    has_const = items[0] == b'const' or items[1] == b'const'
    if has_const:
        items.remove(b'const')
    if len(items) != 2:
        raise Exception("Param contains too many tokens: " + param_str.decode('utf-8'))
    cpp_type, name = items
    if cpp_type not in parseUtil.cpp_types:
        raise Exception("Invalid param type: " + param_str.decode('utf-8'))
    return (cpp_type, parseUtil.cpp_to_cmp_type[cpp_type], name, 1 if has_const else 0, 1 if is_ref else 0)

def parse_api_str(api_str, parseUtil):
    m = parseUtil.const_parse_re.match(api_str)
    if m:
        return { 'category': b'const', 'name': m.group(2), 'type': (m.group(1), parseUtil.cpp_to_cmp_type[m.group(1)]), 'value': m.group(3) }
    m = parseUtil.func_parse_re.match(api_str)
    if not m:
        raise Exception("api_str does not match expeced regex: " + api_str.decode('utf-8'))
    else:
        rettype = (m.group(1), parseUtil.cpp_to_cmp_type[m.group(1)])
        name = m.group(2)
        params = [p.strip() for p in m.group(3).split(b',')]
        if len(params) < 1 or b'Entity *' not in params[-1]:
            raise Exception("last parameter of api must be of type Entity *: " + api_str.decode('utf-8'))
        params = params[:-1] # remove the last 'Entity *' parameter
        params = [parse_param(p, parseUtil) for p in params]
        return { 'category': b'func', 'name': name, 'type': rettype, 'params': params }

def reg_api(parsed_api):
    if parsed_api['category'] == b'func':
        sarr = []

        sarr.append(b'    {\n')
        sarr.append(b'        // function %s\n' % parsed_api['name'])

        sarr.append(b'        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);\n')
        for i in range(len(parsed_api['params'])):
            sarr.append(b'        ast_add_child(pSystemApiDef, ast_create_function_arg("%s", parsedata_find_type_symbol(pParseData, "%s", %d, %d), pParseData));\n' % (parsed_api['params'][i][2], parsed_api['params'][i][1], parsed_api['params'][i][3], parsed_api['params'][i][4]))

        sarr.append(b'        size_t mangledLen = mangle_function_len("%s", pSystemApiDef->pChildren);\n' % parsed_api['name'])
        sarr.append(b'        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);\n')
        sarr.append(b'        mangle_function(mangledName, kMaxCmpId, "%s", pSystemApiDef->pChildren);\n' % parsed_api['name'])
        sarr.append(b'        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "%s", 0, 0)->pSymDataType, mangledName, pSystemApiDef, nullptr, pParseData));\n' % parsed_api['type'][1])

        sarr.append(b'    }\n')
        return b''.join(sarr)
    elif parsed_api['category'] == b'const':
        sarr = []
        sarr.append(b'    {\n')
        sarr.append(b'        // const %s\n' % parsed_api['name'])
        sarr.append(b'        Ast * pSystemConstDef = ast_create(kAST_SystemConstDef, pParseData);\n')
        sarr.append(b'        pSystemConstDef->pSymRec = symrec_create(kSYMT_SystemConst, parsedata_find_type_symbol(pParseData, "%s", 0, 0)->pSymDataType, "%s", pSystemConstDef, nullptr, pParseData);\n' % (parsed_api['type'][1], parsed_api['name']))
        sarr.append(b'        parsedata_add_root_symbol(pParseData, pSystemConstDef->pSymRec);\n')
        sarr.append(b'    }\n')
        return b''.join(sarr)
    else:
        raise Exception("unknown category: " + repr(parsed_api))

def build_metadata(parseUtil):
    lines, includes = get_api_lines(parseUtil)
    api_strs = get_api_strs(lines, parseUtil)
    reg_apis = [reg_api(parse_api_str(api_str, parseUtil)) for api_str in api_strs]
    tmpl = DIRS.system_api_meta_template_cpp_file.read_bytes()
    return tmpl.replace(b'${reg_apis}', b''.join(reg_apis)), includes

def read_file(path):
    if path.is_file():
        return path.read_bytes()
    return b''

def write_metadata(binary_dir):
    global DIRS
    DIRS = dirs3.Dirs(binary_dir)

    parseUtil = ParseUtil()
    new_data, includes = build_metadata(parseUtil)
    old_data = read_file(DIRS.system_api_meta_output_cpp_file)
    if new_data != old_data:
        print("Writing " + str(DIRS.system_api_meta_output_cpp_file))
        DIRS.system_api_meta_output_cpp_file.write_bytes(new_data)
    return includes

if __name__=='__main__':
    write_metadata(sys.argv[1])
