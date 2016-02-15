//------------------------------------------------------------------------------
// Shader.cpp - Base class for shaders
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#include <glm/gtc/type_ptr.hpp>

#include "renderergl/RendererGL.h"

#include "renderergl/shaders/Shader.h"

namespace gaen
{

namespace shaders
{

Shader::Shader(u32 nameHash)
  : mNameHash(nameHash)
  , mProgramId(0)
  , mCodeCount(0)
  , mUniformCount(0)
  , mAttributeCount(0)
  , mpCodes(nullptr)
  , mpUniforms(nullptr)
  , mpAttributes(nullptr)
{}

void Shader::load()
{
    ASSERT(!mIsLoaded);

    if (!mIsLoaded)
    {
        mProgramId = glCreateProgram();

        static const u32 kCodeCountMax = 8;
        GLuint shaderIds[kCodeCountMax];
        memset(shaderIds, 0, sizeof(shaderIds));
        PANIC_IF(mCodeCount > kCodeCountMax, "Too many shader codes: %u, max: %u", mCodeCount, kCodeCountMax);

        // loop over shader's codes and compile them
        for (u32 i = 0; i < mCodeCount; ++i)
        {
            if (mpCodes[i].stage == 0)
                break;
        
            PANIC_IF(!Shader::compile_shader(&shaderIds[i], mpCodes[i].stage, mpCodes[i].code, SHADER_HEADER), "Failed to compile shader: %s", mpCodes[i].filename);
            glAttachShader(mProgramId, shaderIds[i]);
        }

        // link program
        GLint status;
        glLinkProgram(mProgramId);
        glGetProgramiv(mProgramId, GL_LINK_STATUS, &status);
        PANIC_IF(status == 0, "Failed to link shader program: faceted");

        // Release shaders
        for (u32 i = 0; i < mCodeCount; ++i)
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
    for (u32 i = 0; i < mUniformCount; ++i)
    {
        if (mpUniforms[i].nameHash == 0)
            break;
        if (mpUniforms[i].nameHash == nameHash &&
            mpUniforms[i].type == type)
        {
            return &mpUniforms[i];
        }
    }
    return nullptr;
}

void Shader::setUniformUint(u32 nameHash, u32 value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_UNSIGNED_INT);
    if (pUniform)
        glUniform1ui(pUniform->location, value);
    else
        ERR("UniformUint does not exist in shader");
}

void Shader::setUniformVec3(u32 nameHash, const glm::vec3 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_VEC3);
    if(pUniform)
        glUniform3fv(pUniform->location, 1, glm::value_ptr(value));
    else
        ERR("UniformVec3 does not exist in shader");
}

void Shader::setUniformVec4(u32 nameHash, const glm::vec4 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_VEC4);
    if(pUniform)
        glUniform4fv(pUniform->location, 1, glm::value_ptr(value));
    else
        ERR("UniformVec4 does not exist in shader");
}

void Shader::setUniformMat3(u32 nameHash, const glm::mat3 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_MAT3);
    if(pUniform)
        glUniformMatrix3fv(pUniform->location, 1, 0, glm::value_ptr(value));
    else
        ERR("UniformMat3 does not exist in shader");
}

void Shader::setUniformMat4(u32 nameHash, const glm::mat4 & value)
{
    ASSERT(mIsLoaded);
    VariableInfo * pUniform = findUniform(nameHash, GL_FLOAT_MAT4);
    if(pUniform)
        glUniformMatrix4fv(pUniform->location, 1, 0, glm::value_ptr(value));
    else
        ERR("UniformMat4 does not exist in shader");
}

u32 Shader::textureLocation(u32 nameHash, u32 type)
{
    VariableInfo * pUniform = findUniform(nameHash, type);
    if(pUniform)
        return pUniform->location;
    else
    {
        ERR("Texture does not exist in shader");
        return -1;
    }
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
