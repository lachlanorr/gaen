#!/usr/bin/env python

#-------------------------------------------------------------------------------
# start_project.py - Start a new project based on gaen engine
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

import sys
import os
import subprocess
import re
import shutil

import dirs

def get_git_remote():
    wd = os.getcwd()
    os.chdir(dirs.SCRIPT_DIR)
    o = subprocess.check_output(['git', 'remote', '-vv'])
    os.chdir(wd)
    o = o.splitlines()[0]
    o = o.split()[1]
    return re.sub('/[^@]+@', '//', o)

def get_git_branch():
    wd = os.getcwd()
    os.chdir(dirs.SCRIPT_DIR)
    o = subprocess.check_output(['git', 'status'])
    os.chdir(wd)
    o = o.splitlines()[0]
    if not o.startswith('On branch '):
        print 'Unexpected output from git status:', o
    o = o.split()[-1]
    return o

def get_git_commit():
    wd = os.getcwd()
    os.chdir(dirs.SCRIPT_DIR)
    o = subprocess.check_output(['git', 'rev-parse', 'HEAD'])
    os.chdir(wd)
    o = o.splitlines()[0]
    return o

def project_name():
    if len(sys.argv) > 1:
        return sys.argv[1]
    else:
        # use directory name
        return os.path.split(dirs.GAEN_PARENT_DIR)[1]

REPLACEMENTS = { 'PROJECT_NAME': project_name(),
                 'PROJECT_NAME_UPPER': project_name().upper(),
                 'GAEN_GIT_REMOTE': get_git_remote(),
                 'GAEN_GIT_BRANCH': get_git_branch(),
                 'GAEN_GIT_COMMIT': get_git_commit()
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
    for root, _, files in os.walk(dirs.TEMPLATE_DIR):
        for f in files:
            in_f = os.path.join(root, f)
            out_f = in_f.replace(dirs.TEMPLATE_DIR, dirs.GAEN_PARENT_DIR)
            out_f = out_f.replace("PROJECT_NAME", project_name())
            fill_template(in_f, out_f)

def copy_gaen_scripts():
    gaen_cmp_subdir = os.path.join('src', 'scripts', 'cmp', 'gaen')
    gaen_cmp_dir = os.path.join(dirs.GAEN_DIR, gaen_cmp_subdir)
    for root, _, files in os.walk(gaen_cmp_dir):
        for f in files:
            in_f = os.path.join(root, f)
            out_f = in_f.replace(gaen_cmp_dir, os.path.join(dirs.GAEN_PARENT_DIR, gaen_cmp_subdir))
            fill_template(in_f, out_f)

def copy_gaen_assets():
    gaen_assets_subdir = os.path.join('assets', 'raw', 'gaen')
    gaen_assets_dir = os.path.join(dirs.GAEN_DIR, gaen_assets_subdir)
    project_assets_dir = os.path.join(dirs.GAEN_PARENT_DIR, gaen_assets_subdir)
    print gaen_assets_dir
    print project_assets_dir
    shutil.copytree(gaen_assets_dir, project_assets_dir)

def err_and_exit(msg, code):
    print(msg)
    sys.exit(code)

def check_dir():
    if os.path.abspath(os.path.curdir) != dirs.GAEN_PARENT_DIR:
        err_and_exit("start_project.py must be run from the parent directory of a checked out gaen repository", 2)
    if os.listdir(dirs.GAEN_PARENT_DIR) != ['gaen']:
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
        copy_gaen_scripts()
        copy_gaen_assets()
        print
        print "Gaen project '%s' created in current directory." % project_name()
        print bootstrap_msg()
    
if __name__=='__main__':
    main()
    
