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

struct ShaderVarInfo
{
    String<kMEM_Renderer> name;
    u32 index;
    GLenum type;
};

struct ShaderInfo
{
    String<kMEM_Renderer> name;
    String<kMEM_Renderer> outPathCpp;
    String<kMEM_Renderer> outPathH;
    List<kMEM_Renderer, ShaderVarInfo> attibutes; 
    List<kMEM_Renderer, ShaderVarInfo> uniforms;
};

void parse_shd(char * vertPath, char * fragPath, const char * shdPath)
{
    FileReader rdr(shdPath);
    Config<kMEM_Chef> shd;
    shd.read(rdr.ifs);

    static const char * kVertShader = "vert_shader";
    static const char * kFragShader = "frag_shader";

    PANIC_IF(!shd.hasKey(kVertShader), "Missing vert_shader in .shd");
    PANIC_IF(!shd.hasKey(kFragShader), "Missing frag_shader in .shd");

    strcpy(vertPath, shdPath);
    strcpy(fragPath, shdPath);
    parent_dir(vertPath);
    parent_dir(fragPath);

    append_path(vertPath, shd.get(kVertShader));
    append_path(fragPath, shd.get(kFragShader));
}

void process_shader_program(ShaderInfo & si, const char * vertPath, const char * fragPath)
{
    FileReader vertRdr(vertPath);
    Scoped_GFREE<char> vertCode((char*)GALLOC(kMEM_Renderer, vertRdr.size()+1)); // +1 for null we'll add to end
    vertRdr.read(vertCode.get(), vertRdr.size());
    vertCode.get()[vertRdr.size()] = '\0';

    FileReader fragRdr(fragPath);
    Scoped_GFREE<char> fragCode((char*)GALLOC(kMEM_Renderer, fragRdr.size()+1)); // +1 for null we'll add to end
    fragRdr.read(fragCode.get(), fragRdr.size());
    fragCode.get()[fragRdr.size()] = '\0';

    GLuint vertShader;
    bool vertCompRes = RendererGL::compile_shader(&vertShader, GL_VERTEX_SHADER, vertCode.get(), SHADER_HEADER);
    if (!vertCompRes)
    {
        PANIC("Failed to compile vertex shader: %s", vertPath);
    }

    GLuint fragShader;
    bool fragCompRes = RendererGL::compile_shader(&fragShader, GL_FRAGMENT_SHADER, fragCode.get(), SHADER_HEADER);
    if (!fragCompRes)
    {
        glDeleteShader(vertShader);
        PANIC("Failed to compile fragment shader: %s", fragPath);
    }


    // create program
    GLuint programId = glCreateProgram();


    // attach shaders to program
    glAttachShader(programId, vertShader);
    glAttachShader(programId, fragShader);

    
    // link program
    GLint status;
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        glDeleteProgram(programId);
        PANIC("Failed to link shader program: vert: %s, frag: %s", vertPath, fragPath);
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
    if (programId != 0)
        glDeleteProgram(programId);
    if (vertShader != 0)
        glDeleteShader(vertShader);
    if (fragShader != 0)
        glDeleteShader(fragShader);

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

    char vertPath[kMaxPath+1];
    char fragPath[kMaxPath+1];

    parse_shd(vertPath, fragPath, shdPathNorm);

    ShaderInfo si;
    si.name = filenameRoot;
    si.outPathCpp = outCpp;
    si.outPathH = outH;
    process_shader_program(si, vertPath, fragPath);

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

