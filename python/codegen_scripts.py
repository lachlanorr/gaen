#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen_scripts.py - Run cmpc on all compose scripts to generate cpp files
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

import sys
import subprocess
import hashlib
import datetime
import string
import posixpath
import re

import codegen_api
import path_utils

H_SECTION_RE = re.compile(rb'^/// \.H SECTION\n(.*)./// \.CPP SECTION\n.*$', flags=re.MULTILINE|re.DOTALL)
CPP_SECTION_RE = re.compile(rb'^.*/// \.CPP SECTION\n(.*)$', flags=re.MULTILINE|re.DOTALL)
REGISTER_RE = re.compile(r'void ((register_(entity|component)_([^\s]+))\([^\)]*\))')

def modification_time(fpath):
    return datetime.datetime.fromtimestamp(fpath.stat().st_mtime)

def check_file(fpath):
    if fpath.exists():
        return True, datetime.datetime.fromtimestamp(fpath.stat().st_mtime)
    else:
        return False, None

def read_cpp_file(fpath):
    d = fpath.read_text()

    hashloc = d.find('// HASH: ') + len('// HASH: ')

    hashval = d[hashloc:hashloc+32]
    source = d[hashloc+33:]

    return d, source, hashval

def write_file(fpath, data):
    dpath = fpath.parent
    dpath.mkdir(parents=True, exist_ok=True)
    fpath.write_bytes(data.encode('utf-8'))

class ScriptInfo:
    def __init__(self, cmp_full_path, paths):
        self.cmp_full_path = cmp_full_path
        out_path_work = paths.scripts_output_dir/'cpp'/(self.cmp_full_path.relative_to(paths.scripts_cmp_dir))
        self.cpp_full_path = out_path_work.with_suffix('.cpp')
        self.h_full_path = out_path_work.with_suffix('.h')
        self.cmp_filename = self.cmp_full_path.name
        self.cpp_filename = self.cpp_full_path.name
        self.h_filename = self.h_full_path.name

        self.cpp_exists, self.cppModTime = check_file(self.cpp_full_path)
        self.cmp_exists, self.cmpModTime = check_file(self.cmp_full_path)
        self.h_exists, self.hModTime = check_file(self.h_full_path)

        if (self.cpp_exists):
            self.cpp_output_old, self.cpp_source_old, self.cpp_source_old_hash = read_cpp_file(self.cpp_full_path)
            self.cpp_source_old_hash_actual = hashlib.md5(self.cpp_source_old.encode('utf-8')).hexdigest()

        if (self.h_exists):
            self.h_output_old, self.h_source_old, self.h_source_old_hash = read_cpp_file(self.h_full_path)
            self.h_source_old_hash_actual = hashlib.md5(self.h_source_old.encode('utf-8')).hexdigest()

        self._compile(paths)

        cpp_tpl = string.Template(paths.script_cpp_tpl.read_text())

        self.cpp_output = cpp_tpl.substitute(filename=self.cpp_filename,
                                             cmp_filename=self.cmp_full_path.name,
                                             license_text=license_text('//', paths.project_LICENSE),
                                             source_hash=self.cpp_source_hash,
                                             source=self.cpp_source)

        if (self.h_source is not None):
            self.h_output = cpp_tpl.substitute(filename=self.h_filename,
                                               cmp_filename=self.cmp_full_path.name,
                                               license_text=license_text('//', paths.project_LICENSE),
                                               source_hash=self.h_source_hash,
                                               source=self.h_source)


    def _compile(self, paths):
        rel_scr_inc = paths.script_includes_h.relative_to(paths.binary_dir/'gaen'/'src').as_posix()
        args = [paths.cmpc, '-i', rel_scr_inc, self.cmp_full_path]

        # LORRNOTE: Printing these always isn't useful, but keeping
        # these lines commented out for future debugging purposes.
        #for arg in args:
        #    print(arg),
        #print()

        p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        sout, serr = p.communicate()
        if p.returncode == 0:
            output = sout.replace(b'\r\n', b'\n')
            mh = H_SECTION_RE.match(output)
            if (mh):
                self.h_source = mh.group(1).decode('utf-8')
                self.h_source_hash = hashlib.md5(mh.group(1)).hexdigest();
            else:
                self.h_source = None

            mcpp = CPP_SECTION_RE.match(output)
            self.cpp_source = mcpp.group(1).decode('utf-8')
            self.cpp_source_hash = hashlib.md5(mcpp.group(1)).hexdigest();
            return True
        else:
            print(serr),
            return False

    def _should_write_cpp(self):
        if not self.cpp_exists:
            return True
        if self.cpp_output == self.cpp_output_old:
            return False
        return True

    def _should_write_h(self):
        if not self.h_exists:
            return True
        if self.h_output == self.h_output_old:
            return False
        return True

    def write_cpp(self):
        if (self.cpp_exists and
            self.cpp_source_old_hash != self.cpp_source_old_hash_actual):
            # file already exists and has been modified
            write_file(self.cpp_full_path + ".codegen", self.cpp_output)
            print("WARNING: %s has been modified and is not being replaced, diff with %s.codegen and manually apply the changes." % (self.cpp_full_path, self.cpp_full_path))
        elif self._should_write_cpp():
            print(self.cpp_filename)
            write_file(self.cpp_full_path, self.cpp_output)

        if (self.h_source is not None):
            if (self.h_exists and
                self.h_source_old_hash != self.h_source_old_hash_actual):
                # file already exists and has been modified
                write_file(self.h_full_path + ".codegen", self.h_output)
                print("WARNING: %s has been modified and is not being replaced, diff with %s.codegen and manually apply the changes." % (self.h_full_path, self.h_full_path))
            elif self._should_write_h():
                print(self.h_filename)
                write_file(self.h_full_path, self.h_output)

LICENSE_TEXTS = {} # memoize
def license_text(comment_chars, license_path):
    k = (comment_chars, license_path)
    if k in LICENSE_TEXTS:
        return LICENSE_TEXTS[k]

    lic = ''
    if license_path.exists():
        lines = [(comment_chars + ' ' + line).rstrip() for line in license_path.open()]
        lic = '\n' + comment_chars + '\n' + '\n'.join(lines)
    LICENSE_TEXTS[k] = lic
    return lic

def write_registration_cpp(script_infos, paths):
    registration_lines = []
    registration_call_lines = []
    for si in script_infos:
        for line in si.cpp_output.splitlines():
            m = REGISTER_RE.search(line)
            if m:
                registration_lines.append(m.group(1) + ';')
                registration_call_lines.append(m.group(2) + '(registry);')

    extern_cpp = '\n'.join(['extern void ' + i for i in sorted(registration_lines)])
    calls_cpp = '\n    '.join(sorted(registration_call_lines))
    registration_cpp_tpl = string.Template(paths.registration_cpp_tpl.read_text())
    reg_cpp = registration_cpp_tpl.substitute(extern_cpp=extern_cpp,
                                              calls_cpp=calls_cpp,
                                              license_text=license_text('//', paths.project_LICENSE))
    if not paths.registration_cpp.exists() or paths.registration_cpp.read_text() != reg_cpp:
        print(paths.registration_cpp.name)
        write_file(paths.registration_cpp, reg_cpp)


def cmakeify_bin_script_path(p, binary_dir):
    return p.as_posix().replace(binary_dir.as_posix(), '  ${CMAKE_BINARY_DIR}')

def cmakeify_src_script_path(p, scripts_dir):
    return p.as_posix().replace(scripts_dir.as_posix(), '  ${scripts_dir}')

def strip_src_scripts_dir(p):
    dirpath = posixpath.split(p.lstrip())[0]
    dirpath = dirpath.replace('${scripts_dir}', '')
    return dirpath

def strip_bin_scripts_dir(p, paths):
    dirpath = posixpath.split(p.lstrip())[0]
    dirpath = dirpath.replace('${CMAKE_BINARY_DIR}', paths.binary_dir.as_posix())
    dirpath = dirpath.replace(paths.scripts_output_dir.as_posix(), '')
    return dirpath

def write_cmake(cmp_files, cpp_files, h_files, paths):
    cmp_rel_files = [cmakeify_src_script_path(f, paths.scripts_dir) for f in cmp_files]
    cpp_rel_files = [cmakeify_bin_script_path(f, paths.binary_dir) for f in cpp_files]
    h_rel_files = [cmakeify_bin_script_path(f, paths.binary_dir) for f in h_files]
    ide_src_props = ['source_group("%s" FILES %s)' % (strip_src_scripts_dir(r)[1:], r.lstrip()) for r in cmp_rel_files]
    ide_src_props += ['source_group("%s" FILES %s)' % (strip_bin_scripts_dir(r, paths)[1:], r.lstrip()) for r in cpp_rel_files]
    ide_src_props += ['source_group("%s" FILES %s)' % (strip_bin_scripts_dir(r, paths)[1:], r.lstrip()) for r in h_rel_files]

    scripts_cmake_tpl = string.Template(paths.scripts_cmake_tpl.read_text())
    scripts_cmake = scripts_cmake_tpl.substitute(scripts_dir=paths.scripts_dir.as_posix().replace(paths.source_dir.as_posix(), "${CMAKE_SOURCE_DIR}"),
                                                 license_text=license_text('#', paths.project_LICENSE),
                                                 files='\n'.join(cmp_rel_files + cpp_rel_files + h_rel_files),
                                                 ide_source_props='\n'.join(ide_src_props))

    if not paths.scripts_cmake.exists() or paths.scripts_cmake.read_text() != scripts_cmake:
        print(paths.scripts_cmake.name)
        write_file(paths.scripts_cmake, scripts_cmake)
        # touch the scripts/CMakeLists.txt file since we generated
        # codegen.cmake and want to poke cmake to reprocess
        paths.scripts_cmakelists_txt.touch()

def find_cmp_files(paths):
    return list(paths.scripts_dir.rglob('*.cmp'))

def write_script_includes(includes, paths):
    include_lines = ['#include "%s"' % inc for inc in includes]
    script_includes_h_tpl = string.Template(paths.script_includes_h_tpl.read_text())
    script_includes_h = script_includes_h_tpl.substitute(source='\n'.join(include_lines),
                                                         license_text=license_text('//', paths.project_LICENSE))
    if not paths.script_includes_h.exists() or paths.script_includes_h.read_text() != script_includes_h:
        print(paths.script_includes_h)
        write_file(paths.script_includes_h, script_includes_h)

def main(binary_dir):
    paths = path_utils.Paths(binary_dir)

    paths.scripts_output_dir.mkdir(parents=True, exist_ok=True)

    # run codegen_api first to get include list from most
    # recent api code
    parseUtil = codegen_api.ParseUtil(paths)
    new_data, includes = codegen_api.build_metadata(parseUtil)

    write_script_includes(includes, paths)

    if not paths.cmpc.exists():
        print("ERROR: cmpc not found, do you need to build?")
        exit(1)
    script_infos = []
    cmp_files = []
    cpp_files = []
    h_files = []
    has_errors = False

    cmp_inputs = find_cmp_files(paths)

    for fcmp in cmp_inputs:
        try:
            si = ScriptInfo(fcmp, paths)
            si.write_cpp()
            script_infos.append(si)
            cmp_files.append(si.cmp_full_path)
            cpp_files.append(si.cpp_full_path)
            if (si.h_source is not None):
                h_files.append(si.h_full_path)
        except:
            print("ERROR: %s failed to compile" % f)
            raise
            has_errors = True;

    if not has_errors:
        write_registration_cpp(script_infos, paths)
        write_cmake(cmp_files, cpp_files, h_files, paths)


if __name__=='__main__':
    main(sys.argv[1])
