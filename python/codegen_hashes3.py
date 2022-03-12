#!/usr/bin/env python

#-------------------------------------------------------------------------------
# update_hashes.py - Pulls fnv references out of code and updates hashes.h/cpp
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

'''
Pulls hash references out of code and updates hashes.h/cpp.

Looks for patterns of type "HASH::[a-zA-Z_][a-zA-Z0-9_]*" in the code, and
inserts the corresponding fnv hashes in src/engine/hashes.h/cpp.
'''

import sys
import re
import pathlib

import path_utils

def fnv32a(s):
    hval = 0x811c9dc5
    fnv_32_prime = 0x01000193
    uint32_max = 2 ** 32
    for c in s:
        hval = hval ^ c
        hval = (hval * fnv_32_prime) % uint32_max
    return hval

# These are members of FNV class, which we don't want to confuse with
# references to precalculated FNV hashses.
EXCLUDE_PATTERNS = [b'HASH::hash_func',
                    b'HASH::reverse_hash',
                    ]

def process_file(fpath):
    d = fpath.read_bytes()
    return [s for s in re.findall(b'HASH::[a-zA-Z_][a-zA-Z0-9_]*', d) if s not in EXCLUDE_PATTERNS]

def should_process_file(fpath, paths):
    return (fpath.is_file() and
            fpath.suffix in ['.h', '.cpp', '.mm'] and
            not fpath.is_relative_to(paths.gaen_shaders_dir) and
            not fpath.is_relative_to(paths.project_shaders_dir))

def process_dir(path, paths):
    hashes = []
    for fpath in path.rglob('*'):
        if should_process_file(fpath, paths):
            hashes += process_file(fpath)
    return hashes

def build_hash_list(paths):
    hash_list = process_dir(paths.gaen_src_dir, paths)
    if paths.is_project:
        hash_list += process_dir(paths.project_src_dir, paths)
    hash_list = [hash[len(b'HASH::'):] for hash in hash_list]
    hash_list = sorted(set(hash_list), key=lambda s: s.lower())
    hash_list = [(hash, fnv32a(hash)) for hash in hash_list]
    return hash_list

def max_hash_name_len(hash_list):
    return max([len(h[0]) for h in hash_list])

def hashes_declarations(hash_list):
    max_len = max_hash_name_len(hash_list)
    code = b''.join([b'    static const u32 %s%s = 0x%08x; // %10d\n' % (h[0], b' ' * (max_len-len(h[0])), h[1], h[1]) for h in hash_list])
    return code[:-1] # strip trailing \n

def hashes_h_construct(hash_list, paths):
    htmpl = paths.hashes_h_tpl.read_bytes()
    return htmpl.replace(b'${hashes_const_declarations}', hashes_declarations(hash_list))

def hashes_initializations(hash_list):
    max_len = max_hash_name_len(hash_list)
    code = b''.join([b'    if (HASH::hash_func("%s")%s != HASH::%s) %s PANIC("Hash mismatch between Python and C++");\n' % (h[0],b' ' * (max_len-len(h[0])),h[0],b' ' * (max_len-len(h[0]))) for h in hash_list])
    return code[:-1] # strip trailing \n

def hashes_cpp_construct(hash_list, paths):
    cpptmpl = paths.hashes_cpp_tpl.read_bytes()
    return cpptmpl.replace(b'${hashes_map_insertions}', hashes_initializations(hash_list))

def update_hashes_files(binary_dir):
    paths = path_utils.Paths(binary_dir)

    hash_list = build_hash_list(paths)
    path_utils.write_file_if_different(paths.hashes_h, hashes_h_construct(hash_list, paths))
    path_utils.write_file_if_different(paths.hashes_cpp, hashes_cpp_construct(hash_list, paths))

if __name__=='__main__':
    update_hashes_files(sys.argv[1])
