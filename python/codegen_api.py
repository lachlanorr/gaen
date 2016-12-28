#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen_api.py - Update system_api_meta.cpp with definitions from system_api.h
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2016 Lachlan Orr
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

import dirs

META_TEMPLATE = '''\
//------------------------------------------------------------------------------
// system_api_meta.cpp - Autogenerated with codegen.py from system_api.h
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#include "core/HashMap.h"

#include "compose/compiler_structs.h"

namespace gaen
{

void register_system_apis(ParseData * pParseData)
{
<<reg_apis>>
}


} // namespace gaen
'''

def get_compiler_cpp_lines():
    return [line.rstrip() for line in open(dirs.COMPILER_CPP_CPP_FILE)]

def read_types():
    '''
    Read types from compiler_cpp.cpp so we don't have to maintain
    a separate list here.'
    '''
    lines = get_compiler_cpp_lines()
    cpp_types = []
    compose_types = []
    i = 0
    in_cpp_type_str_func = False
    while i < len(lines):
        line = lines[i]
        if not in_cpp_type_str_func:
            if "void register_basic_types(ParseData * pParseData)" in line:
                in_cpp_type_str_func = True
        else:
            if "}" in line:
                if len(cpp_types) == 0 or len(cpp_types) != len(compose_types):
                    raise Exception('Failed to parse types from compiler_cpp.cpp')
                return cpp_types, compose_types
            m = re.search(r'register_basic_type\([^,]+,\s+\"([^\"]+)\",\s+\"([^\"]+)\",.*', line)
            if m:
                compose_types.append(m.group(1))
                cpp_types.append(m.group(2))
        i += 1
    raise Exception('Expected to find "default:" case but did not')

CPP_TYPES, COMPOSE_TYPES = read_types()
CPP_TO_COMPOSE_TYPE = dict(zip(CPP_TYPES, COMPOSE_TYPES))
COMPOSE_TO_CPP_TYPE = dict(zip(COMPOSE_TYPES, CPP_TYPES))

def get_api_lines_in_file(in_path):
    with open(in_path) as in_f:
        data = in_f.read()

    # Only pull out the lines within the gaen::system_api namespace.
    # This regex will not be perfect, but if users adhere to the coding standards
    # of the rest of the gaen library, this will work well enough. The alternative
    # is parsing C++, which is overkill really.
    m = re.match('.*^namespace\s+gaen\s*{.*^namespace\s+system_api\s*{([^}]*)}.*', data, re.MULTILINE | re.DOTALL)

    if not m:
        return []
        
    data = m.group(1).strip()
    return [line.rstrip() for line in data.splitlines()]

def get_api_lines_in_dir(dir_path, lines, includes):
    for root, dirs, files in os.walk(dir_path):
        for f in files:
            if f.endswith('.h'):
                full_path= os.path.join(root, f)
                api_lines = get_api_lines_in_file(full_path)
                if len(api_lines):
                    lines += api_lines
                    pathdir = os.path.split(full_path)[0]
                    fname = os.path.split(full_path)[1]
                    lastdir = os.path.split(pathdir)[1]
                    includes.append(posixpath.join(lastdir, fname))

def get_api_lines():
    lines = []
    includes = []
    get_api_lines_in_dir(dirs.GAEN_SRC_DIR, lines, includes)
    if dirs.IS_PROJECT:
        get_api_lines_in_dir(dirs.PROJECT_SRC_DIR, lines, includes)
    return lines, includes

def type_regex():
    return '(' + '|'.join(CPP_TYPES) + ')'

def api_start_regex():
    return r'^[\s]*' + type_regex() + r'[\s]+([a-zA-Z0-9_]+)\(.*$'

def append_if_api(line, api_strs):
    if line.endswith(", Entity * pCaller);") or line.endswith("(Entity * pCaller);"):
        api_strs.append(line)
    elif line != 'ApiResult get_last_result();':
        print "Apparent API not being passed an 'Entity * pCaller' as last parameter: " + line

def get_api_strs(lines):
    in_call = False
    api_strs = []
    curr_line = ''
    i = 0
    while i < len(lines):
        line = lines[i]
        if in_call:
            line = ' ' + line.strip()
            curr_line += line
            if line.endswith(');'):
                append_if_api(curr_line, api_strs)
                in_call = False
        else: # not in_call
            if re.match(api_start_regex(), line):
                line = line.strip()
                if line.endswith(');'):
                    append_if_api(line, api_strs)
                    curr_line = ''
                else:
                    in_call = True
                    curr_line = line
        i += 1
    return api_strs

def parse_param(param_str):
    items = param_str.split('&') # for const params
    is_ref = len(items) == 2
    if not is_ref: # param isn't a reference
        items = param_str.rsplit(' ', 1)
    items = items[0].split() + items[1].split()
    const = ''
    has_const = items[0] == 'const' or items[1] == 'const'
    if has_const:
        items.remove('const')
    if len(items) != 2:
        raise Exception("Param contains too many tokens: " + param_str)
    cpp_type, name = items
    if cpp_type not in CPP_TYPES:
        raise Exception("Invalid param type: " + param_str)
    return (cpp_type, CPP_TO_COMPOSE_TYPE[cpp_type], name, 1 if has_const else 0, 1 if is_ref else 0)
            

def parse_api_str(api_str):
    m = re.match(type_regex() + r'[\s]+([^\s]+)[\s]*\(([^\)]*)\);', api_str)
    if not m:
        raise Exception("api_str does not match expeced regex: " + api_str)
    else:
        rettype = (m.group(1), CPP_TO_COMPOSE_TYPE[m.group(1)])
        name = m.group(2)
        params = [p.strip() for p in m.group(3).split(',')]
        if len(params) < 1 or "Entity *" not in params[-1]:
            raise Exception("last parameter of api must be of type Entity *: " + api_str)
        params = params[:-1] # remove the last 'Entity *' parameter
        params = [parse_param(p) for p in params]
        return name, rettype, params
    
def reg_api(parsed_api):
    sarr = []

    sarr.append('    {\n')
    sarr.append('        // %s\n' % parsed_api[0])

    sarr.append('        Ast * pSystemApiDef = ast_create(kAST_SystemApiDef, pParseData);\n')
    for i in range(len(parsed_api[2])):
        sarr.append('        ast_add_child(pSystemApiDef, ast_create_function_arg("%s", parsedata_find_type_symbol(pParseData, "%s", %d, %d), pParseData));\n' % (parsed_api[2][i][2], parsed_api[2][i][1], parsed_api[2][i][3], parsed_api[2][i][4]))

    sarr.append('        size_t mangledLen = mangle_function_len("%s", pSystemApiDef->pChildren);\n' % parsed_api[0])
    sarr.append('        char * mangledName = (char*)COMP_ALLOC(mangledLen + 1);\n')
    sarr.append('        mangle_function(mangledName, kMaxCmpId, "%s", pSystemApiDef->pChildren);\n' % parsed_api[0])
    sarr.append('        parsedata_add_root_symbol(pParseData, symrec_create(kSYMT_SystemApi, parsedata_find_type_symbol(pParseData, "%s", 0, 0)->pSymDataType, mangledName, pSystemApiDef, nullptr, pParseData));\n' % parsed_api[1][1])

    sarr.append('    }\n')
    return ''.join(sarr)

def build_metadata():
    lines, includes = get_api_lines()
    api_strs = get_api_strs(lines)
    reg_apis = [reg_api(parse_api_str(api_str)) for api_str in api_strs]
    return META_TEMPLATE.replace("<<reg_apis>>", ''.join(reg_apis)), includes

def read_file(filename):
    if os.path.exists(filename):
        f = open(filename, 'r')
        d = f.read()
        f.close()
    else:
        d = ''
    return d

def write_metadata():
    system_api_meta_path = sys.argv[1]
    new_data, includes = build_metadata()
    old_data = read_file(system_api_meta_path)
    if new_data != old_data:
        print "Writing " + system_api_meta_path
        with open(system_api_meta_path, 'wb') as f:
            f.write(new_data)
    return includes

if __name__=='__main__':
    write_metadata()

