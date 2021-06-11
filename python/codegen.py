#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen.py - Run cmpc on all compose scripts to generate cpp files
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

import os
import sys
import posixpath
import StringIO
import subprocess
import md5
import datetime
import re

import codegen_api
import dirs

TEMPLATE = '''\
//------------------------------------------------------------------------------
// %s - Auto-generated from %s%s
//------------------------------------------------------------------------------

// HASH: %s
%s'''

CMAKE_TEMPLATE = '''\
#-------------------------------------------------------------------------------
# codegen.cmake - Autogenerated cmake containing generated classes<<license>>
#-------------------------------------------------------------------------------

SET (scripts_dir <<scripts_dir>>)

SET (scripts_codegen_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/registration.cpp
<<files>>
)

IDE_SOURCE_PROPERTIES( "/" "${CMAKE_CURRENT_BINARY_DIR}/registration.cpp" )
<<ide_source_props>>
'''

REGISTRATION_TEMPLATE = '''\
//------------------------------------------------------------------------------
// registration.cpp - Autogenerated from script Entities and Components<<license>>
//------------------------------------------------------------------------------

#include "engine/Registry.h"

namespace gaen
{

%s

void register_all_entities_and_components(Registry & registry)
{
    %s
}

} // namespace gaen
'''

def cmake_scripts_dir():
    return dirs.CMP_SCRIPTS_DIR_P

def modification_time(filename):
    t = os.path.getmtime(filename)
    return datetime.datetime.fromtimestamp(t)

def check_file(filename):
    if os.path.exists(filename):
        return True, modification_time(filename)
    else:
        return False, None

def read_cpp_file(filename):
    f = open(filename, 'r')
    d = f.read()
    f.close()

    hashloc = d.find('// HASH: ') + len('// HASH: ')

    hashval = d[hashloc:hashloc+32]
    source = d[hashloc+33:]

    return d, source, hashval

def read_file(filename):
    f = open(filename, 'r')
    d = f.read()
    f.close()
    return d

def write_file(filename, data):
    dirname = posixpath.split(filename)[0]
    if not os.path.exists(dirname):
        os.makedirs(dirname)

    f = open(filename, 'wb')
    f.write(data)
    f.close()

def bindir():
    return sys.argv[1].replace('\\', '/')

class ScriptInfo(object):
    def __init__(self, cmpFullPath, includes):
        self.cmpFullPath = cmpFullPath
        cppPath = cmpFullPath[cmpFullPath.index('/src/scripts/cmp')+len('/src/scripts'):].replace('/cmp/', '/cpp/').replace('.cmp', '.cpp')
        self.cppFullPath = bindir() + cppPath
        self.hFullPath = self.cppFullPath.replace('.cpp', '.h')
        self.cmpFilename = posixpath.split(self.cmpFullPath)[1]
        self.cppFilename = posixpath.split(self.cppFullPath)[1]
        self.hFilename = posixpath.split(self.hFullPath)[1]

        self.cppExists, self.cppModTime = check_file(self.cppFullPath)
        self.cmpExists, self.cmpModTime = check_file(self.cmpFullPath)
        self.hExists, self.hModTime = check_file(self.hFullPath)

        if (self.cppExists):
            self.cppOutputOld, self.cppSourceOld, self.cppSourceOldHash = read_cpp_file(self.cppFullPath)
            self.cppSourceOldHashActual = md5.new(self.cppSourceOld).hexdigest()

        if (self.hExists):
            self.hOutputOld, self.hSourceOld, self.hSourceOldHash = read_cpp_file(self.hFullPath)
            self.hSourceOldHashActual = md5.new(self.hSourceOld).hexdigest()

        self._compile(includes)

        self.cppOutput = TEMPLATE % (self.cppFilename, self.cmpFilename, license_text('//', self.cppFullPath), self.cppSourceHash, self.cppSource)

        if (self.hSource is not None):
            self.hOutput = TEMPLATE % (self.hFilename, self.cmpFilename, license_text('//', self.hFullPath), self.hSourceHash, self.hSource)


    def _compile(self, includes):
        args = [CMPC]
        for inc in includes:
            args += ['-i', inc]
        args.append(self.cmpFullPath)

        # LORRNOTE: Printing these always isn't useful, but keeping
        # these lines commented out for future debugging purposes.
        #for arg in args:
        #    print arg,
        #print

        p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        sout, serr = p.communicate()
        if p.returncode == 0:
            output = sout.replace('\r\n', '\n')
            m = re.match(r'^/// \.H SECTION\n(.*)./// \.CPP SECTION\n.*$', output, flags=re.MULTILINE|re.DOTALL)
            if (m):
                self.hSource = m.group(1)
                self.hSourceHash = md5.new(self.hSource).hexdigest();
            else:
                self.hSource = None
            self.cppSource = re.match(r'^.*/// \.CPP SECTION\n(.*)$', output, flags=re.MULTILINE|re.DOTALL).group(1)
            self.cppSourceHash = md5.new(self.cppSource).hexdigest();
            return True
        else:
            print serr,
            return False

    def _should_write_cpp(self):
        if not self.cppExists:
            return True
        if self.cppOutput == self.cppOutputOld:
            return False
        return True

    def _should_write_h(self):
        if not self.hExists:
            return True
        if self.hOutput == self.hOutputOld:
            return False
        return True


    def write_cpp(self):
        if (self.cppExists and
            self.cppSourceOldHash != self.cppSourceOldHashActual):
            # file already exists and has been modified
            write_file(self.cppFullPath + ".codegen", self.cppOutput)
            print "WARNING: %s has been modified and is not being replaced, diff with %s.codegen and manually apply the changes." % (self.cppFullPath, self.cppFullPath)
        elif self._should_write_cpp():
            print self.cppFilename
            write_file(self.cppFullPath, self.cppOutput)

        if (self.hSource is not None):
            if (self.hExists and
                self.hSourceOldHash != self.hSourceOldHashActual):
                # file already exists and has been modified
                write_file(self.hFullPath + ".codegen", self.hOutput)
                print "WARNING: %s has been modified and is not being replaced, diff with %s.codegen and manually apply the changes." % (self.hFullPath, self.hFullPath)
            elif self._should_write_h():
                print self.hFilename
                write_file(self.hFullPath, self.hOutput)



def find_cmpc():
    for root, _, files in os.walk(dirs.BUILD_DIR_P):
        for f in files:
            if root.endswith('packaged') and f in ['cmpc.exe', 'cmpc']:
                return posixpath.join(root.replace('\\', '/'), f)
    return None

CMPC = find_cmpc()

def license_text(comment_chars, file_path):
    lic_path = posixpath.join(dirs.CMP_SCRIPTS_DIR_P, 'license.txt')
    if not os.path.exists(lic_path):
        return ''
    else:
        lines = [(comment_chars + ' ' + line).rstrip() for line in open(lic_path)]
        return '\n' + comment_chars + '\n' + '\n'.join(lines)


def registration_cpp_path():
    return posixpath.join(sys.argv[1], "registration.cpp")

def write_registration_cpp(script_infos):
    registration_lines = []
    registration_call_lines = []
    for si in script_infos:
        for line in si.cppOutput.splitlines():
            m = re.search(r'void ((register_(entity|component)_([^\s]+))\([^\)]*\))', line)
            if m:
                registration_lines.append(m.group(1) + ';')
                registration_call_lines.append(m.group(2) + '(registry);')

    reg_cpp_path = registration_cpp_path()
    extern_cpp = '\n'.join(['extern void ' + i for i in sorted(registration_lines)])
    calls_cpp = '\n    '.join(sorted(registration_call_lines))
    reg_cpp = REGISTRATION_TEMPLATE % (extern_cpp, calls_cpp)
    reg_cpp = reg_cpp.replace('<<license>>', license_text('//', reg_cpp_path))
    if not os.path.exists(reg_cpp_path) or read_file(reg_cpp_path) != reg_cpp:
        print posixpath.split(reg_cpp_path)[-1]
        write_file(reg_cpp_path, reg_cpp)


def cmakeify_bin_script_path(p):
    if bindir() in p:
        return p.replace(bindir(), '  ${CMAKE_CURRENT_BINARY_DIR}')
    return p

def cmakeify_src_script_path(p):
    if dirs.CMP_SCRIPTS_DIR_P in p:
        return p.replace(dirs.CMP_SCRIPTS_DIR_P, '  ${scripts_dir}')
    return p

def strip_scripts_dir(p):
    dirpath = posixpath.split(p.lstrip())[0]
    dirpath = dirpath.replace('${scripts_dir}', '')
    dirpath = dirpath.replace('${CMAKE_CURRENT_BINARY_DIR}', '')
    return dirpath

def write_cmake(cmp_files, cpp_files, h_files):
    cmp_rel_files = [cmakeify_src_script_path(f) for f in cmp_files]
    cpp_rel_files = [cmakeify_bin_script_path(f) for f in cpp_files]
    h_rel_files = [cmakeify_bin_script_path(f) for f in h_files]
    ide_src_props = ['IDE_SOURCE_PROPERTIES( "%s" "%s" )' % (strip_scripts_dir(r), r.lstrip()) for r in cmp_rel_files]
    ide_src_props += ['IDE_SOURCE_PROPERTIES( "%s" "%s" )' % (strip_scripts_dir(r), r.lstrip()) for r in cpp_rel_files]
    ide_src_props += ['IDE_SOURCE_PROPERTIES( "%s" "%s" )' % (strip_scripts_dir(r), r.lstrip()) for r in h_rel_files]
    cmake_path = posixpath.join(sys.argv[1], 'codegen.cmake')
    template = CMAKE_TEMPLATE
    template = template.replace('<<scripts_dir>>', cmake_scripts_dir())
    template = template.replace('<<license>>', license_text('#', cmake_path))
    template = template.replace('<<files>>', '\n'.join(cmp_rel_files + cpp_rel_files + h_rel_files))
    template = template.replace('<<ide_source_props>>', '\n'.join(ide_src_props))
    if not os.path.exists(cmake_path) or read_file(cmake_path) != template:
        print posixpath.split(cmake_path)[-1]
        write_file(cmake_path, template)
        # touch the scripts/CMakeLists.txt file since we generated
        # codegen.cmake and want to poke cmake to reprocess
        os.utime(posixpath.join(dirs.GAEN_SCRIPTS_DIR, 'CMakeLists.txt'), None)


def find_source_files(scripts_dir, source_files):
    for root, _, files in os.walk(scripts_dir):
        for f in files:
            if f.endswith('cmp'):
                source_files.append(posixpath.join(root.replace('\\', '/'), f))

def main():
    # run codegen_api first to get include list from most
    # recent api code
    new_data, includes = codegen_api.build_metadata()

    if CMPC is None:
        print "ERROR: cmpc not found, do you need to build?"
        exit(1)
    script_infos = []
    cmp_files = []
    cpp_files = []
    h_files = []
    has_errors = False;

    source_files = []
    find_source_files(dirs.CMP_SCRIPTS_DIR_P, source_files)

    for f in source_files:
        try:
            if f.endswith('.cmp'):
                si = ScriptInfo(f, includes)
                si.write_cpp()
                script_infos.append(si)
                cmp_files.append(si.cmpFullPath)
                cpp_files.append(si.cppFullPath)
                if (si.hSource is not None):
                    h_files.append(si.hFullPath)
        except:
            print "ERROR: %s failed to compile" % f
            raise
            has_errors = True;

    if not has_errors:
        write_registration_cpp(script_infos)
        write_cmake(cmp_files, cpp_files, h_files)


if __name__=='__main__':
    main()

