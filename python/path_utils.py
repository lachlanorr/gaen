#!/usr/bin/env python

#-------------------------------------------------------------------------------
# dirs.py - Shared utils to find various dirs within gaen
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
import pathlib

class Paths:
    def __init__(self, binary_dir):
        self.binary_dir = pathlib.Path(binary_dir)

        self.python_dir = pathlib.Path(os.path.abspath(__file__)).parent
        self.gaen_dir = self.python_dir.parent
        self.gaen_src_dir = self.gaen_dir/'src'
        self.gaen_shaders_dir = self.gaen_src_dir/'gaen'/'renderergl'/'shaders'

        project_info_file = self.gaen_dir.parent/'gaen_project.info'
        if project_info_file.is_file():
            self.is_project = True
            self.binary_gaen_dir = self.binary_dir/'gaen'
            self.project_name = project_info_file.read_text().strip()
            self.project_dir = self.gaen_dir.parent
            self.project_src_dir = self.project_dir/'src'
            self.project_shaders_dir = self.project_src_dir/'shaders'
            self.project_messages_def_yaml = self.project_src_dir/(self.project_name+'_lib')/'messages_def.yaml'

        else:
            self.is_project = False
            self.binary_gaen_dir = self.binary_dir

        self.scripts_output_dir = self.binary_gaen_dir/'src'/'gaen'/'scripts'

        self.hashes_cpp_tpl = self.python_dir/'templates'/'hashes.cpp.tpl'
        self.hashes_h_tpl = self.python_dir/'templates'/'hashes.h.tpl'
        self.hashes_output_dir = self.binary_gaen_dir/'src'/'gaen'/'hashes'
        self.hashes_cpp = self.hashes_output_dir/'hashes3.cpp'
        self.hashes_h = self.hashes_output_dir/'hashes3.h'

        self.compiler_cpp = self.gaen_src_dir/'gaen'/'compose'/'compiler.cpp'
        self.system_api_meta_cpp = self.binary_gaen_dir/'src'/'gaen'/'compose'/'system_api_meta3.cpp'
        self.system_api_meta_cpp_tpl = self.python_dir/'templates'/'system_api_meta.cpp.tpl'

        self.engine_cmakelists_txt = self.gaen_src_dir/'gaen'/'engine'/'CMakeLists.txt'
        self.messages_output_dir = self.binary_gaen_dir/'src'/'gaen'/'engine'/'messages3'
        self.messages_cmake = self.messages_output_dir/'messages.cmake'
        self.message_cpp_tpl = self.python_dir/'templates'/'message.cpp.tpl'
        self.filelist_cmake_tpl = self.python_dir/'templates'/'filelist.cmake.tpl'
        self.messages_def_yaml = self.gaen_src_dir/'gaen'/'engine'/'messages_def.yaml'
        self.field_types_yaml = self.gaen_src_dir/'gaen'/'engine'/'field_types.yaml'

def read_file(path):
    if path.is_file():
        return path.read_bytes()
    return None

def write_file_if_different(path, new_data):
    curr_data = read_file(path)
    if curr_data != new_data:
        print("Writing " + str(path))
        path.write_bytes(new_data)
        return True
    return False


#SCRIPT_FILE       = pathlib.Path(os.path.abspath(__file__))
#SCRIPT_DIR        = pathlib.Path(os.path.abspath(__file__))[0]
#TEMPLATE_DIR      = os.path.join(SCRIPT_DIR, 'templates', 'project')
#GAEN_DIR          = os.path.split(SCRIPT_DIR)[0]
#GAEN_PARENT_DIR   = os.path.split(GAEN_DIR)[0]
#GAEN_SRC_DIR      = os.path.join(GAEN_DIR, 'src')
#COMPILER_CPP_CPP_FILE = os.path.join(GAEN_SRC_DIR, 'gaen/compose', 'compiler.cpp')
#PROJECT_INFO_FILE = os.path.join(GAEN_PARENT_DIR, 'gaen_project.info')
#IS_PROJECT        = __is_project()
#PROJECT_DIR       = __project_dir()
#PROJECT_SRC_DIR   = os.path.join(PROJECT_DIR, 'src')
#BUILD_DIR         = os.path.join(PROJECT_DIR, 'build')
#CMP_SCRIPTS_DIR   = os.path.join(PROJECT_DIR, 'src', 'scripts')
#GAEN_SCRIPTS_DIR  = os.path.join(GAEN_SRC_DIR, 'gaen/scripts')
#PROJECT_NAME      = __project_name()
