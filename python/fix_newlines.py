#!/usr/bin/env python

#-------------------------------------------------------------------------------
# fix_newlines.py - Remove \r from code files
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


import os

import dirs

EXTS = ['.cpp',
        '.h',
        '.cc',
        '.c',
        '.l',
        '.y',
        '.py',
        '.txt',
        '.cmake',
        '.org',
        '.bat',
        '.sh',
        '.html',
        '.doxy',
        '.def',
        '.atl',
]

for subdir, direc, files in os.walk(dirs.PROJECT_DIR):
    for file in files:
        p = os.path.join(subdir, file).replace('\\', '/')
        if not p.startswith(dirs.BUILD_DIR_P + '/'):
            if os.path.splitext(p)[1] in EXTS:
                with open(p, 'rb') as f:
                    d = f.read()
                    drep = d.replace("\r", "")
                if drep != d:
                    print "Fixing " + p
                    with open(p, 'wb') as f:
                        f.write(drep)


