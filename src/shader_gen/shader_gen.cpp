//------------------------------------------------------------------------------
// shader_gen.cpp - Command line tool that generates C++ classes from raw shaders.
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

#include "core/mem.h"
#include "core/List.h"
#include "core/String.h"
#include "assets/Config.h"
#include "assets/file_utils.h"
#include "renderergl/RendererGL.h"
#include "shader_gen/glutils.h"

namespace gaen
{

#define S String<kMEM_Renderer>
#define LF S("\n")
#define I indent(indentLevel)
#define I1 indent(indentLevel+1)
#define I2 indent(indentLevel+2)
#define I3 indent(indentLevel+3)

static const char * kShadedHeader =
    "//------------------------------------------------------------------------------\n"
    "// %s.cpp - Auto-generated shader from %s.shd\n"
    "//\n"
    "// Gaen Concurrency Engine - http://gaen.org\n"
    "// Copyright (c) 2014-2015 Lachlan Orr\n"
    "//\n"
    "// This software is provided 'as-is', without any express or implied\n"
    "// warranty. In no event will the authors be held liable for any damages\n"
    "// arising from the use of this software.\n"
    "//\n"
    "// Permission is granted to anyone to use this software for any purpose,\n"
    "// including commercial applications, and to alter it and redistribute it\n"
    "// freely, subject to the following restrictions:\n"
    "//\n"
    "//   1. The origin of this software must not be misrepresented; you must not\n"
    "//   claim that you wrote the original software. If you use this software\n"
    "//   in a product, an acknowledgment in the product documentation would be\n"
    "//   appreciated but is not required.\n"
    "//\n"
    "//   2. Altered source versions must be plainly marked as such, and must not be\n"
    "//   misrepresented as being the original software.\n"
    "//\n"
    "//   3. This notice may not be removed or altered from any source\n"
    "//   distribution.\n"
    "//------------------------------------------------------------------------------\n";

struct ShaderVarInfo
{
    S name;
    u32 index;
    GLenum type;
};

struct ShaderSource
{
    u32 type;
    S path;

    ShaderSource(u32 type, const char * path)
      : type(type)
      , path(path)
    {}
};

struct ShaderInfo
{
    S name;
    S outPathCpp;
    S outPathH;
    List<kMEM_Renderer, ShaderVarInfo> attibutes; 
    List<kMEM_Renderer, ShaderVarInfo> uniforms;
};

List<kMEM_Renderer, ShaderSource> parse_shd(const char * shdPath)
{
    List<kMEM_Renderer, ShaderSource> sources;

    FileReader rdr(shdPath);
    Config<kMEM_Chef> shd;
    shd.read(rdr.ifs);

    char scratch[kMaxPath];

    static const char * kShaderNames[] = {"vert_shader",
                                          "frag_shader",
                                          "comp_shader",
                                          nullptr};
    static const u32 kShaderTypes[] = {GL_VERTEX_SHADER,
                                       GL_FRAGMENT_SHADER,
                                       GL_COMPUTE_SHADER,
                                       0};

    u32 idx = 0;
    while (kShaderNames[idx])
    {
        if (shd.hasKey(kShaderNames[idx]))
        {
            strcpy(scratch, shdPath);
            parent_dir(scratch);
            append_path(scratch, shd.get(kShaderNames[idx]));
            sources.emplace_back(kShaderTypes[idx], scratch);
        }
        idx++;
    }

    return sources;
}

void process_shader_program(ShaderInfo & si, const List<kMEM_Renderer, ShaderSource> & sources)
{
    // create program
    GLuint programId = glCreateProgram();

    List<kMEM_Renderer, GLuint> shaderList;

    // attach shaders to program
    for (const ShaderSource & source : sources)
    {
        FileReader sourceRdr(source.path.c_str());
        Scoped_GFREE<char> sourceCode((char*)GALLOC(kMEM_Renderer, sourceRdr.size()+1)); // +1 for null we'll add to end
        sourceRdr.read(sourceCode.get(), sourceRdr.size());
        sourceCode.get()[sourceRdr.size()] = '\0';

        GLuint shader;
        bool sourceCompRes = RendererGL::compile_shader(&shader, source.type, sourceCode.get(), SHADER_HEADER);
        if (!sourceCompRes)
        {
            PANIC("Failed to compile shader: %s", source.path);
        }

        shaderList.push_back(shader);

        glAttachShader(programId, shader);
    }

    
    // link program
    GLint status;
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        for (GLuint shader : shaderList)
        {
            glDeleteShader(shader);
        }
        PANIC("Failed to link shader program");
    }

    GLsizei nameLen;
    GLint size;
    GLenum type;
    char name[kMaxPath+1];
    
    // Extract goodness
    GLint attribCount = 0;
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &attribCount);

    for (i32 i = 0; i < attribCount; ++i)
    {
        glGetActiveAttrib(programId, i, kMaxPath, &nameLen, &size, &type, name);
        name[nameLen] = '\0';

        si.attibutes.push_back(ShaderVarInfo());
        si.attibutes.back().index = i;
        si.attibutes.back().name = name;
        si.attibutes.back().type = type;
    }



    GLint uniformCount = 0;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

    for (i32 i = 0; i < uniformCount; ++i)
    {
        glGetActiveUniform(programId, i, kMaxPath, &nameLen, &size, &type, name);
        name[nameLen] = '\0';

        si.uniforms.push_back(ShaderVarInfo());
        si.uniforms.back().index = i;
        si.uniforms.back().name = name;
        si.uniforms.back().type = type;
    }

    // Release vertex and fragment shaders
    glDeleteProgram(programId);
    for (GLuint shader : shaderList)
    {
        glDeleteShader(shader);
    }
}

S generate_shader_h(const ShaderInfo & si)
{
    char scratch[kMaxPath+1];

    S code = "";

    snprintf(scratch, kMaxPath, "#ifndef GAEN_RENDERERGL_SHADERS_%s_H", si.name);

    code += S("namespace gaen\n");
    code += S("{\n");
    code += S("namespace shaders\n");
    code += S("{\n");
    code += LF;
    
    code += S("class ") + 

    code += LF;
    code += S("} // namespace shaders\n");
    code += S("} // namespace gaen\n");

    return code;
}

void generate_shader(const char * shdPath)
{
    char shdPathNorm[kMaxPath+1];
    normalize_path(shdPathNorm, shdPath);

    PANIC_IF(0 != strcmp("shd", get_ext(shdPathNorm)), "Shaders does not end in .shd extension: %s", shdPath);

    char filenameRoot[kMaxPath+1];
    get_filename_root(filenameRoot, shdPathNorm);

    char outCpp[kMaxPath+1];
    strcpy(outCpp, shdPathNorm);
    change_ext(outCpp, "cpp");
    char outH[kMaxPath+1];
    strcpy(outH, shdPathNorm);
    change_ext(outH, "h");

    List<kMEM_Renderer, ShaderSource> sources = parse_shd(shdPathNorm);

    ShaderInfo si;
    si.name = filenameRoot;
    si.outPathCpp = outCpp;
    si.outPathH = outH;
    process_shader_program(si, sources);

    S hCode = generate_shader_h(si);
}


}

// Force Optimus enabled systems to use Nvidia adapter
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(int argc, char ** argv)
{
    using namespace gaen;

    if (argc != 2)
    {
        printf("Usage: shader_gen <input_shd_file>\n");
        exit(1);
    }

    init_opengl();
    generate_shader(argv[1]);
    fin_opengl();
}

