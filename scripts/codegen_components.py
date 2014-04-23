#!/usr/bin/env python

#-------------------------------------------------------------------------------
# make_component_classes.py - Generate c++ component classes from defs
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

import posixpath

from codegen_utils import *

class Templates:
    ComponentClass = read_file_data(posixpath.join(scripts_path(), 'templates/component_template.cpp'))
    CmakeFileList  = read_file_data(posixpath.join(scripts_path(), 'templates/filelist_template.cmake'))

def components_def_path():
    return posixpath.join(gaen_path(), 'src/engine/components')

def template_subst(template, replacements):
    for k, v in replacements.iteritems():
        template = template.replace('<<%s>>' % k, v)
    return template

def gen_component_cmake(field_handlers):
    lines = []
    for field_handler in field_handlers:
        lines.append('  components/%sComponents.h' % field_handler.object_name)
    lines.sort()
    return template_subst(Templates.CmakeFileList, {'files'       : '\n'.join(lines),
                                                    'autogen_type': 'components'})

def parse_components_def():
    context = globals()
    context['FIELD_HANDLERS'] = []
    with open(posixpath.join(scripts_path(), 'field_handler.py')) as f:
        exec f in context
    def_path = posixpath.join(components_def_path(), 'components.def')
    with open(def_path) as f:
        exec f in context
    return context['FIELD_HANDLERS']

def gen_component_classes():
    field_handlers = parse_components_def()
#    for field_handler in field_handlers:
#        cpp_data = gen_component_class(field_handler)
#        cpp_path = posixpath.join(components_def_path(), field_handler.object_name + 'Component.h')
#        replace_file_if_different(cpp_path, cpp_data)
    cmake_data = gen_component_cmake(field_handlers)
    cmake_path = posixpath.join(components_def_path(), 'components.cmake')
    replace_file_if_different(cmake_path, cmake_data)
        

if __name__ == '__main__':
    print 'Remove this exit'
    exit(0)
    gen_component_classes()

