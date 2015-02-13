#!/usr/bin/env python

#-------------------------------------------------------------------------------
# start_project.py - Start a new project based on gaen engine
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

import sys
import os
import subprocess
import re

import dirs

def get_git_remote():
    wd = os.getcwd()
    os.chdir(dirs.SCRIPT_DIR)
    o = subprocess.check_output(['git', 'remote', '-vv'])
    os.chdir(wd)
    o = o.splitlines()[0]
    o = o.split()[1]
    return re.sub('/[^@]+@', '//', o)

def project_name():
    if len(sys.argv) > 1:
        return sys.argv[1]
    else:
        # use directory name
        return os.path.split(dirs.GAEN_DIR)[0]

REPLACEMENTS = { 'PROJECT_NAME': project_name(),
                 'PROJECT_NAME_UPPER': project_name().upper(),
                 'GAEN_GIT_REMOTE': get_git_remote()
}

def write_file(out_f, data):
    dirname = os.path.split(out_f)[0]
    if not os.path.exists(dirname):
        os.makedirs(dirname)
        
    f = open(out_f, 'w')
    f.write(data)
    f.close()

def fill_template(in_file, out_file):
    with open(in_file) as in_f:
        content = in_f.read()
    for k, v in REPLACEMENTS.iteritems():
        content = content.replace("<<%s>>" % k, v)
    write_file(out_file, content)

def proc_templates():
    for root, dirs, files in os.walk(dirs.TEMPLATE_DIR):
        for f in files:
            in_f = os.path.join(root, f)
            out_f = in_f.replace(dirs.TEMPLATE_DIR, dirs.PROJECT_DIR)
            out_f = out_f.replace("PROJECT_NAME", project_name())
            fill_template(in_f, out_f)

def err_and_exit(msg, code):
    print(msg)
    sys.exit(code)

def check_dir():
    if os.path.abspath(os.path.curdir) != dirs.PROJECT_DIR:
        err_and_exit("start_project.py must be run from the parent directory of a checked out gaen repository", 2)
    if os.listdir(dirs.PROJECT_DIR) != ['gaen']:
        err_and_exit("start_project.py must be run from a directory containing only a 'gaen' sub directory, and nothing else", 1)

def bootstrap_msg():
    if os.name == 'nt':
        return 'Run bootstrap.bat to generate Visual Studio solution.'
    elif os.name == 'mac':
        return 'Run bootstrap.sh to generate Xcode project.'
    else:
        return 'Run bootstrap.sh to generate make file.'

def main():
    check_dir()

    print "Starting new project '%s' in current directory..." % project_name()
    i = raw_input("Continue? [Y/n] ")

    if i == '' or i.lower() == 'y':
        proc_templates()
        print
        print "Gaen project '%s' created in current directory." % project_name()
        print bootstrap_msg()
    
if __name__=='__main__':
    main()
    
