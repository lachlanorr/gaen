#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen.py - Run cmpc on all compose scripts to generate cpp files
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014 Lachlan Orr
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
import posixpath
import StringIO
import subprocess
import md5
import datetime

TEMPLATE = '''\
//------------------------------------------------------------------------------
// %s - Auto-generated from %s
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

// HASH: %s
%s'''

CMAKE_TEMPLATE = '''\
#-------------------------------------------------------------------------------
# codegen.cmake - Autogenerated cmake containing generated classes
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014 Lachlan Orr
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

SET (scripts_dir <<scripts_dir>>)

SET (scripts_codegen_SOURCES
<<files>>
)

<<ide_source_props>>
'''

def gaen_dir():
    scriptdir = os.path.split(os.path.abspath(__file__))[0].replace('\\', '/')
    return posixpath.split(scriptdir)[0]

def default_scripts_dir():
    return posixpath.join(gaen_dir(), 'src/scripts')

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
    

class ScriptInfo(object):
    def __init__(self, cmpFullPath):
        self.cmpFullPath = cmpFullPath
        self.cppFullPath = cmpFullPath.replace('/cmp/', '/cpp/').replace('.cmp', '.cpp')
        self.cmpFilename = posixpath.split(self.cmpFullPath)[1]
        self.cppFilename = posixpath.split(self.cppFullPath)[1]
        
        self.cppExists, self.cppModTime = check_file(self.cppFullPath)
        self.cmpExists, self.cmpModTime = check_file(self.cmpFullPath)

        if (self.cppExists):
            self.cppOutputOld, self.cppSourceOld, self.cppSourceOldHash = read_cpp_file(self.cppFullPath)
            self.cppSourceOldHashActual = md5.new(self.cppSourceOld).hexdigest()

        self._compile()

        self.cppOutput = TEMPLATE % (self.cppFilename, self.cmpFilename, self.cppSourceHash, self.cppSource)

    def _compile(self):
        p = subprocess.Popen([CMPC, self.cmpFullPath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait()
        sout, serr = p.communicate()

        if p.returncode == 0:
            self.cppSource = sout.replace('\r\n', '\n')
            self.cppSourceHash = md5.new(self.cppSource).hexdigest();
            return True
        else:
            return False

    def _should_write_cpp(self):
        if not self.cppExists:
            return True
        if self.cppOutput == self.cppOutputOld:
            return False
        return True
            

    def write_cpp(self):
        if (self.cppExists and
            self.cppSourceOldHash != self.cppSourceOldHashActual):
            # file already exists and has been modified
            write_file(self.cppFullPath + ".codegen", self.cppOutput)
            print "WARNING: %s has been modified and is not being replaced, diff with %s.codegen and manually apply the changes." % (self.cppFullPath, self.cppFullPath)
        elif self._should_write_cpp():
            print "Writing %s" % self.cppFullPath
            write_file(self.cppFullPath, self.cppOutput)
            
        

def find_cmpc():
    for root, dirs, files in os.walk(gaen_dir()):
        for f in files:
            if root.endswith('packaged') and f in ['cmpc.exe', 'cmpc']:
                return posixpath.join(root.replace('\\', '/'), f)
    return None

CMPC = find_cmpc()
SCRIPTS_DIR = default_scripts_dir()

def write_cmake(cmp_files, cpp_files):
    cmp_rel_files = [f.replace(SCRIPTS_DIR, '  ${scripts_dir}') for f in cmp_files]
    cpp_rel_files = [f.replace(SCRIPTS_DIR, '  ${scripts_dir}') for f in cpp_files]
    ide_src_props = ['IDE_SOURCE_PROPERTIES( "%s" "%s" )' % (posixpath.split(r.lstrip())[0].replace('${scripts_dir}/cmp', '/cmp'), r.lstrip()) for r in cmp_rel_files]
    ide_src_props += ['IDE_SOURCE_PROPERTIES( "%s" "%s" )' % (posixpath.split(r.lstrip())[0].replace('${scripts_dir}/cpp', '/cpp'), r.lstrip()) for r in cpp_rel_files]
    template = CMAKE_TEMPLATE
    template = template.replace('<<scripts_dir>>', SCRIPTS_DIR)
    template = template.replace('<<files>>', '\n'.join(cmp_rel_files + cpp_rel_files))
    template = template.replace('<<ide_source_props>>', '\n'.join(ide_src_props))
    cmake_path = posixpath.join(gaen_dir(), 'src/scripts/codegen.cmake')
    if not os.path.exists(cmake_path) or read_file(cmake_path) != template:
        print "Writing %s" % cmake_path
        write_file(cmake_path, template)

def main():
    if CMPC is None:
        print "ERROR: cmpc not found, do you need to build?"
        exit(1)
    cmp_files = []
    cpp_files = []
    for root, dirs, files in os.walk(SCRIPTS_DIR):
        for f in files:
            if f.endswith('.cmp'):
                si = ScriptInfo(posixpath.join(root.replace('\\', '/'), f))
                si.write_cpp()
                cmp_files.append(si.cmpFullPath)
                cpp_files.append(si.cppFullPath)
    write_cmake(cmp_files, cpp_files)
    return None
    

if __name__=='__main__':
    main()

