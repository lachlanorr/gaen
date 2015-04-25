//------------------------------------------------------------------------------
// Shader.h - Base class for shaders
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#include "renderergl/RendererGL.h"

#include "renderergl/shaders/Shader.h"

namespace gaen
{

namespace shaders
{

Shader::Shader(u32 nameHash)
  : mNameHash(nameHash)
  , mProgramId(0)
{
    memset(&mCodes, 0, sizeof(mCodes));
    memset(&mUniforms, 0, sizeof(mUniforms));
    memset(&mAttributes, 0, sizeof(mAttributes));
}

void Shader::load()
{
    ASSERT(!mIsLoaded);

    if (!mIsLoaded)
    {
        mProgramId = glCreateProgram();

        GLuint shaderIds[kCodeMax];
        memset(shaderIds, 0, sizeof(shaderIds));

        // loop over shader's codes and compile them
        for (u32 i = 0; i < kCodeMax; ++i)
        {
            if (mCodes[i].stage == 0)
                break;
        
            PANIC_IF(!Shader::compile_shader(&shaderIds[i], mCodes[i].stage, mCodes[i].code, SHADER_HEADER), "Failed to compile shader: %s", mCodes[i].filename);
            glAttachShader(mProgramId, shaderIds[i]);
        }

        // link program
        GLint status;
        glLinkProgram(mProgramId);
        glGetProgramiv(mProgramId, GL_LINK_STATUS, &status);
        PANIC_IF(status == 0, "Failed to link shader program: faceted");

        // Release shaders
        for (u32 i = 0; i < kCodeMax; ++i)
        {
            if (shaderIds[i] == 0)
                break;
            glDeleteShader(shaderIds[i]);
        }

        mIsLoaded = true;
    }
}

void Shader::unload()
{
    ASSERT(mIsLoaded);

    if (mIsLoaded)
    {
        ASSERT(mProgramId);
        if (mProgramId)
        {
            glDeleteProgram(mProgramId);
            mProgramId = 0;
        }
        mIsLoaded = false;
    }
}

void Shader::use()
{
    if (!mIsLoaded)
        load();
    glUseProgram(mProgramId);
}

Shader::VariableInfo * Shader::findUniform(u32 nameHash, u32 type)
{
    for (u32 i = 0; i < kUniformMax; ++i)
    {
        if (mUniforms[i].nameHash == 0)
            break;;
        if (mUniforms[i].nameHash == nameHash &&
            mUniforms[i].type == type)
        {
            return &mUniforms[i];
        }
    }
    return nullptr;
}

void Shader::setUniformVec3(u32 nameHash, const Vec3 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_VEC3);
    PANIC_IF(!pUniform, "UniformVec3 does not exist in shader");
    glUniform3fv(pUniform->index, 1, value.elems);
}

void Shader::setUniformVec4(u32 nameHash, const Vec4 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_VEC4);
    PANIC_IF(!pUniform, "UniformVec4 does not exist in shader");
    glUniform4fv(pUniform->index, 1, value.elems);
}

void Shader::setUniformMat3(u32 nameHash, const Mat3 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_MAT3);
    PANIC_IF(!pUniform, "UniformMat3 does not exist in shader");
    glUniformMatrix3fv(pUniform->index, 1, 0, value.elems);
}

void Shader::setUniformMat4(u32 nameHash, const Mat4 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_MAT4);
    PANIC_IF(!pUniform, "UniformMat4 does not exist in shader");
    glUniformMatrix4fv(pUniform->index, 1, 0, value.elems);
}

bool Shader::compile_shader(GLuint * pShader, GLenum type, const char * shaderCode, const char * headerCode)
{
    const char * shaderCodes[2];
    u32 shaderCodesSize = 0;

    if (headerCode)
    {
        shaderCodes[shaderCodesSize++] = headerCode;
    }
    shaderCodes[shaderCodesSize++] = shaderCode;

    GLuint shader;

    shader = glCreateShader(type);
    glShaderSource(shader, shaderCodesSize, shaderCodes, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        char errMsg[256];
        int len;
        glGetShaderInfoLog(shader, 256, &len, errMsg);

        glDeleteShader(shader);
        ERR("Failed to compile shader: %s", errMsg);
        return false;
    }

    *pShader = shader;
    return true;
}

} // namespace shaders
} // namespace gaen
