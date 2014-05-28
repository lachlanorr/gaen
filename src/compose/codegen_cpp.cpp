//------------------------------------------------------------------------------
// codegen_cpp.cpp - C++ code generation
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#include "compose/codegen_cpp.h"
#include "compose/compiler_structs.h"

#include "core/List.h"

namespace gaen
{

#define S String<kMEM_Compose>
#define LF S("\n")
#define I indent(indentLevel)

static S indent(u32 level)
{
    static const char * sIndents[] = { "",
                                       "    ",
                                       "        ",
                                       "            ",
                                       "                ",
                                       "                    ",
                                       "                        ",
                                       "                            "};
    static const u32 kMaxLevel = sizeof(sIndents) / sizeof(char*);

    if (level >= kMaxLevel)
        level = kMaxLevel - 1;

    return S(sIndents[level]);
}

static S type_str(DataType dt)
{
    switch (dt)
    {
    case kDT_int:
        return S("int");
    case kDT_uint:
        return S("uint");
    case kDT_float:
        return S("float");
    case kDT_bool:
        return S("bool");
    case kDT_char:
        return S("char");
    case kDT_vec3:
        return S("vec3");
    case kDT_vec4:
        return S("vec4");
    case kDT_mat3:
        return S("mat3");
    case kDT_mat34:
        return S("mat34");
    case kDT_mat4:
        return S("mat4");
    case kDT_void:
        return S("void");
    default:
        PANIC("type_str invalid DataType: %d", dt);
        return S("");
    }
}

static S codegen_recurse(const Ast * pAst,
                         int indentLevel)
{
    switch (pAst->type)
    {
    case kAST_FunctionDef:
    {
        return S("");
    }
    case kAST_EntityDef:
    {
        return S("");
    }
    case kAST_ComponentDef:
    {
        S code("namespace comp\n{\n\n");
        code += I + S("class ") + S(pAst->pSymRec->name) + S(" : public Component\n{\n");
        
        code += I + S("public:\n");
        code += I + S("    static Component* construct()\n");
        code += I + S("    {\n");
        code += I + S("        return GNEW(kMEM_Engine, ") + S(pAst->pSymRec->name) + S(");\n");
        code += I + S("    }\n");
        code += I + S("    \n");

        code += I + S("private:\n");
        code += I + S("    ") + S(pAst->pSymRec->name) + S("()\n");
        code += I + S("    {\n");
        code += I + S("        ASSERT_MSG(sIsRegistered, \"Component not registered: 0x%08x\", HASH::") + S(pAst->pSymRec->name) + S(");\n");
        code += I + S("    }\n");

        code += I + S("    ") + S(pAst->pSymRec->name) + S("(const ") + S(pAst->pSymRec->name) + S("&)      = delete;\n");
        code += I + S("    ") + S(pAst->pSymRec->name) + S("(const ") + S(pAst->pSymRec->name) + S("&&)     = delete;\n");
        code += I + S("    ") + S(pAst->pSymRec->name) + S(" & operator=(const ") + S(pAst->pSymRec->name) + S("&)  = delete;\n");
        code += I + S("    ") + S(pAst->pSymRec->name) + S(" & operator=(const ") + S(pAst->pSymRec->name) + S("&&) = delete;\n");
        
        code += S("\n");
        code += I + S("    static bool sIsRegistered;\n");
        
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            code += codegen_recurse(pChild, indentLevel + 1);
        }

        code += I + ("};\n\n");

        code += (I + S("bool ") + S(pAst->pSymRec->name) +
                 S("::sIsRegistered = ComponentRegistry::register_constructor(HASH::") +
                 S(pAst->pSymRec->name) + S(", ") + S(pAst->pSymRec->name) + S("::construct);\n"));
        
        code += I + S("\n} // namespace comp\n ");
        return code;
   }
    case kAST_MessageDef:
    {
        return S("");
    }
    case kAST_PropertyDef:
    {
        auto code = I + type_str(pAst->pSymRec->dataType) + S(" #") + S(pAst->pSymRec->name);
        
        code += S(";\n");
        return S("");
    }
    case kAST_FieldDef:
    {
        return S("");
    }

    case kAST_Block:
    {
        return S("");
    }
    case kAST_FunctionParams:
    {
        return S("");
    }
    case kAST_FunctionCall:
    {
        return S("");
    }
    case kAST_SymbolRef:
    {
        return S("");
    }
    case kAST_SymbolDecl:
    {
        return S("");
    }

    case kAST_If:
    {
        return S("");
    }
    case kAST_While:
    {
        return S("");
    }
    case kAST_DoWhile:
    {
        return S("");
    }
    case kAST_For:
    {
        return S("");
    }
    
    case kAST_Add:
    {
        return S("");
    }
    case kAST_Sub:
    {
        return S("");
    }
    case kAST_Mul:
    {
        return S("");
    }
    case kAST_Div:
    {
        return S("");
    }
    case kAST_Mod:
    {
        return S("");
    }
    case kAST_LShift:
    {
        return S("");
    }
    case kAST_RShift:
    {
        return S("");
    }
    case kAST_And:
    {
        return S("");
    }
    case kAST_Or:
    {
        return S("");
    }
    case kAST_XOr:
    {
        return S("");
    }

    case kAST_Eq:
    {
        return S("");
    }
    case kAST_NEq:
    {
        return S("");
    }
    case kAST_LT:
    {
        return S("");
    }
    case kAST_LTE:
    {
        return S("");
    }
    case kAST_GT:
    {
        return S("");
    }
    case kAST_GTE:
    {
        return S("");
    }

    case kAST_Assign:
    {
        return S("");
    }
    case kAST_AddAssign:
    {
        return S("");
    }
    case kAST_SubAssign:
    {
        return S("");
    }
    case kAST_MulAssign:
    {
        return S("");
    }
    case kAST_DivAssign:
    {
        return S("");
    }
    case kAST_ModAssign:
    {
        return S("");
    }
    case kAST_LShiftAssign:
    {
        return S("");
    }
    case kAST_RShiftAssign:
    {
        return S("");
    }
    case kAST_AndAssign:
    {
        return S("");
    }
    case kAST_XorAssign:
    {
        return S("");
    }
    case kAST_OrAssign:
    {
        return S("");
    }

    case kAST_Not:
    {
        return S("");
    }
    case kAST_Complement:
    {
        return S("");
    }
    case kAST_Negate:
    {
        return S("");
    }

    case kAST_PreInc:
    {
        return S("");
    }
    case kAST_PreDec:
    {
        return S("");
    }
    case kAST_PostInc:
    {
        return S("");
    }
    case kAST_PostDec:
    {
        return S("");
    }

    case kAST_IntLiteral:
    {
        return S("");
    }
    case kAST_FloatLiteral:
    {
        return S("");
    }
    case kAST_StringLiteral:
    {
        return S("");
    }

    case kAST_Identifier:
    {
        return S("");
    }
    case kAST_PropertySet:
    {
        return S("");
    }
    case kAST_MessageSend:
    {
        return S("");
    }

    default:
        PANIC("codegen_recurse invalid AstType: %d", pAst->type);
        return S("");
    }
}

static void extract_filenames(const S & cmpFullPath,
                              CodeCpp & codeCpp)
{
    codeCpp.cmpFullPath = cmpFullPath;

    if (cmpFullPath.substr(cmpFullPath.size() - 4) != S(".cmp"))
        PANIC("Compose filename doesn't have .cmp extension: %s", cmpFullPath.c_str());

    S scriptsCompose("/scripts/cmp/");

    size_t scriptsComposePos = cmpFullPath.rfind(scriptsCompose);
    if (scriptsComposePos == S::npos)
        PANIC("Compose filename doesn't contain .../scripts/cmp/... directory: %s", cmpFullPath.c_str());

    size_t lastSlashPos = cmpFullPath.rfind('/');

    S relPath = cmpFullPath.substr(scriptsComposePos + scriptsCompose.size(), lastSlashPos - (scriptsComposePos + scriptsCompose.size()) + 1);

    codeCpp.cmpFilename = cmpFullPath.substr(cmpFullPath.rfind('/') + 1);
    // already ends with .cmp
    codeCpp.cppFilename = codeCpp.cmpFilename;
    codeCpp.cppFilename[codeCpp.cppFilename.size() - 2] = 'p';
    codeCpp.cppFilename[codeCpp.cppFilename.size() - 1] = 'p';

    codeCpp.cmpRelPath = relPath + codeCpp.cmpFilename;
    codeCpp.cppRelPath = relPath + codeCpp.cppFilename;
    
    codeCpp.cppFullPath = cmpFullPath.substr(0, scriptsComposePos);
    codeCpp.cppFullPath += "/scripts/cpp/";
    codeCpp.cppFullPath += codeCpp.cppRelPath;
}
                       

CodeCpp codegen_cpp(const ParseData * pParseData)
{
    ASSERT(pParseData);

    CodeCpp codeCpp;

    extract_filenames(pParseData->fullPath, codeCpp);
    
    codeCpp.code = S("");

    codeCpp.code += S("#include \"engine/stdafx.h\"\n");
    codeCpp.code += LF;
    codeCpp.code += S("#include \"engine/Registry.h\"\n");
    codeCpp.code += S("#include \"engine/Component.h\"\n");
    codeCpp.code += LF;

    codeCpp.code += S("namespace gaen\n{\n\n");

    auto codeDefs = S("");
    for (Ast * pAst : pParseData->pRootAst->pChildren->nodes)
    {
        // only process top level AST nodes from the primary file
        if (pAst->fullPath == pParseData->fullPath)
        {
            codeDefs += codegen_recurse(pAst, 0) + S("\n");
        }
    }

    codeCpp.code += codeDefs;

    codeCpp.code += S("} // namespace gaen\n");

    return codeCpp;
}

} // namespace gaen
