#!/usr/bin/env python

#-------------------------------------------------------------------------------
# update_hashes.py - Pulls fnv references out of code and updates hashes.h/cpp
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

'''
Pulls hash references out of code and updates hashes.h/cpp.

Looks for patterns of type "HAHS::[a-zA-Z_][a-zA-Z0-9_]*" in the code, and
inserts the corresponding fnv hashes in src/engine/hashes.h/cpp.
'''

import os
import re

def fnv32a(s):
    hval = 0x811c9dc5
    fnv_32_prime = 0x01000193
    uint32_max = 2 ** 32
    for c in s:
        hval = hval ^ ord(c)
        hval = (hval * fnv_32_prime) % uint32_max
    return hval
 
# These are members of FNV class, which we don't want to confuse with
# references to precalculated FNV hashses.
EXCLUDE_PATTERNS = ['HASH::hash_func',
                    'HASH::reverse_hash',
                    ]
                    
def process_file(path):
    with open(path) as f:
        d = f.read()
        return [s for s in re.findall('HASH::[a-zA-Z_][a-zA-Z0-9_]*', d) if s not in EXCLUDE_PATTERNS]
    
def process_dir(path):
    hashes = []
    for root, dirs, files in os.walk(path):
        for f in files:
            if f not in ['hashes.h', 'hashes.cpp']:
                ext = os.path.splitext(f)[1]
                if ext in ['.h', '.cpp', '.mm']:
                    fullpath = os.path.join(root, f)
                    hashes += process_file(fullpath)
    return hashes
            

def src_dir():
    scriptdir = os.path.split(os.path.abspath(__file__))[0]
    gaendir = os.path.split(scriptdir)[0]
    srcdir = os.path.join(gaendir, 'src')
    return srcdir

def project_src_dir():
    scriptdir = os.path.split(os.path.abspath(__file__))[0]
    gaendir = os.path.split(scriptdir)[0]
    projectdir = os.path.split(gaendir)[0]
    project_srcdir = os.path.join(projectdir, 'src')
    if os.path.exists(project_srcdir):
        return project_srcdir
    else:
        return None

def hashes_h_path():
    return os.path.join(src_dir(), 'engine/hashes.h')

def hashes_cpp_path():
    return os.path.join(src_dir(), 'engine/hashes.cpp')

def build_hash_list():
    hash_list = process_dir(src_dir())
    psrc = project_src_dir()
    if psrc:
        hash_list += process_dir(psrc)
    hash_list = [hash[len("HASH::"):] for hash in hash_list]
    hash_list = sorted(set(hash_list), key=lambda s: s.lower())
    hash_list = [(hash, fnv32a(hash)) for hash in hash_list]
    return hash_list

def max_hash_name_len(hash_list):
    return max([len(h[0]) for h in hash_list])

def hashes_declarations(hash_list):
    max_len = max_hash_name_len(hash_list)
    return ''.join(['    static const u32 %s%s = 0x%08x; // %10d\n' % (h[0], ' ' * (max_len-len(h[0])), h[1], h[1]) for h in hash_list])

def hashes_h_construct(hash_list):
    return HASHES_H_TEMPLATE.replace('<<hashes_const_declarations>>', hashes_declarations(hash_list))

def hashes_initializations(hash_list):
    max_len = max_hash_name_len(hash_list)
    return ''.join(['    if (HASH::hash_func("%s")%s != HASH::%s) %s PANIC("Hash mismatch between Python and C++");\n' % (h[0],' ' * (max_len-len(h[0])),h[0],' ' * (max_len-len(h[0]))) for h in hash_list])

def hashes_cpp_construct(hash_list):
    return HASHES_CPP_TEMPLATE.replace('<<hashes_map_insertions>>', hashes_initializations(hash_list))

def is_file_different(path, data):
    if os.path.isfile(path):
        f = open(path)
        d = f.read()
        f.close()
        return d != data
    return True

def write_file_if_different(path, new_data):
    if is_file_different(path, new_data):
        f = open(path, 'w')
        f.write(new_data)
        f.close()

def update_hashes_files():
    hash_list = build_hash_list()
    write_file_if_different(hashes_h_path(), hashes_h_construct(hash_list))
    write_file_if_different(hashes_cpp_path(), hashes_cpp_construct(hash_list))

def main():
    hash_list = build_hash_list()
    update_hashes_files()


HASHES_H_TEMPLATE = ('//------------------------------------------------------------------------------\n'
                  '// hashes.h - Precalculated hashes, generated by update_hashes.py\n'
                  '//\n'
                  '// Gaen Concurrency Engine - http://gaen.org\n'
                  '// Copyright (c) 2014-2015 Lachlan Orr\n'
                  '//\n'
                  '// This software is provided \'as-is\', without any express or implied\n'
                  '// warranty. In no event will the authors be held liable for any damages\n'
                  '// arising from the use of this software.\n'
                  '//\n'
                  '// Permission is granted to anyone to use this software for any purpose,\n'
                  '// including commercial applications, and to alter it and redistribute it\n'
                  '// freely, subject to the following restrictions:\n'
                  '//\n'
                  '//   1. The origin of this software must not be misrepresented; you must not\n'
                  '//   claim that you wrote the original software. If you use this software\n'
                  '//   in a product, an acknowledgment in the product documentation would be\n'
                  '//   appreciated but is not required.\n'
                  '//\n'
                  '//   2. Altered source versions must be plainly marked as such, and must not be\n'
                  '//   misrepresented as being the original software.\n'
                  '//\n'
                  '//   3. This notice may not be removed or altered from any source\n'
                  '//   distribution.\n'
                  '//------------------------------------------------------------------------------\n'
                  '\n'
                  '#ifndef GAEN_ENGINE_HASHES_H\n'
                  '#define GAEN_ENGINE_HASHES_H\n'
                  '\n'
                  '#include "core/HashMap.h"\n'
                  '#include "core/base_defines.h"\n'
                  '\n'
                  '// Determines if string collisions are detected in hashes\n'
                  '#define TRACK_HASHES HAS_X\n'
                  '\n'
                  'namespace gaen\n'
                  '{\n'
                  '\n'
                  'class HASH\n'
                  '{\n'
                  'public:\n'
                  '    static u32 hash_func(const char * str);\n'
                  '    static const char * reverse_hash(u32 hash);\n'
                  '\n'
                  '    // Pre calculated hashes.\n'
                  '    // Generated with update_hashes.py, which gets run\n'
                  '    // during the build.\n'
                  '<<hashes_const_declarations>>'
                  '};\n'
                  '\n'
                  '} // namespace gaen\n'
                  '\n'
                  '\n'
                  '#endif // #ifndef GAEN_ENGINE_HASHES_H\n'
                 )

HASHES_CPP_TEMPLATE = ('//------------------------------------------------------------------------------\n'
                    '// hashes.cpp - Precalculated hashes, generated by update_hashes.py\n'
                    '//\n'
                    '// Gaen Concurrency Engine - http://gaen.org\n'
                    '// Copyright (c) 2014-2015 Lachlan Orr\n'
                    '//\n'
                    '// This software is provided \'as-is\', without any express or implied\n'
                    '// warranty. In no event will the authors be held liable for any damages\n'
                    '// arising from the use of this software.\n'
                    '//\n'
                    '// Permission is granted to anyone to use this software for any purpose,\n'
                    '// including commercial applications, and to alter it and redistribute it\n'
                    '// freely, subject to the following restrictions:\n'
                    '//\n'
                    '//   1. The origin of this software must not be misrepresented; you must not\n'
                    '//   claim that you wrote the original software. If you use this software\n'
                    '//   in a product, an acknowledgment in the product documentation would be\n'
                    '//   appreciated but is not required.\n'
                    '//\n'
                    '//   2. Altered source versions must be plainly marked as such, and must not be\n'
                    '//   misrepresented as being the original software.\n'
                    '//\n'
                    '//   3. This notice may not be removed or altered from any source\n'
                    '//   distribution.\n'
                    '//------------------------------------------------------------------------------\n'
                    '\n'
                    '#include "engine/stdafx.h"\n'
                    '\n'
                    '#include "core/hashing.h"\n'
                    '#include "engine/hashes.h"\n'
                    '\n'
                    '#if HAS(TRACK_HASHES)\n'
                    '#include <mutex>\n'
                    '#include "core/HashMap.h"\n'
                    '#include "core/String.h"\n'
                    '#endif // #if HAS(TRACK_HASHES)\n'
                    '\n'
                    '#define HASH_FUNC ::gaen::gaen_hash\n'
                    '\n'
                    'namespace gaen\n'
                    '{\n'
                    '\n'
                    '#if HAS(TRACK_HASHES)\n'
                    'typedef HashMap<kMEM_Debug, u32, String<kMEM_Debug>> TrackMap;\n'
                    'static std::mutex sTrackMapMutex;\n'
                    'static TrackMap sTrackMap;\n'
                    '// Insert all our precalculated hashes into sTrackMap so we will know\n'
                    '// if we encounter conflicts in dynamically calculated hashes.  Also,\n'
                    '// verify the precalculted hashes match what our C++ version returns.\n'
                    'bool build_initial_track_map()\n'
                    '{\n'
                    '    sTrackMap.rehash(8192);\n'
                    '<<hashes_map_insertions>>'
                    '    return true;\n'   
                    '}\n'
                    '\n'
                    'namespace\n'
                    '{\n'
                    '    bool ret = build_initial_track_map();\n'   
                    '}\n'
                    '#endif // #if HAS(TRACK_HASHES)\n'
                    '\n'
                    'u32 HASH::hash_func(const char *str)\n'
                    '{\n'
                    '    ASSERT(str);\n'
                    '\n'
                    '    u32 hval = HASH_FUNC(str);\n'
                    '\n'
                    '#if HAS(TRACK_HASHES)\n'
                    '    {\n'
                    '        std::lock_guard<std::mutex> lock(sTrackMapMutex);\n'
                    '        auto it = sTrackMap.find(hval);\n'
                    '        if (it != sTrackMap.end())\n'
                    '        {\n'
                    '            // If strings don\'t match, you have two distinct strings whose\n'
                    '            // hashes clash.  This will likely break stuff in horrific\n'
                    '            // ways.  You need to tweak one of the strings, change its\n'
                    '            // value slightly and the problem should be resolved.\n'
                    '            if (0 != strcmp(it->second.c_str(), str))\n'
                    '                ERR("Hash Clash Detected: %s and %s", it->second.c_str(), str);\n'
                    '        }\n'
                    '        else\n'
                    '        {\n'
                    '            sTrackMap.emplace(hval, str);\n'
                    '        }\n'
                    '    }\n'
                    '#endif // #if HAS(TRACK_HASHES)\n'
                    '\n'
                    '    // return our new hash value\n'
                    '    return hval;\n'
                    '}\n'
                    '\n'
                    'const char * HASH::reverse_hash(u32 hash)\n'
                    '{\n'
                    '#if HAS(TRACK_HASHES)\n'
                    '    {\n'
                    '        std::lock_guard<std::mutex> lock(sTrackMapMutex);\n'
                    '        auto it = sTrackMap.find(hash);\n'
                    '        if (it != sTrackMap.end())\n'
                    '        {\n'
                    '            return it->second.c_str();\n'
                    '        }\n'
                    '        else\n'
                    '        {\n'
                    '            return "HASH_NOT_FOUND";\n'
                    '        }\n'
                    '    }\n'
                    '#else  // #if HAS(TRACK_HASHES)\n'
                    '    return "HASH_TRACKING_NOT_ENABLED";\n'
                    '#endif // #if HAS(TRACK_HASHES)\n'
                    '}\n'
                    '\n'
                    '} // namespace gaen\n'
                    '\n'
                   )


if __name__=='__main__':
    main()
