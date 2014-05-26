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
    
    return source, hashval

class ScriptInfo(object):
    def __init__(self, cmpFullPath):
        self.cmpFullPath = cmpFullPath
        self.cppFullPath = cmpFullPath.replace('/cmp/', '/cpp/').replace('.cmp', '.cpp')
        self.cmpFilename = posixpath.split(self.cmpFullPath)[1]
        self.cppFilename = posixpath.split(self.cppFullPath)[1]
        
        self.cppExists, self.cppModTime = check_file(self.cppFullPath)
        self.cmpExists, self.cmpModTime = check_file(self.cmpFullPath)

        if (self.cppExists):
            self.cppSourceOld, self.cppSourceHashOld = read_cpp_file(self.cppFullPath)

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

    def write_cpp(self):
        dirname = posixpath.split(self.cppFullPath)[0]
        if not os.path.exists(dirname):
            os.makedirs(dirname)

        
        f = open(self.cppFullPath, 'wb')
        f.write(self.cppOutput)
        f.close()

def find_cmpc():
    for root, dirs, files in os.walk(gaen_dir()):
        for f in files:
            if root.endswith('packaged') and f in ['cmpc.exe', 'cmpc']:
                return posixpath.join(root.replace('\\', '/'), f)
    return None

CMPC = find_cmpc()

if __name__=='__main__':
    if CMPC is None:
        print "cmpc not found, do you need to build?"
        exit(1)

#def gencpp(path):
    
