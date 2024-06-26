//------------------------------------------------------------------------------
// shader_gen.cpp - Command line tool that generates C++ classes from raw shaders
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#include "gaen/core/mem.h"
#include "gaen/core/hashing.h"
#include "gaen/core/List.h"
#include "gaen/core/String.h"
#include "gaen/assets/Config.h"
#include "gaen/assets/file_utils.h"
#include "gaen/renderergl/gaen_opengl.h"
#include "gaen/shader_gen/glutils.h"

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
    S name;
    u32 index;
    u32 location;
    GLenum type;
};

struct ShaderSource
{
    u32 type;
    const char * typeStr;
    S path;
    S code;

    ShaderSource(u32 type, const char * typeStr, const char * path)
      : type(type)
      , typeStr(typeStr)
      , path(path)
    {}
};

struct ShaderInfo
{
    S name;
    S nameUpper;
    S outPathCpp;
    S outPathH;
    List<kMEM_Renderer, ShaderVarInfo> attributes;
    List<kMEM_Renderer, ShaderVarInfo> uniforms;
    List<kMEM_Renderer, ShaderVarInfo> textures;
    List<kMEM_Renderer, ShaderSource> sources;
};

const char * get_type_name(GLenum type)
{
    switch (type)
    {
    case GL_UNSIGNED_INT:
        return "GL_UNSIGNED_INT";
    case GL_FLOAT:
        return "GL_FLOAT";
    case GL_FLOAT_VEC2:
        return "GL_FLOAT_VEC2";
    case GL_FLOAT_VEC3:
        return "GL_FLOAT_VEC3";
    case GL_FLOAT_VEC4:
        return "GL_FLOAT_VEC4";
    case GL_FLOAT_MAT3:
        return "GL_FLOAT_MAT3";
    case GL_FLOAT_MAT4:
        return "GL_FLOAT_MAT4";
    case GL_IMAGE_2D:
        return "GL_IMAGE_2D";
    case GL_UNSIGNED_INT_IMAGE_2D:
        return "GL_UNSIGNED_INT_IMAGE_2D";
    case GL_UNSIGNED_INT_IMAGE_BUFFER:
        return "GL_UNSIGNED_INT_IMAGE_BUFFER";
    case GL_SAMPLER_2D:
        return "GL_SAMPLER_2D";
    }
    PANIC("Unsupported GLenum type: 0x%x", type);
    return nullptr;
}

S generate_header(const char * shaderRootName, const char * extension)
{
    char scratch[512];

    S code("//------------------------------------------------------------------------------\n");

    sprintf(scratch, "// %s.%s - Auto-generated shader from %s.shd\n", shaderRootName, extension, shaderRootName);
    code += scratch;

    code +=
    "//\n"
    "// Gaen Concurrency Engine - http://gaen.org\n"
    "// Copyright (c) 2014-2022 Lachlan Orr\n"
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

    return code;
}

void append_cmake_file_cb(const char * path, void * context)
{
    if (!strstr(path, "codegen.cmake"))
    {
        S & code = *reinterpret_cast<S*>(context);
        char scratch[kMaxPath+1];
        get_filename(scratch, path);
        code += S("  ${shaders_dir}/") + scratch + LF;
    }
}

S generate_codegen_cmake(const char * shadersDir)
{
    S code =
        "#-------------------------------------------------------------------------------\n"
        "# codegen.cmake - Autogenerated cmake containing generated classes\n"
        "#\n"
        "# Gaen Concurrency Engine - http://gaen.org\n"
        "# Copyright (c) 2014-2022 Lachlan Orr\n"
        "#\n"
        "# This software is provided 'as-is', without any express or implied\n"
        "# warranty. In no event will the authors be held liable for any damages\n"
        "# arising from the use of this software.\n"
        "#\n"
        "# Permission is granted to anyone to use this software for any purpose,\n"
        "# including commercial applications, and to alter it and redistribute it\n"
        "# freely, subject to the following restrictions:\n"
        "#\n"
        "#   1. The origin of this software must not be misrepresented; you must not\n"
        "#   claim that you wrote the original software. If you use this software\n"
        "#   in a product, an acknowledgment in the product documentation would be\n"
        "#   appreciated but is not required.\n"
        "#\n"
        "#   2. Altered source versions must be plainly marked as such, and must not be\n"
        "#   misrepresented as being the original software.\n"
        "#\n"
        "#   3. This notice may not be removed or altered from any source\n"
        "#   distribution.\n"
        "#-------------------------------------------------------------------------------\n"
        "\n"
        "SET (shaders_codegen_SOURCES\n";

    recurse_dir(shadersDir, &code, append_cmake_file_cb);

    code +=
        ")\n"
        "\n"
        "source_group( \"shaders\" ${shaders_codegen_SOURCES})\n";

    return code;
}

void append_shader_registration_file_includes_cb(const char * path, void * context)
{
    if (0 == strcmp(get_ext(path), "h") && !strstr(path, "/Shader.h"))
    {
        char scratch[kMaxPath+1];
        get_filename_root(scratch, path);
        if (0 == strcmp(scratch, "Shader.h"))
            return;
        char scratch2[kMaxPath+1];
        sprintf(scratch2, "#include \"gaen/renderergl/shaders/%s.h\"\n", scratch);
        S & code = *reinterpret_cast<S*>(context);
        code += scratch2;
    }
}

void append_shader_registration_file_cb(const char * path, void * context)
{
    if (0 == strcmp(get_ext(path), "h") && !strstr(path, "/Shader.h"))
    {
        char scratch[kMaxPath+1];
        get_filename_root(scratch, path);
        if (0 == strcmp(scratch, "Shader.h"))
            return;
        char scratch2[kMaxPath+1];
        sprintf(scratch2, "    registerShaderConstructor(0x%08x /* HASH::%s */, shaders::%s::construct);\n", gaen_hash(scratch), scratch, scratch);
        S & code = *reinterpret_cast<S*>(context);
        code += scratch2;
    }
}

S generate_shader_registration(const char * shadersDir)
{
    S code =
        "//------------------------------------------------------------------------------\n"
        "// ShaderRegistry_codegen.cpp - Shader factory class\n"
        "//\n"
        "// Gaen Concurrency Engine - http://gaen.org\n"
        "// Copyright (c) 2014-2022 Lachlan Orr\n"
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
        "//------------------------------------------------------------------------------\n"
        "\n"
        "#include \"gaen/renderergl/ShaderRegistry.h\"\n";

    recurse_dir(shadersDir, &code, append_shader_registration_file_includes_cb);

    code +=
        "\n"
        "namespace gaen\n"
        "{\n"
        "\n"
        "void ShaderRegistry::registerAllShaderConstructors()\n"
        "{\n";

    recurse_dir(shadersDir, &code, append_shader_registration_file_cb);

    code +=
        "}\n"
        "\n"
        "\n"
        "} // namespace gaen\n";

    return code;
}

void parse_shd(ShaderInfo & si, const char * shdPath)
{
    FileReader rdr(shdPath);
    PANIC_IF(!rdr.isOk(), "Unable to load file: %s", shdPath);
    Config<kMEM_Chef> shd;
    shd.read(rdr.ifs);

    char scratch[kMaxPath];

    static const char * kShaderNames[] = {"vertex_shader",
                                          "fragment_shader",
                                          "compute_shader",
                                          nullptr};
    static const u32 kShaderTypes[] = {GL_VERTEX_SHADER,
                                       GL_FRAGMENT_SHADER,
                                       GL_COMPUTE_SHADER,
                                       0};

    static const char * kShaderTypeStrs[] = {"GL_VERTEX_SHADER",
                                             "GL_FRAGMENT_SHADER",
                                             "GL_COMPUTE_SHADER",
                                             nullptr};

    u32 idx = 0;
    while (kShaderNames[idx])
    {
        if (shd.hasKey(kShaderNames[idx]))
        {
            strcpy(scratch, shdPath);
            parent_dir(scratch);
            append_path(scratch, shd.get(kShaderNames[idx]));
            si.sources.emplace_back(kShaderTypes[idx], kShaderTypeStrs[idx], scratch);
        }
        idx++;
    }
}

bool should_generate_shader(ShaderInfo & si)
{
    if (!file_exists(si.outPathH.c_str()) ||
        !file_exists(si.outPathCpp.c_str()))
        return true;

    char procPath[kMaxPath+1];
    process_path(procPath);
    if (is_file_newer(procPath, si.outPathCpp.c_str()) ||
        is_file_newer(procPath, si.outPathH.c_str()))
        return true;

    for (const ShaderSource & ss : si.sources)
    {
        if (is_file_newer(ss.path.c_str(), si.outPathCpp.c_str()) ||
            is_file_newer(ss.path.c_str(), si.outPathH.c_str()))
            return true;
    }
    return false;
}

bool compile_shader(GLuint * pShader, GLenum type, const char * shaderCode, const char * headerCode)
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

void process_shader_program(ShaderInfo & si)
{
    // create program
    GLuint programId = glCreateProgram();

    List<kMEM_Renderer, GLuint> shaderList;

    // attach shaders to program
    for (ShaderSource & source : si.sources)
    {
        FileReader sourceRdr(source.path.c_str());
        PANIC_IF(!sourceRdr.isOk(), "Unable to load file: %s", source.path.c_str());

        Scoped_GFREE<char> sourceCode((char*)GALLOC(kMEM_Renderer, sourceRdr.size()+1)); // +1 for null we'll add to end
        sourceRdr.read(sourceCode.get(), sourceRdr.size());
        sourceCode.get()[sourceRdr.size()] = '\0';

        Scoped_GFREE<char> sourceCodeLF((char*)GALLOC(kMEM_Renderer, sourceRdr.size()+1)); // +1 for null we'll add to end
        // copy excluding \r, we need to normalize to only \n so we don't get mixed line feeds in output
        char * src = sourceCode.get();
        char * dst = sourceCodeLF.get();
        while (*src)
        {
            if (*src != '\r')
                *dst++ = *src++;
            else
                src++;
        }
        *dst = '\0';

        source.code = sourceCodeLF.get();

        GLuint shader;
        bool sourceCompRes = compile_shader(&shader, source.type, sourceCodeLF.get(), SHADER_HEADER);
        if (!sourceCompRes)
        {
            PANIC("Failed to compile shader: %s", source.path.c_str());
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
        char errMsg[256];
        int len;
        glGetProgramInfoLog(programId, 256, &len, errMsg);

        for (GLuint shader : shaderList)
        {
            glDeleteShader(shader);
        }
        PANIC("Failed to link shader program: %s", errMsg);
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

        si.attributes.push_back(ShaderVarInfo());
        si.attributes.back().index = i;
        si.attributes.back().location = glGetAttribLocation(programId, name);
        si.attributes.back().name = name;
        si.attributes.back().type = type;
    }



    GLint uniformCount = 0;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

    for (i32 i = 0; i < uniformCount; ++i)
    {
        glGetActiveUniform(programId, i, kMaxPath, &nameLen, &size, &type, name);
        name[nameLen] = '\0';

        si.uniforms.push_back(ShaderVarInfo());
        si.uniforms.back().index = i;
        si.uniforms.back().location = glGetUniformLocation(programId, name);
        si.uniforms.back().name = name;
        si.uniforms.back().type = type;

        if (type == GL_SAMPLER_2D)
        {
            si.textures.push_back(si.uniforms.back());
        }
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

    S code = generate_header(si.name.c_str(), "h");

    snprintf(scratch, kMaxPath, "#ifndef GAEN_RENDERERGL_SHADERS_%s_H\n", si.nameUpper.c_str());
    code += scratch;
    snprintf(scratch, kMaxPath, "#define GAEN_RENDERERGL_SHADERS_%s_H\n", si.nameUpper.c_str());
    code += scratch;

    code += ("#include \"gaen/renderergl/shaders/Shader.h\"\n");

    code += S("namespace gaen\n");
    code += S("{\n");
    code += S("namespace shaders\n");
    code += S("{\n");
    code += LF;

    code += S("class ") + si.name + S(" : Shader\n");
    code += S("{\n");
    code += S("public:\n");
    code += S("    static Shader * construct();\n");
    code += LF;
    code += S("private:\n");

    snprintf(scratch, kMaxPath, "    %s() : Shader(0x%08x /* HASH::%s */) {}\n", si.name.c_str(), gaen_hash(si.name.c_str()), si.name.c_str());
    code += scratch;
    code += LF;

    // storage for codes, uniforms, and attributes
    snprintf(scratch, kMaxPath, "    static const u32 kCodeCount = %u;\n", (u32)si.sources.size());
    code += scratch;
    snprintf(scratch, kMaxPath, "    static const u32 kUniformCount = %u;\n", (u32)si.uniforms.size());
    code += scratch;
    snprintf(scratch, kMaxPath, "    static const u32 kAttributeCount = %u;\n", (u32)si.attributes.size());
    code += scratch;
    snprintf(scratch, kMaxPath, "    static const u32 kTextureCount = %u;\n", (u32)si.textures.size());
    code += scratch;

    code += LF;

    code += S("    Shader::ShaderCode mCodes[kCodeCount];\n");
    if (si.uniforms.size() > 0)
    {
        code += S("    Shader::VariableInfo mUniforms[kUniformCount];\n");
    }
    if (si.attributes.size() > 0)
    {
        code += S("    Shader::VariableInfo mAttributes[kAttributeCount];\n");
    }
    if (si.textures.size() > 0)
    {
        code += S("    Shader::VariableInfo mTextures[kTextureCount];\n");
    }

    code += S("}; // class ") + si.name + LF;

    code += LF;
    code += S("} // namespace shaders\n");
    code += S("} // namespace gaen\n");

    snprintf(scratch, kMaxPath, "#endif // #ifdef GAEN_RENDERERGL_SHADERS_%s_H\n", si.nameUpper.c_str());
    code += scratch;

    return code;
}

S gen_shader_code_const(const char * rawCode)
{
    S code;

    const char * bol = rawCode;
    const char * eol;

    while (*bol)
    {
        eol = bol;
        while (*eol && *eol != '\n')
        {
            eol++;
        }

        code += S("    \"");
        code += S(bol, eol - bol);
        code += S("\\n\"\n");

        if (*eol)
            bol = eol+1;
        else
            break;
    }

    return code;
}

S generate_shader_cpp(const ShaderInfo & si)
{
    char scratch[kMaxPath+1];

    S code = generate_header(si.name.c_str(), "cpp");

    code += LF;
    code += S("#include \"gaen/core/mem.h\"\n");

    snprintf(scratch, kMaxPath, "#include \"gaen/renderergl/shaders/%s.h\"\n", si.name.c_str());
    code += scratch;
    code += LF;

    code += S("namespace gaen\n");
    code += S("{\n");
    code += S("namespace shaders\n");
    code += S("{\n");
    code += LF;

    for (const ShaderSource & source : si.sources)
    {
        code += S("static const char * kShaderCode_") + get_ext(source.path.c_str()) + S(" =\n");
        code += gen_shader_code_const(source.code.c_str());
        code += S("    ; // kShaderCode_") + get_ext(source.path.c_str()) + S(" (END)\n");
        code += LF;
    }

    snprintf(scratch, kMaxPath, "Shader * %s::construct()\n", si.name.c_str());
    code += scratch;
    code += S("{\n");

    snprintf(scratch, kMaxPath, "    %s * pShader = GNEW(kMEM_Renderer, %s);\n", si.name.c_str(), si.name.c_str());
    code += scratch;
    code += LF;

    code += S("    // Program Codes\n");
    u32 i = 0;
    for (const ShaderSource & source : si.sources)
    {
        snprintf(scratch, kMaxPath, "    pShader->mCodes[%u].stage = %s;\n", i, source.typeStr);
        code += scratch;
        snprintf(scratch, kMaxPath, "    pShader->mCodes[%u].filename = \"%s\";\n", i, get_filename(source.path.c_str()));
        code += scratch;
        snprintf(scratch, kMaxPath, "    pShader->mCodes[%u].code = kShaderCode_%s;\n", i, get_ext(source.path.c_str()));
        code += scratch;
        code += LF;

        ++i;
    }

    code += S("    // Uniforms\n");
    code += S("    //  These are computed at runtime in Shader::processProgram()\n");

    code += LF;

    code += S("    // Attributes\n");
    code += S("    //  These are computed at runtime in Shader::processProgram()\n");

    code += LF;

    code += S("    // Textures\n");
    code += S("    //  These are computed at runtime in Shader::processProgram()\n");

    code += LF;

    code += S("    // Set base Shader members to our arrays and counts\n");
    code += S("    pShader->mCodeCount = kCodeCount;\n");
    code += S("    pShader->mpCodes = pShader->mCodes;\n");
    code += S("    pShader->mUniformCount = kUniformCount;\n");
    if (si.uniforms.size() > 0)
        code += S("    pShader->mpUniforms = pShader->mUniforms;\n");
    else
        code += S("    pShader->mpUniforms = nullptr;\n");
    code += S("    pShader->mAttributeCount = kAttributeCount;\n");
    if (si.attributes.size() > 0)
        code += S("    pShader->mpAttributes = pShader->mAttributes;\n");
    else
        code += S("    pShader->mpAttributes = nullptr;\n");
    code += S("    pShader->mTextureCount = kTextureCount;\n");
    if (si.textures.size() > 0)
        code += S("    pShader->mpTextures = pShader->mTextures;\n");
    else
        code += S("    pShader->mpTextures = nullptr;\n");

    code += LF;

    code += S("    return pShader;\n");
    code += S("}\n");

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

    ShaderInfo si;

    si.name = filenameRoot;
    upper(filenameRoot);
    si.nameUpper = filenameRoot;

    si.outPathCpp = outCpp;
    si.outPathH = outH;

    parse_shd(si, shdPathNorm);

    if (should_generate_shader(si))
    {
        process_shader_program(si);

        S hCode = generate_shader_h(si);
        printf("Writing %s\n", outH);
        FileWriter wrtH(outH);
        wrtH.ofs.write(hCode.c_str(), hCode.length());

        S cppCode = generate_shader_cpp(si);
        printf("Writing %s\n", outCpp);
        FileWriter wrtCpp(outCpp);
        wrtCpp.ofs.write(cppCode.c_str(), cppCode.length());
    }
}

void process_shd_file_cb(const char * path, void * context)
{
    if (strcmp(get_ext(path), "shd") == 0)
    {
        generate_shader(path);
    }
}


void find_shaders_dir(char * shadersDir)
{
    char path[kMaxPath+1];
    char checkPath[kMaxPath+1];
    char checkProjectPath[kMaxPath+1];
    char * foundPath = nullptr;

    process_path(path);
    parent_dir(path);

    for (;;)
    {
        PANIC_IF(!*path, "Unable to find shaders dir, make sure shader_gen executable is located under a gaen or gaen project directory tree.");

        snprintf(checkPath, kMaxPath, "%s/src/gaen/renderergl/shaders/", path);
        snprintf(checkProjectPath, kMaxPath, "%s/gaen/src/gaen/renderergl/shaders/", path);
        printf("checkPath: %s\n", checkPath);
        printf("checkProjectPath: %s\n", checkPath);
        printf("-----\n");
        if (dir_exists(checkPath))
        {
            foundPath = checkPath;
        }
        else if (dir_exists(checkProjectPath))
        {
            foundPath = checkProjectPath;
        }

        if (foundPath)
        {
            // get rid of / at end
            size_t foundPathLen = strlen(foundPath);
            foundPath[foundPathLen-1] = '\0';
            strcpy(shadersDir, foundPath);
            return;
        }
        parent_dir(path);
    }
}

}

// Force Optimus enabled systems to use Nvidia adapter
#if IS_PLATFORM_WIN32
extern "C"
{
    __declspec(dllexport) gaen::u32 NvOptimusEnablement = 0x00000001;
}
#endif

int main(int argc, char ** argv)
{
    using namespace gaen;

    init_opengl();

    char shadersDir[kMaxPath+1];
    find_shaders_dir(shadersDir);
    printf("shadersDir: %s\n", shadersDir);

    if (argc == 1)
    {
        // no args, gen all shaders
        recurse_dir(shadersDir, nullptr, process_shd_file_cb);

        // generate shader registration
        S regCode = generate_shader_registration(shadersDir);
        char regPath[kMaxPath+1];
        strcpy(regPath, shadersDir);
        append_path(regPath, "ShaderRegistry_codegen.cpp");
        if (write_file_if_contents_differ(regPath, regCode.c_str()))
            printf("Writing %s\n", regPath);

        // generate codegen.cmake
        S codegenCmake = generate_codegen_cmake(shadersDir);
        char cmakePath[kMaxPath+1];
        strcpy(cmakePath, shadersDir);
        append_path(cmakePath, "codegen.cmake");
        if (write_file_if_contents_differ(cmakePath, codegenCmake.c_str()))
            printf("Writing %s\n", cmakePath);

    }
    else
    {
        // generate just the shader specified
        generate_shader(argv[1]);
    }

    fin_opengl();
}

