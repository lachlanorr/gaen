#!/usr/bin/env python

#-------------------------------------------------------------------------------
# make_win32gl.py - Prep windows function pointers from glcorearb.h
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

'''
Processes glcorearb.h, which is provided by opengl.org, and builds
win32gl.h and win32gl.cpp.  These files form the interface of function
pointers for all opengl funcs in the GameAfar engine.
'''

import re
import sys

# Set of func addresses that are in the standard Windows opengl32.dll
# (i.e. 1.0 and 1.1 funcs).  All others will be searched for in the
# video driver's implementation using wglGetProcAddress.
OPENGL32_FUNCS = set([
    # 1.0
    'glCullFace',
    'glFrontFace',
    'glHint',
    'glLineWidth',
    'glPointSize',
    'glPolygonMode',
    'glScissor',
    'glTexParameterf',
    'glTexParameterfv',
    'glTexParameteri',
    'glTexParameteriv',
    'glTexImage1D',
    'glTexImage2D',
    'glDrawBuffer',
    'glClear',
    'glClearColor',
    'glClearStencil',
    'glClearDepth',
    'glStencilMask',
    'glColorMask',
    'glDepthMask',
    'glDisable',
    'glEnable',
    'glFinish',
    'glFlush',
    'glBlendFunc',
    'glLogicOp',
    'glStencilFunc',
    'glStencilOp',
    'glDepthFunc',
    'glPixelStoref',
    'glPixelStorei',
    'glReadBuffer',
    'glReadPixels',
    'glGetBooleanv',
    'glGetDoublev',
    'glGetError',
    'glGetFloatv',
    'glGetIntegerv',
    'glGetString',
    'glGetTexImage',
    'glGetTexParameterfv',
    'glGetTexParameteriv',
    'glGetTexLevelParameterfv',
    'glGetTexLevelParameteriv',
    'glIsEnabled',
    'glDepthRange',
    'glViewport',
    # 1.1
    'glDrawArrays',
    'glDrawElements',
    'glGetPointerv',
    'glPolygonOffset',
    'glCopyTexImage1D',
    'glCopyTexImage2D',
    'glCopyTexSubImage1D',
    'glCopyTexSubImage2D',
    'glTexSubImage1D',
    'glTexSubImage2D',
    'glBindTexture',
    'glDeleteTextures',
    'glGenTextures',
    'glIsTexture',
    ])

def get_funcs(lines):
    '''
    Look for api names in lines (these are easily identified by
    APIENTRY), and return a list of all func names.
    '''
    funcs = []
    for line in lines:
        m = re.match(r'GLAPI.*APIENTRY[\s]+([\S]+).*', line)
        if m:
            funcs.append(m.groups()[0])
    return funcs

def func_ptr_type(func):
    '''
    Convert an opengl func name to it's corresponding function pointer
    typedef name.  e.g. glCullFace -> PFNGLCULLFACEPROC
    '''
    return 'PFN%sPROC' % func.upper()


def proc_funcs(funcs):
    externs = []
    vars = []
    inits = []
    for func in funcs:
        func_ptr = func_ptr_type(func)
        externs.append('extern %s %s;' % (func_ptr, func))
        vars.append('%s %s;' % (func_ptr, func))

        if func in OPENGL32_FUNCS:
            inits.append('    %s = (%s)GetProcAddress(hOpenGL32, "%s");' % (func, func_ptr, func))
        else:
            inits.append('    %s = (%s)wglGetProcAddress("%s");' % (func, func_ptr, func))
    return externs, vars, inits
        

WIN32GL_H_TEMPLATE = ('//------------------------------------------------------------------------------\n'
                      '// win32gl.h - win32 opengl function pointers\n'
                      '//\n'
                      '// Gaen Concurrency Engine - http://gaen.org\n'
                      '// Copyright (c) 2014-2016 Lachlan Orr\n'
                      '//\n'
                      '// This software is provided \'as-is\', without any express or implied\n'
                      '// warranty. In no event will the authors be held liable for any damages\n'
                      '// arising from the use of this software.\n'
                      '//\n'
                      '// Permission is granted to anyone to use this software for any purpose,\n'
                      '// including commercial applications, and to alter it and redistribute it\n'
                      '// freely, subject to the following restrictions:\n'
                      '//\n'
                      '//   1. The origin of this software must not be misrepresented; you must not\n'
                      '//   claim that you wrote the original software. If you use this software\n'
                      '//   in a product, an acknowledgment in the product documentation would be\n'
                      '//   appreciated but is not required.\n'
                      '//\n'
                      '//   2. Altered source versions must be plainly marked as such, and must not be\n'
                      '//   misrepresented as being the original software.\n'
                      '//\n'
                      '//   3. This notice may not be removed or altered from any source\n'
                      '//   distribution.\n'
                      '//------------------------------------------------------------------------------\n'
                      '\n'
                      '#ifndef GAEN_GLRENDERER_WIN32GL_H\n'
                      '#define GAEN_GLRENDERER_WIN32GL_H\n'
                      '\n'
                      '#include <GL/glcorearb.h>\n'
                      '\n'
                      '// init_win32gl must be called to setup function pointers before any\n'
                      '// opengl functions are called.\n'
                      'void init_win32gl();\n'
                      '\n'
                      '// Externs defined to call OpenGL funcs defined in win32gl.cpp\n'
                      '%s\n'
                      '\n'
                      '#endif // GAEN_GLRENDERER_WIN32GL_H\n'
                  )

WIN32GL_CPP_TEMPLATE = ('//------------------------------------------------------------------------------\n'
                        '// win32gl.h - win32 opengl function pointers\n'
                        '//\n'
                        '// Gaen Concurrency Engine - http://gaen.org\n'
                        '// Copyright (c) 2014-2016 Lachlan Orr\n'
                        '//\n'
                        '// This software is provided \'as-is\', without any express or implied\n'
                        '// warranty. In no event will the authors be held liable for any damages\n'
                        '// arising from the use of this software.\n'
                        '//\n'
                        '// Permission is granted to anyone to use this software for any purpose,\n'
                        '// including commercial applications, and to alter it and redistribute it\n'
                        '// freely, subject to the following restrictions:\n'
                        '//\n'
                        '//   1. The origin of this software must not be misrepresented; you must not\n'
                        '//   claim that you wrote the original software. If you use this software\n'
                        '//   in a product, an acknowledgment in the product documentation would be\n'
                        '//   appreciated but is not required.\n'
                        '//\n'
                        '//   2. Altered source versions must be plainly marked as such, and must not be\n'
                        '//   misrepresented as being the original software.\n'
                        '//\n'
                        '//   3. This notice may not be removed or altered from any source\n'
                        '//   distribution.\n'
                        '//------------------------------------------------------------------------------\n'
                        '\n'
                        '#include <GL/glcorearb.h>\n'
                        '\n'
                        '#include "core/base_defines.h"\n'
                        '\n'
                        '// Delcarations of OpenGL function pointers\n'
                        '%s\n'
                        '\n'
                        'void init_win32gl()\n'
                        '{\n'
                        '    HMODULE hOpenGL32 = GetModuleHandle("opengl32.dll");\n'
                        '    if (!hOpenGL32)\n'
                        '        PANIC("Unable to load opengl32.dll");\n'
                        '\n'
                        '    // Initialize all function pointers to OpenGL functions.\n'
                        '%s\n'
                        '}\n'
                    )

def proc_file(filename):
    lines = [line.rstrip() for line in open(filename)]
    funcs = get_funcs(lines)
    externs, vars, inits = proc_funcs(funcs)

    win32gl_h = open('win32gl.h', 'wb')
    win32gl_h.write(WIN32GL_H_TEMPLATE % '\n'.join(externs))
    win32gl_h.close()

    win32gl_cpp = open('win32gl.cpp', 'wb')
    win32gl_cpp.write(WIN32GL_CPP_TEMPLATE % ('\n'.join(vars), '\n'.join(inits)))
    win32gl_cpp.close()

def main():
    proc_file(sys.argv[1])

if __name__ == '__main__':
    main()


    
