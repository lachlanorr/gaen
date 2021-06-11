#!/usr/bin/env python

#-------------------------------------------------------------------------------
# bad_copyrights.py - Check for copyright headers in source files
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

'''
Prints out source file paths that are missing standard gaen
copyright/license message.
'''

import os
import posixpath
import re
import datetime as dt

SCRIPT_HEADER = (r"#-------------------------------------------------------------------------------\n"
                 r"# %s - .+[^\.]\n"
                 r"#\n"
                 r"# Gaen Concurrency Engine - http://gaen\.org\n"
                 r"# Copyright \(c\) 2014-%d Lachlan Orr\n"
                 r"#\n"
                 r"# This software is provided 'as-is', without any express or implied\n"
                 r"# warranty\. In no event will the authors be held liable for any damages\n"
                 r"# arising from the use of this software\.\n"
                 r"#\n"
                 r"# Permission is granted to anyone to use this software for any purpose,\n"
                 r"# including commercial applications, and to alter it and redistribute it\n"
                 r"# freely, subject to the following restrictions:\n"
                 r"#\n"
                 r"#   1\. The origin of this software must not be misrepresented; you must not\n"
                 r"#   claim that you wrote the original software\. If you use this software\n"
                 r"#   in a product, an acknowledgment in the product documentation would be\n"
                 r"#   appreciated but is not required\.\n"
                 r"#\n"
                 r"#   2\. Altered source versions must be plainly marked as such, and must not be\n"
                 r"#   misrepresented as being the original software\.\n"
                 r"#\n"
                 r"#   3\. This notice may not be removed or altered from any source\n"
                 r"#   distribution\.\n"
                 r"#-------------------------------------------------------------------------------\n")


C_HEADER = (r"/\*------------------------------------------------------------------------------\n"
            r"%s - .+[^\.]\n"
            r"\n"
            r"Gaen Concurrency Engine - http://gaen\.org\n"
            r"Copyright \(c\) 2014-%d Lachlan Orr\n"
            r"\n"
            r"This software is provided 'as-is', without any express or implied\n"
            r"warranty\. In no event will the authors be held liable for any damages\n"
            r"arising from the use of this software\.\n"
            r"\n"
            r"Permission is granted to anyone to use this software for any purpose,\n"
            r"including commercial applications, and to alter it and redistribute it\n"
            r"freely, subject to the following restrictions:\n"
            r"\n"
            r"  1\. The origin of this software must not be misrepresented; you must not\n"
            r"  claim that you wrote the original software\. If you use this software\n"
            r"  in a product, an acknowledgment in the product documentation would be\n"
            r"  appreciated but is not required\.\n"
            r"\n"
            r"  2\. Altered source versions must be plainly marked as such, and must not be\n"
            r"  misrepresented as being the original software\.\n"
            r"\n"
            r"  3\. This notice may not be removed or altered from any source\n"
            r"  distribution\.\n"
            r"------------------------------------------------------------------------------\*/\n")


CPP_HEADER = (r"//------------------------------------------------------------------------------\n"
              r"// %s - .+[^\.]\n"
              r"//\n"
              r"// Gaen Concurrency Engine - http://gaen\.org\n"
              r"// Copyright \(c\) 2014-%d Lachlan Orr\n"
              r"//\n"
              r"// This software is provided 'as-is', without any express or implied\n"
              r"// warranty\. In no event will the authors be held liable for any damages\n"
              r"// arising from the use of this software\.\n"
              r"//\n"
              r"// Permission is granted to anyone to use this software for any purpose,\n"
              r"// including commercial applications, and to alter it and redistribute it\n"
              r"// freely, subject to the following restrictions:\n"
              r"//\n"
              r"//   1\. The origin of this software must not be misrepresented; you must not\n"
              r"//   claim that you wrote the original software\. If you use this software\n"
              r"//   in a product, an acknowledgment in the product documentation would be\n"
              r"//   appreciated but is not required\.\n"
              r"//\n"
              r"//   2\. Altered source versions must be plainly marked as such, and must not be\n"
              r"//   misrepresented as being the original software\.\n"
              r"//\n"
              r"//   3\. This notice may not be removed or altered from any source\n"
              r"//   distribution\.\n"
              r"//------------------------------------------------------------------------------\n")


def current_year():
    return dt.datetime.now().year

HEADERS = [(re.compile(r'^.*\.(h|c|cpp|cmp|m|mm)$'),       CPP_HEADER),
           (re.compile(r'^.*\.(y|l)$'),                    C_HEADER),
           (re.compile(r'^.*\.py$'),                       SCRIPT_HEADER),
           (re.compile(r'^.*(CMakeLists.txt|\.cmake)$'),   SCRIPT_HEADER),
          ]

EXCLUDE_DIR_RE = re.compile(r'^.*(/external/|/build/|/python/templates/|/compose/compose_(parser|scanner)\.).*$')

def expected_header(path):
    fname = posixpath.split(path)[1]
    for pattern, header in HEADERS:
        if re.match(pattern, path):
            return header % (fname, current_year())
    return None

def verify_header(path, exphdr):
    with open(path, 'r') as f:
        data = f.read()
        return re.search(exphdr, data, re.MULTILINE)

def checkfile(path):
    exphdr = expected_header(path)
    if (exphdr is not None):
        return verify_header(path, exphdr)
    return True

def checkdir(path):
    for root, dirs, files in os.walk(path):
        for f in files:
            fullpath = posixpath.join(root.replace('\\', '/'), f)
            if not re.match(EXCLUDE_DIR_RE, fullpath):
                if not checkfile(fullpath):
                    print fullpath

def checkdirs():
    scriptdir = os.path.split(os.path.abspath(__file__))[0].replace('\\', '/')
    gaendir = posixpath.split(scriptdir)[0]
    checkdir(gaendir)

if __name__=='__main__':
    checkdirs()

