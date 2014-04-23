#!/usr/bin/env python

#-------------------------------------------------------------------------------
# codegen_utils.py - Common utilities used in code generation
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
import re

def read_file_data(path):
    if not os.path.exists(path):
        return None
    with open(path, 'rb') as f:
        return f.read()

def replace_file_if_different(path, data):
    if read_file_data(path) != data:
        print 'Writing ' + path
        with open(path, 'wb') as out_f:
            out_f.write(data)

def scripts_path():
    scriptdir = os.path.split(os.path.abspath(__file__))[0].replace('\\', '/')
    return scriptdir

def gaen_path():
    return posixpath.split(scripts_path())[0]

def template_subst(template, replacements):
    for k, v in replacements.iteritems():
        template = template.replace('<<%s>>' % k, v)
    return template

def gen_includes(includes):
    inc = []
    for i in includes:
        inc.append('#include "%s"\n' % i)
    return ''.join(inc)

def camel_to_underscores(s):
    s = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', s)
    s = re.sub(r'([a-z\d])([A-Z])', r'\1_\2', s)
    s = s.strip('_')
    return s.lower()
    
