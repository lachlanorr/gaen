#!/usr/bin/env python

#-------------------------------------------------------------------------------
# dirs.py - Shared utils to find various dirs within gaen
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

def __is_project():
    if '-n' in sys.argv or '--noproject' in sys.argv:
        return False
    else:
        return os.path.exists(PROJECT_INFO_FILE)

def __project_dir():
    if IS_PROJECT:
        return GAEN_PARENT_DIR
    else:
        return GAEN_DIR

def __project_name():
    if IS_PROJECT:
        with open(PROJECT_INFO_FILE) as in_f:
            d = in_f.read().strip()
            return d.splitlines()[0] # return first non blank line
    else:
        return 'gaen'

SCRIPT_DIR        = os.path.split(os.path.abspath(__file__))[0]
TEMPLATE_DIR      = os.path.join(SCRIPT_DIR, 'templates', 'project')
GAEN_DIR          = os.path.split(SCRIPT_DIR)[0]
GAEN_PARENT_DIR   = os.path.split(GAEN_DIR)[0]
GAEN_SRC_DIR      = os.path.join(GAEN_DIR, 'src')
COMPILER_CPP_CPP_FILE = os.path.join(GAEN_SRC_DIR, 'gaen/compose', 'compiler.cpp')
PROJECT_INFO_FILE = os.path.join(GAEN_PARENT_DIR, 'gaen_project.info')
IS_PROJECT        = __is_project()
PROJECT_DIR       = __project_dir()
PROJECT_SRC_DIR   = os.path.join(PROJECT_DIR, 'src')
BUILD_DIR         = os.path.join(PROJECT_DIR, 'build')
CMP_SCRIPTS_DIR   = os.path.join(PROJECT_DIR, 'src', 'scripts')
GAEN_SCRIPTS_DIR  = os.path.join(GAEN_SRC_DIR, 'gaen/scripts')
PROJECT_NAME      = __project_name()

# create posix versions of all paths
for a in dir(sys.modules[__name__]):
    if a.endswith('_DIR') or a.endswith('_FILE'):
        v = getattr(sys.modules[__name__], a)
        setattr(sys.modules[__name__], a + '_P', v.replace('\\', '/'))
