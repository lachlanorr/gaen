#!/usr/bin/env python

#-------------------------------------------------------------------------------
# start_project.py - Start a new project based on gaen engine
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

import sys
import os

def script_dir():
    return os.path.split(os.path.abspath(__file__))[0]

def template_dir():
    return os.path.join(script_dir(), 'templates', 'project')

def gaen_dir():
    return os.path.split(script_dir())[0]
    
def project_dir():
    return os.path.split(gaen_dir())[0]

def project_name():
    return os.path.split(project_dir())[1]

REPLACEMENTS = { 'PROJECT_NAME': project_name(),
                 'PROJECT_NAME_UPPER': project_name().upper()
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
    for root, dirs, files in os.walk(template_dir()):
        for f in files:
            in_f = os.path.join(root, f)
            out_f = in_f.replace(template_dir(), project_dir())
            fill_template(in_f, out_f)

def err_and_exit(msg, code):
    print(msg)
    sys.exit(code)

def check_dir():
    if os.path.abspath(os.path.curdir) != project_dir():
        err_and_exit("start_project.py must be run from the parent directory of a checked out gaen repository", 2)
    if os.listdir(project_dir()) != ['gaen']:
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
    
