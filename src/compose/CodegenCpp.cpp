//------------------------------------------------------------------------------
// CodegenCpp.cpp - C++ code generation
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#include <algorithm>

#include "core/thread_local.h"
#include "core/List.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
#include "hashes/hashes.h"
#include "compose/utils.h"
#include "compose/CodegenCpp.h"

namespace gaen
{

#define LF S("\n")
#define I indent(indentLevel)
#define I1 indent(indentLevel+1)
#define I2 indent(indentLevel+2)
#define I3 indent(indentLevel+3)

S CodegenCpp::indent(u32 level)
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

const char * CodegenCpp::cellFieldStr(const SymDataType * pSdt, ParseData * pParseData)
{
    switch (pSdt->typeDesc.dataType)
    {
    case kDT_int:
    case kDT_entity:
        return "i";
    case kDT_float:
        return "f";
    case kDT_bool:
        return "b";
    case kDT_color:
        return "color";
    default:
        COMP_ERROR(pParseData, "cellFieldStr invalid DataType: %d", pSdt->typeDesc.dataType);
        return "";
    }
}

void CodegenCpp::encodeString(char * enc, size_t encSize, const char * str)
{
    char * encMax = enc + encSize;
    char * d = enc;
    const char * s = str;

    *d++ = '"';

    while (*s)
    {
        if (d >= encMax-1)
            break;
        switch (*s)
        {
        case 0x07: // Alarm (Beep, Bell)
            *d++ = '\\';
            *d = 'a';
            break;
        case 0x08: // Backspace
            *d++ = '\\';
            *d = 'b';
            break;
        case 0x0c: // Formfeed
            *d++ = '\\';
            *d = 'f';
            break;
        case 0x0a: // Newline
            *d++ = '\\';
            *d = 'n';
            break;
        case 0x0d: // Carriage Return
            *d++ = '\\';
            *d = 'r';
            break;
        case 0x09: // Horizontal Tab
            *d++ = '\\';
            *d = 't';
            break;
        case 0x0B: // Vertical Tab
            *d++ = '\\';
            *d = 'v';
            break;
        case '\\': // Backslash
            *d++ = '\\';
            *d = '\\';
            break;
        case '"': // Double quotation mark
            *d++ = '\\';
            *d = '"';
            break;
        default:
            *d = *s;
            break;
        }
        s++;
        d++;
    }

    *d++ = '"';
    *d = '\0';
}

S CodegenCpp::propertyBlockAccessor(const SymDataType * pSdt, const BlockInfo & blockInfo, const char * blockVarName, bool isConst, ParseData * pParseData)
{
    static const u32 kScratchSize = 255;
    char scratch[kScratchSize+1];
    scratch[kScratchSize] = '\0';

    switch (pSdt->typeDesc.dataType)
    {
    case kDT_int:
    case kDT_float:
    case kDT_bool:
    case kDT_color:
    case kDT_entity:
        snprintf(scratch, kScratchSize, "%s[%u].cells[%u].%s", blockVarName, blockInfo.blockIndex, blockInfo.cellIndex, cellFieldStr(pSdt, pParseData));
        return S(scratch);
    case kDT_vec3:
    case kDT_vec4:
    case kDT_ivec3:
    case kDT_ivec4:
    case kDT_quat:
    case kDT_mat3:
    case kDT_mat43:
    case kDT_mat4:
        ASSERT(blockInfo.cellIndex == 0);
        if (!isConst)
            snprintf(scratch, kScratchSize, "*reinterpret_cast<%s*>(&%s[%u].qCell)", pSdt->cppTypeStr, blockVarName, blockInfo.blockIndex);
        else
            snprintf(scratch, kScratchSize, "*reinterpret_cast<const %s*>(&%s[%u].qCell)", pSdt->cppTypeStr, blockVarName, blockInfo.blockIndex);
        return S(scratch);
    case kDT_vec2:
    case kDT_ivec2:
    case kDT_string:
    case kDT_asset:
    case kDT_handle:
    case kDT_asset_handle:
        if (!isConst)
            snprintf(scratch, kScratchSize, "*reinterpret_cast<%s*>(&%s[%u].cells[%u])", pSdt->cppTypeStr, blockVarName, blockInfo.blockIndex, blockInfo.cellIndex);
        else
            snprintf(scratch, kScratchSize, "*reinterpret_cast<const %s*>(&%s[%u].cells[%u])", pSdt->cppTypeStr, blockVarName, blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    default:
        COMP_ERROR(pParseData, "Invalid dataType: %d", pSdt->typeDesc.dataType);
        return S("");
    }
}

S CodegenCpp::messageBlockAccessor(const SymDataType * pSdt, const BlockInfo & blockInfo, const char * blockVarName, bool isConst, ParseData * pParseData)
{
    static const u32 kScratchSize = 255;
    char scratch[kScratchSize+1];
    scratch[kScratchSize] = '\0';

    if (blockInfo.isPayload)
    {
        snprintf(scratch,
                 kScratchSize,
                 "%s.message().payload.%s",
                 blockVarName,
                 cellFieldStr(pSdt, pParseData));
        return S(scratch);
    }
    else
    {
        if (pSdt->cellCount < kCellsPerBlock)
        {
            snprintf(scratch, kScratchSize, "%s.extract<%s>(%u, %u)", blockVarName, pSdt->cppTypeStr, blockInfo.blockIndex, blockInfo.cellIndex);
            return S(scratch);
        }
        else // 4 cells or more
        {
            ASSERT(blockInfo.cellIndex == 0);
            snprintf(scratch, kScratchSize, "%s.extract<%s>(%u)", blockVarName, pSdt->cppTypeStr, blockInfo.blockIndex);
            return S(scratch);
        }
    }
}


S CodegenCpp::binaryOp(const Ast * pAst, const char * op)
{
    return S("(") + codegenRecurse(pAst->pLhs, 0) + S(" ") + S(op) + S(" ") + codegenRecurse(pAst->pRhs, 0) + S(")");
}

S CodegenCpp::unaryOp(const Ast * pAst, const char* op)
{
    return S(op) + S("(") + codegenRecurse(pAst->pRhs, 0) + S(")");
}

S CodegenCpp::unaryOpPost(const Ast * pAst, const char* op)
{
    return S("(") + codegenRecurse(pAst->pRhs, 0) + S(")") + S(op);
}

S CodegenCpp::assign(const Ast * pAst, const char * op)
{
    if (!pAst->pSymRecRef)
    {
        COMP_ERROR(pAst->pParseData, "Unknown symbol: %s", pAst->str);
        return S("");
    }


    if (pAst->pSymRecRef->type == kSYMT_Local || !is_block_memory_type(pAst->pSymRecRef->pSymDataType))
    {
        return symref(pAst, pAst->pSymRecRef, pAst->pParseData) + S(" ") + S(op) + S(" ") + codegenRecurse(pAst->pRhs, 0);
    }
    else
    {
        if (strcmp(op, "=") != 0)
        {
            COMP_ERROR(pAst->pParseData, "Invalid assignment op %s for dataType %d", op, pAst->pSymRecRef->pSymDataType->typeDesc.dataType);
            return S("");
        }
        // call set function for ref counted types so addref/release can be done properly
        return S("pThis->set_") + S(pAst->pSymRecRef->name) + S("(") + codegenRecurse(pAst->pRhs, 0) + S(")");
    }
}

S CodegenCpp::messageParamSymref(SymRec * pSymRec)
{
    static const u32 kScratchSize = 256;
    char scratch[kScratchSize+1];

    S code;
    ASSERT(pSymRec->type == kSYMT_MessageParam);

    const BlockInfo * pBlockInfo = pSymRec->pAst->pBlockInfos->find(pSymRec->pAst);
    if (!pBlockInfo->isBlockMemoryType)
    {
        if (!pSymRec->pStructSymRec)
        {
            code = S("/*") + S(pSymRec->name) + S("*/") + messageBlockAccessor(ast_data_type(pSymRec->pAst), *pBlockInfo, "msgAcc", true, pSymRec->pAst->pParseData);
        }
        else
        {
            // we're dealing with a field within a struct (like v.x in a vec3, for example)
            const char * fieldName = strrchr(pSymRec->pAst->str, '.');
            ASSERT(fieldName != nullptr);
            const BlockInfo * pStructBlockInfo = pSymRec->pAst->pBlockInfos->find(pSymRec->pStructSymRec->pAst);
            code = S("/*") + S(pSymRec->name) + S("*/(");
            code += messageBlockAccessor(ast_data_type(pSymRec->pStructSymRec->pAst), *pStructBlockInfo, "msgAcc", true, pSymRec->pAst->pParseData);
            code += S(")") + S(fieldName);
        }
    }
    else
    {
        switch (pBlockInfo->pSymDataType->typeDesc.dataType)
        {
        case kDT_string:
            ASSERT(pBlockInfo->blockMemoryIndex != -1);

            snprintf(scratch,
                     kScratchSize,
                     "pThis->self().blockMemory().stringReadMessage(msgAcc, %u, %u)",
                     pSymRec->pAst->pBlockInfos->blockCount,
                     pBlockInfo->blockMemoryIndex);
            code = S(scratch);
            break;
        default:
            PANIC("Unsupported DataType: %u", pBlockInfo->pSymDataType->typeDesc.dataType);
            code = S("");
            break;
        }
    }

    return code;
}

S CodegenCpp::symref(const Ast * pAst, SymRec * pSymRec, ParseData * pParseData)
{
    if (!pSymRec)
    {
        // We shouldn't get here, as this error should have been
        // handled/reported before symref is called, but here for
        // safety.
        COMP_ERROR(pParseData, "Null symrec");
        return S("");
    }

    S code;

    if (pSymRec->type == kSYMT_MessageParam)
    {
        // Just refer by name since MessageParams are extracted into local variables in message handler block
        code += S(pSymRec->name);
    }
    else if (pSymRec->type == kSYMT_Property || pSymRec->type == kSYMT_Field)
    {
        if (pSymRec->flags & kSRFL_Member)
        {
            const char * dot = strchr(pSymRec->name, '.');
            if (dot)
            {
                code = S("pThis->") + S(pSymRec->name, dot - pSymRec->name) + S("()") + S(dot);
                if (pSymRec->flags & kSRFL_NeedsCppParens)
                    code += S("()");
            }
            else
                code = S("pThis->") + S(pSymRec->name) + S("()");
        }
        else
        {
            code = S("pThis->") + S(pSymRec->name) + S("()");
        }
    }
    else if (pSymRec->type == kSYMT_GlobalConst)
    {
        code = S("compose_globals::") + S(pSymRec->fullName);
    }
    else
    {
        PANIC_IF(!pAst, "Null pAst, invalid for this case");
        if (pAst->pLhs)
        {
            PANIC_IF(pAst->pLhs->type != kAST_DottedId, "Only dotted_id are valid here");
            code = S(pAst->pLhs->str);
            if (pSymRec->flags & kSRFL_NeedsCppParens)
                code += S("()"); // properties and fields are accessed from mpBlocks using generated private accessors
        }
    }
    return code;
}

S CodegenCpp::hashLiteral(const char * str)
{
    ASSERT(str);

    mHashes.emplace(str);
    static const u32 kScratchSize = 255;
    char scratch[kScratchSize+1];
    u32 hashVal = HASH::hash_func(str);
    snprintf(scratch, kScratchSize, "0x%08x/*#%s,%u*/", hashVal, str, hashVal);
    scratch[kScratchSize] = '\0';

    return S(scratch);
}

S CodegenCpp::floatLiteral(f32 val)
{
    static const u32 kScratchSize = 32;
    char scratch[kScratchSize+1];
    snprintf(scratch, kScratchSize, "%1.8ef", val);
    return S(scratch);
}

S CodegenCpp::setPropertyHandlers(const Ast * pAst, int indentLevel)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    bool hasProperty = false;

    for (const auto & kv : pAst->pScope->pSymTab->dict)
    {
        SymRec * pSymRec = kv.second;
        if (is_prop(pSymRec))
        {
            hasProperty = true;
            break;
        }
    }

    S code = S("");

    if (hasProperty)
    {
        code += I + S("case HASH::set_property:\n");
        code += I1 + S("switch (_msg.payload.u)\n");
        code += I1 + S("{\n");
        for (const Ast * pPropAst : pAst->pChildren->nodes)
        {
            SymRec * pSymRec = pPropAst->pSymRec;
            if (is_prop(pSymRec))
            {
                ASSERT(pPropAst->type == kAST_PropertyDef);
                static const u32 kScratchSize = 128;
                char scratch[kScratchSize+1];

                u32 cellCount = pSymRec->pSymDataType->cellCount;
                u32 blockCount = block_count(cellCount);

                code += I1 + S("case ") + hashLiteral(pSymRec->name) + S(":\n");
                code += I1 + S("{\n");
                if (!is_block_memory_type(pSymRec->pSymDataType))
                {
                    snprintf(scratch, kScratchSize, "u32 requiredBlockCount = %d;\n", blockCount);
                    code += I2 + S(scratch);
                    code += I2 + S("if (_msg.blockCount >= requiredBlockCount)\n");
                    code += I2 + S("{\n");

                    u32 fullBlocksToCopy = blockCount;
                    if (cellCount % kCellsPerBlock != 0)
                        fullBlocksToCopy--;

                    code += I3 + S("// set tempVal to what was sent in the message") + LF;
                    code += I3 + S(pSymRec->pSymDataType->cppTypeStr) + S(" tempVal;") + LF;

                    // copy full blocks
                    for (u32 i = 0; i < fullBlocksToCopy; ++i)
                    {
                        snprintf(scratch,
                                 kScratchSize,
                                 "reinterpret_cast<Block*>(&tempVal)[%d] = msgAcc[%d];\n",
                                 i,
                                 i);
                        code += I3 + S(scratch);
                    }
                    // copy remaining cells from last block
                    if (fullBlocksToCopy < blockCount)
                    {
                        u32 lastBlockIdx = blockCount - 1;
                        u32 cellsToCopy = cellCount % kCellsPerBlock;
                        for (u32 i = 0; i < cellsToCopy; ++i)
                        {
                            snprintf(scratch,
                                     kScratchSize,
                                     "reinterpret_cast<Block*>(&tempVal)[%d].cells[%d] = msgAcc[%d].cells[%d];\n",
                                     lastBlockIdx,
                                     i,
                                     lastBlockIdx,
                                     i);
                            code += I3 + S(scratch);
                        }
                    }
                    if (pPropAst->pLhs)
                    {
                        code += I3 + S("// call 'pre' before property is changed") + LF;
                        code += I3 + S(pSymRec->name) + S("__pre(tempVal);") + LF;
                    }
                    code += I3 + S("// copy tempVal into actual property") + LF;
                    code += I3 + S(pSymRec->name) + S("() = tempVal;") + LF;
                    if (pPropAst->pMid)
                    {
                        code += I3 + S("// call 'post' now that property is changed") + LF;
                        code += I3 + S(pSymRec->name) + S("__post(") + S(pSymRec->name) + S("());") + LF;
                    }

                    code += I3 + S("return MessageResult::Consumed;\n");
                    code += I2 + S("}\n");
                    code += I2 + S("break;\n");
                    code += I1 + S("}\n");
                }
                else // block memory type
                {
                    // This is a BlockMemory type, like a string, list, or dict.
                    // Size is dynamic and must be read from first block.
                    code += I2 + S("if (_msg.blockCount < 1) break; // not enough even for BlockData header\n");
                    code += I2 + S("const BlockData * pBlockData = reinterpret_cast<const BlockData*>(&msgAcc[0]);\n");
                    code += I2 + S("if (pBlockData->type != ") + S(compose_type_to_block_type(pSymRec->pSymDataType->typeDesc.dataType)) + S(") break; // incorrect BlockData type\n");
                    code += I2 + S("u32 requiredBlockCount = pBlockData->blockCount;\n");
                    code += I2 + S("if (_msg.blockCount >= requiredBlockCount)\n");
                    code += I2 + S("{\n");
                    code += I2 + S("    Address addr = pThis->self().blockMemory().allocCopy(pBlockData);\n");
                    code += I2 + S("    set_") + S(pSymRec->name) + S("(pThis->self().blockMemory().string(addr));\n");
                    code += I2 + S("    return MessageResult::Consumed;\n");
                    code += I2 + S("}\n");
                    code += I2 + S("break;\n");
                    code += I1 + S("}\n");
                }
            }
        }
        code += I1 + S("}\n");
        code += I1 + S("return MessageResult::Propagate; // Invalid property\n");
    }
    return code;
}

S CodegenCpp::dataTypeInitValue(const SymDataType * pSdt, ParseData * pParseData)
{
    switch (pSdt->typeDesc.dataType)
    {
    case kDT_int:
        return S("0");
    case kDT_float:
        return S("0.0f");
    case kDT_bool:
        return S("false");
    case kDT_vec2:
        return S("vec2(0.0f, 0.0f)");
    case kDT_vec3:
        return S("vec3(0.0f, 0.0f, 0.0f)");
    case kDT_vec4:
        return S("vec4(0.0, 0.0f, 0.0f, 1.0f)");
    case kDT_ivec2:
        return S("ivec2(0.0f, 0.0f)");
    case kDT_ivec3:
        return S("ivec3(0.0f, 0.0f, 0.0f)");
    case kDT_ivec4:
        return S("ivec4(0.0, 0.0f, 0.0f, 1.0f)");
    case kDT_quat:
        return S("quat(0.0, 0.0f, 0.0f, 1.0f)");
    case kDT_mat3:
        return S("mat3a(1.0f)");
    case kDT_mat43:
        return S("mat43(1.0f)");
    case kDT_mat4:
        return S("mat4(1.0f)");
    case kDT_handle:
    case kDT_asset_handle:
        return S("nullptr");
    case kDT_string:
    case kDT_asset:
        return S("pThis->self().blockMemory().stringAlloc(\"\")");
    case kDT_entity:
        return S("-1");
    default:
        COMP_ERROR(pParseData, "Unknown initial value for datatype: %d", pSdt->typeDesc.dataType);
        return S("");
    }
}

S CodegenCpp::initData(const Ast * pAst, int indentLevel, ScriptDataCategory dataCategory)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    S code = S("");
    for (const Ast * pChild : pAst->pChildren->nodes)
    {
        SymRec * pSymRec = pChild->pSymRec;
        ScriptDataCategory cat = data_category(pChild);
        if (cat == dataCategory)
        {
            if (!is_block_memory_type(pSymRec->pSymDataType))
            {
                code += I1 + symref(nullptr, pSymRec, pAst->pParseData);
                code += S(" = ");

                // Does the script initialize this with a value?
                if (pSymRec->pInitVal)
                {
                    code += codegenRecurse(pSymRec->pInitVal, 0);
                }
                else
                {
                    // initialize with a default value based on the type
                    code += dataTypeInitValue(pSymRec->pSymDataType, pAst->pParseData);
                }
                code += S(";\n");
            }
            else // ref counted go through set methods so we can addref/release properly
            {
                code += I + S("    set_") + pSymRec->name + S("(");

                // Does the script initialize this with a value?
                if (pSymRec->pInitVal)
                {
                    code += codegenRecurse(pSymRec->pInitVal, 0);
                }
                else
                {
                    // initialize with a default value based on the type
                    code += dataTypeInitValue(pSymRec->pSymDataType, pAst->pParseData);
                }
                code += S(");\n");
            }
        }
    }
    return code;
}

S CodegenCpp::initAssets(const Ast * pAst, int indentLevel)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    S code = S("");
    for (SymRec * pSymRec : pAst->pScope->pSymTab->orderedSymRecs)
    {
        if (is_prop_or_field(pSymRec) &&
            pSymRec->pSymDataType->typeDesc.dataType == kDT_asset)
        {
            const char * handleName = asset_handle_name(pSymRec->name);
            code += I + S("self().requestAsset(mScriptTask.id(), ") + hashLiteral(handleName) + S(", ") + S(pSymRec->name) + S("());\n");
        }
    }
    return code;
}

S CodegenCpp::assetReady(const Ast * pAst, int indentLevel)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    // check if there are any asset properties or fields
    bool hasAssets = false;
    for (SymRec * pSymRec : pAst->pScope->pSymTab->orderedSymRecs)
    {
        if (is_prop_or_field(pSymRec) && pSymRec->pSymDataType->typeDesc.dataType == kDT_asset)
        {
            hasAssets = true;
            break;
        }
    }

    S code = S("");

    if (hasAssets)
    {
        code += I + S("{\n");
        code += I + S("    messages::HandleR<T> msgr(msgAcc);\n");
        code += I + S("    switch (msgr.nameHash())\n");
        code += I + S("    {\n");
        for (SymRec * pSymRec : pAst->pScope->pSymTab->orderedSymRecs)
        {
            if (is_prop_or_field(pSymRec) && pSymRec->pSymDataType->typeDesc.dataType == kDT_asset)
            {
                const char * handleName = asset_handle_name(pSymRec->name);
                code += I + S("    case ") + hashLiteral(handleName) + S(":\n");
                code += I + S("        ") + S(handleName) + ("() = msgr.handle();\n");
                code += I + S("        break;\n");
            }
        }

        code += I + S("    default:\n");
        code += I + S("        ERR(\"Invalid asset nameHash: %u\", msgr.nameHash());\n");
        code += I + S("        break;\n");

        code += I + S("    }\n");
        code += I + S("}\n");
    }
    return code;
}

S CodegenCpp::fin(const Ast * pAst, int indentLevel)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    S code = S("");
    for (SymRec * pSymRec : pAst->pScope->pSymTab->orderedSymRecs)
    {
        if (is_prop_or_field(pSymRec))
        {
            if (is_block_memory_type(pSymRec->pSymDataType) ||
                pSymRec->pSymDataType->typeDesc.dataType == kDT_asset_handle ||
                pSymRec->pSymDataType->typeDesc.dataType == kDT_handle)
            {
                code += I + S(pSymRec->name) + S("__release();") + LF;
            }
        }
    }
    return code;
}

S CodegenCpp::initializationMessageHandlers(const Ast * pAst, const S& postInit, const S& postInitIndependent, const S& postInitDependent, int indentLevel)
{
    S code;

    // init__
    code += I + S("        case HASH::init__:\n");
    code += I + S("        {\n");
    code += I + S("            // Initialize asset properties and fields to default values\n");
    code += initData(pAst, indentLevel + 2, kSDC_Asset);
    code += postInit;
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::init__\n");

    // request_assets__
    code += I + S("        case HASH::request_assets__:\n");
    code += I + S("        {\n");
    code += I + S("            // Request any assets we require\n");
    code += initAssets(pAst, indentLevel + 3);
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::request_assets__\n");

    // asset_ready__
    code += I + S("        case HASH::asset_ready__:\n");
    code += I + S("        {\n");
    code += I + S("            // Asset is loaded and a handle to it has been sent to us\n");
    code += assetReady(pAst, indentLevel + 3);
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::assets_ready__\n");

    // init_independent_data__
    code += I + S("        case HASH::init_independent_data__:\n");
    code += I + S("        {\n");
    code += I + S("            // Initialize non-asset properties to default values\n");
    code += initData(pAst, indentLevel + 2, kSDC_Independent);
    code += postInitIndependent;
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::init_independent_data__\n");

    // init_dependent_data__
    code += I + S("        case HASH::init_dependent_data__:\n");
    code += I + S("        {\n");
    code += I + S("            // Initialize non-asset fields to default values\n");
    code += initData(pAst, indentLevel + 2, kSDC_Dependent);
    code += postInitDependent;
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::init_dependent_data__\n");

    // fin__
    code += I + S("        case HASH::fin__:\n");
    code += I + S("        {\n");
    code += I + S("            // Release any block data or handle fields and properties\n");
    code += fin(pAst, indentLevel + 3);
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::fin__\n");

    // transform
    code += I + S("        // LORRNOTE: Handle transform here as well (already handled in Entity) for convenience\n");
    code += I + S("        case HASH::transform:\n");
    code += I + S("        {\n");
    code += I + S("            messages::TransformR<T> msgr(msgAcc);\n");
    code += I + S("            pThis->self().applyTransform(msgAcc.message().source, msgr.isLocal(), msgr.transform());\n");
    code += I + S("            return MessageResult::Consumed;\n");
    code += I + S("        } // HASH::transform\n");

    return code;
}

S CodegenCpp::messageDef(const Ast * pAst, int indentLevel)
{
    S code;

    static const u32 kScratchSize = 256;
    char scratch[kScratchSize+1];

    if (pAst->type == kAST_MessageDef &&
        pAst->pSymRec)
    {
        code += I + S("case ") + hashLiteral(pAst->str) + S(":\n");
        code += I + S("{\n");

        bool isInit = 0 == strcmp(pAst->str, "init");
        bool isFin = 0 == strcmp(pAst->str, "fin");

        if (pAst->pBlockInfos->blockCount > 0 ||
            pAst->pBlockInfos->blockMemoryItemCount > 0)
        {
            code += I1 + S("// Verify params look compatible with this message type\n");
            snprintf(scratch,
                     kScratchSize,
                     "u32 expectedBlockSize = %d; // BlockCount without BlockMemory params\n",
                     pAst->pBlockInfos->blockCount);
            code += I1 + S(scratch);
        }
        if (pAst->pBlockInfos->blockCount > 0)
        {
            code += I1 + S("if (expectedBlockSize > msgAcc.available())\n");
            code += I1 + S("    return MessageResult::Propagate;\n");
            code += LF;
        }

        if (pAst->pBlockInfos->blockMemoryItemCount > 0)
        {
            code += I1 + S("// Check that block memory params exist in the message\n");
            code += I1 + S("u16 blockMemCount = 0;\n");

            code += LF;
            for (const BlockInfo & bi : pAst->pBlockInfos->items)
            {
                if (bi.isBlockMemoryType)
                {
                    snprintf(scratch,
                             kScratchSize,
                             "blockMemCount = BlockData::validate_block_data(&msgAcc[expectedBlockSize], %s);\n",
                             compose_type_to_block_type(bi.pSymDataType->typeDesc.dataType));
                    code += I1 + S(scratch);
                    code += I1 + S("expectedBlockSize += blockMemCount;\n");

                    code += I1 + S("if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())\n");
                    code += I1 + S("    return MessageResult::Propagate;\n");
                    code += LF;
                }
            }
            code += LF;
        }

        if (pAst->pBlockInfos->items.size() > 0)
        {
            code += I1 + S("// Params look compatible, extract into local variables") + LF;
            for (const BlockInfo & bi : pAst->pBlockInfos->items)
            {
                if (!(bi.pAst->pSymRec->flags & kSRFL_Member))
                {
                    code += I1 + S(bi.pSymDataType->cppTypeStr) + " " + S(bi.pAst->str) + S(" = ") + messageParamSymref(bi.pAst->pSymRec) + S(";") + LF;
                }
            }
            code += LF;
        }


        code += I1 + S("// Message body follows\n");
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            code += codegenRecurse(pChild, indentLevel + 1);
        }

        code += I + S("    return MessageResult::Consumed;\n");
        code += I + S("} // #") + S(pAst->str) + S("\n");
    }

    return code;
}

S CodegenCpp::codegenInitProperties(Ast * pAst, SymTab * pPropsSymTab, const char * taskName, const char * scriptTaskName, ScriptDataCategory dataCategory, int indentLevel)
{
    static const u32 kScratchSize = 256;
    char scratch[kScratchSize+1];

    S code = S("");
    if (pAst && pAst->pChildren)
    {
        for (Ast * pPropInit : pAst->pChildren->nodes)
        {
            if (pPropInit->type == kAST_TransformInit)
            {
                if (dataCategory == kSDC_Independent)
                {
                    code += I + S("// Init transform:\n");
                    code += I + S("{\n");
                    snprintf(scratch,
                             kScratchSize,
                             "    messages::TransformBW msgw(HASH::transform, kMessageFlag_None, %s.id(), %s.id(), mpEntity->hasParent());\n",
                             scriptTaskName,
                             taskName);
                    code += I + S(scratch);
                    code += I + S("    msgw.setTransform(") + codegenRecurse(pPropInit->pRhs, 0) + S(");") + LF;
                    code += I + S("    ") + S(taskName) + S(".message(msgw.accessor());") + LF;
                    code += I + S("}\n");
                }
            }
            else if (pPropInit->type == kAST_ReadyInit)
            {
                // This is a no-op. Calling codegen_ready_message utilizes the ready initializer
            }
            else if (pPropInit->type == kAST_ParentInit)
            {
                // This is a no-op. Calling codegen_ready_message utilizes the ready initializer
            }
            else
            {
                // Ensure the property is valid
                const char * propName = pPropInit->str;

                SymRec * pSymRec = symtab_find_symbol(pPropsSymTab, propName);

                if (!pSymRec)
                    COMP_ERROR(pAst->pParseData, "Unknown property: %s", propName);

                ScriptDataCategory cat = data_category(pSymRec->pAst);
                if (cat == dataCategory)
                {
                    if (pSymRec && pSymRec->type == kSYMT_Field && pSymRec->pSymDataType->typeDesc.dataType == kDT_asset_handle)
                    {
                        propName = asset_path_name(pPropInit->str);
                        pSymRec = symtab_find_symbol(pPropsSymTab, propName);
                    }

                    if (!pSymRec || pSymRec->type != kSYMT_Property)
                    {
                        COMP_ERROR(pAst->pParseData, "Invalid property: '%s'", pPropInit->str);
                    }

                    code += I + S("// Init Property: ") + S(pPropInit->str) + ("\n");
                    code += I + S("{\n");
                    const SymDataType * pRhsSdt = ast_data_type(pPropInit->pRhs);

                    if (!is_block_memory_type(pRhsSdt) && pRhsSdt->typeDesc.dataType != kDT_asset_handle)
                    {
                        u32 valCellCount = pRhsSdt->cellCount;
                        u32 blockCount = block_count(valCellCount);
                        snprintf(scratch,
                                 kScratchSize,
                                 "    StackMessageBlockWriter<%u> msgw(HASH::set_property, kMessageFlag_None, %s.id(), %s.id(), to_cell(%s));\n",
                                 blockCount,
                                 scriptTaskName,
                                 taskName,
                                 hashLiteral(propName).c_str());

                        code += I + S(scratch);
                        DataType dt = pRhsSdt->typeDesc.dataType;
                        switch (dt)
                        {
                        case kDT_float:
                            code += I + S("    msgw[0].cells[0].f = ") + codegenRecurse(pPropInit->pRhs, 0);
                            break;
                        case kDT_int:
                        case kDT_entity:
                            code += I + S("    msgw[0].cells[0].i = ") + codegenRecurse(pPropInit->pRhs, 0);
                            break;
                        case kDT_color:
                            code += I + S("    msgw[0].cells[0].color = ") + codegenRecurse(pPropInit->pRhs, 0);
                            break;
                        case kDT_bool:
                            code += I + S("    msgw[0].cells[0].b = ") + codegenRecurse(pPropInit->pRhs, 0);
                            break;
                        case kDT_vec2:
                        case kDT_vec3:
                        case kDT_vec4:
                        case kDT_ivec2:
                        case kDT_ivec3:
                        case kDT_ivec4:
                        case kDT_quat:
                        case kDT_mat3:
                        case kDT_mat43:
                        case kDT_mat4:
                            code += I + S("    *reinterpret_cast<") + S(pRhsSdt->cppTypeStr) + S("*>(&msgw[0].cells[0].u) = ") + codegenRecurse(pPropInit->pRhs, 0);
                            break;
                        default:
                            COMP_ERROR(pAst->pParseData, "Unsupported type for codegen component property init, type: %d", pPropInit->pRhs->type);
                        }
                        code += S(";\n");
                    }
                    else // a BlockMemory type
                    {
                        static const u32 kScratchSize = 256;
                        char scratch[kScratchSize+1];

                        if (pRhsSdt->typeDesc.dataType != kDT_asset_handle)
                        {
                            snprintf(scratch,
                                     kScratchSize,
                                     "    %s val = %s;\n",
                                     pRhsSdt->cppTypeStr,
                                     codegenRecurse(pPropInit->pRhs, 0).c_str());
                        }
                        else
                        {
                            // In the case of passing assets, we
                            // actually just want to pass the asset
                            // path so that ref counting and such is
                            // taken into account when target loads
                            // the asset normally.
                            snprintf(scratch,
                                     kScratchSize,
                                     "    CmpStringAsset val = %s__path;\n",
                                     codegenRecurse(pPropInit->pRhs, 0).c_str());
                        }
                        code += I + S(scratch);

                        snprintf(scratch,
                                 kScratchSize,
                                 "    ThreadLocalMessageBlockWriter msgw(HASH::set_property, kMessageFlag_None, %s.id(), %s.id(), to_cell(%s), val.blockCount());\n",
                                 scriptTaskName,
                                 taskName,
                                 hashLiteral(propName).c_str());
                        code += I + S(scratch);
                        code += I + S("    val.writeMessage(msgw.accessor(), 0);\n");
                    }
                    code += I + S("    ") + S(taskName) + S(".message(msgw.accessor());\n");
                    code += I + S("}\n");
                }
            }
        }
    }

    return code;
}

S CodegenCpp::codegenInitParentTask(const Ast * pAst)
{
    S code = S("0"); // default to 0 if no parent
    if (pAst && pAst->pChildren)
    {
        for (const Ast * pPropInit : pAst->pChildren->nodes)
        {
            if (pPropInit->type == kAST_ParentInit)
            {
                code = codegenRecurse(pPropInit->pRhs, 0);
            }
        }
    }
    return code;
}

S CodegenCpp::codegenReadyMessage(const Ast * pAst)
{
    S code = S("0"); // default to 0 if no ready message was specified
    if (pAst && pAst->pChildren)
    {
        for (const Ast * pPropInit : pAst->pChildren->nodes)
        {
            if (pPropInit->type == kAST_ReadyInit)
            {
                code = codegenRecurse(pPropInit->pRhs, 0);
            }
        }
    }
    return code;
}

S CodegenCpp::entityInitClass(const char * str)
{
    return S("EntityInit__") + S(str);
}

S CodegenCpp::entityInitFunc(const char * str)
{
    return S("entity_init__") + S(str);
}

const Ast * CodegenCpp::definingParent(const Ast * pAst)
{
    ASSERT(pAst->pScope && pAst->pScope->pSymTab);
    const SymTab * pST = pAst->pScope->pSymTab;

    while (pST != nullptr)
    {
        if (pST->pAst &&
            (pST->pAst->type == kAST_ComponentDef ||
             pST->pAst->type == kAST_EntityDef))
        {
            return pST->pAst;
        }
        else
        {
            pST = pST->pParent;
        }
    }
    return nullptr;
}

const void CodegenCpp::findRefsRecurse(SymTab * pSymTab, const Ast * pAst)
{
    if (pAst)
    {
        if (pAst->type == kAST_SymbolRef)
        {
            if (pAst->pSymRecRef->type == kSYMT_Local ||
                pAst->pSymRecRef->type == kSYMT_Param ||
                pAst->pSymRecRef->type == kSYMT_MessageParam)
            {
                if (!symtab_find_symbol(pSymTab, pAst->pSymRecRef->name))
                {
                    SymRec * pSymRec = symrec_create(pAst->pSymRecRef->type,
                                                     pAst->pSymRecRef->pSymDataType,
                                                     pAst->pSymRecRef->name,
                                                     pAst->pSymRecRef->pAst,
                                                     pAst->pSymRecRef->pInitVal,
                                                     pAst->pParseData);

                    symtab_add_symbol(pSymTab, pSymRec, pSymTab->pParseData);
                }
            }
        }
        findRefsRecurse(pSymTab, pAst->pLhs);
        findRefsRecurse(pSymTab, pAst->pMid);
        findRefsRecurse(pSymTab, pAst->pRhs);
        if (pAst->pChildren)
        {
            for (const Ast * pChild : pAst->pChildren->nodes)
            {
                findRefsRecurse(pSymTab, pChild);
            }
        }
    }
}

const SymTab * CodegenCpp::findClosureSymbols(const Ast * pAst)
{
    if (pAst)
    {
        ASSERT(pAst->type == kAST_EntityInit);
        SymTab * pSymTab = symtab_create(pAst->pParseData);

        // find an references in PropInitList and insert them into our SymTab
        ASSERT(pAst->pRhs && pAst->pRhs->type == kAST_PropInitList);
        if (pAst->pRhs && pAst->pRhs->type == kAST_PropInitList)
        {
            if (pAst->pRhs && pAst->pRhs->pChildren)
            {
                for (const Ast * pChild : pAst->pRhs->pChildren->nodes)
                {
                    findRefsRecurse(pSymTab, pChild->pRhs);
                }
            }
        }

        if (pSymTab->dict.size() > 0)
            return pSymTab;
    }
    return nullptr;
}

S CodegenCpp::closureParams(const SymTab * pClosure)
{
    S code;

    if (pClosure)
    {
        for (const SymRec * pSymRec : pClosure->orderedSymRecs)
        {
            if (!(pSymRec->flags & kSRFL_Member))
            {
                if (pSymRec != pClosure->orderedSymRecs.front())
                    code += S(", ");
                code += S("const ");
                code += S(pSymRec->pSymDataType->cppTypeStr);
                code += S(" & ");
                code += S(pSymRec->name);
            }
        }
    }

    return code;
}

S CodegenCpp::closureArgs(const SymTab * pClosure)
{
    S code;

    if (pClosure)
    {
        for (const SymRec * pSymRec : pClosure->orderedSymRecs)
        {
            if (!(pSymRec->flags & kSRFL_Member))
            {
                if (pSymRec != pClosure->orderedSymRecs.front())
                    code += S(", ");
                code += S(pSymRec->name);
            }
        }
    }

    return code;
}

S CodegenCpp::closureMemberInit(const SymTab * pClosure, u32 indentLevel)
{
    S code;

    if (pClosure)
    {
        for (const SymRec * pSymRec : pClosure->orderedSymRecs)
        {
            if (!(pSymRec->flags & kSRFL_Member))
            {
                code += I + S("  , ") + S(pSymRec->name) + S("(") + S(pSymRec->name) + S(")") + LF;
            }
        }
    }

    return code;
}

S CodegenCpp::closureMemberDecl(const SymTab * pClosure, u32 indentLevel)
{
    S code;

    if (pClosure)
    {
        for (const SymRec * pSymRec : pClosure->orderedSymRecs)
        {
            if (!(pSymRec->flags & kSRFL_Member))
            {
                code += I + S(pSymRec->pSymDataType->cppTypeStr) + S(" ") + S(pSymRec->name) + S(";") + LF;
            }
        }
    }

    return code;
}

S CodegenCpp::codegenHelperFuncsRecurse(const Ast * pAst)
{
    S code = S("");

    if (!pAst)
        return code;

    int indentLevel = 1;

    switch (pAst->type)
    {
    case kAST_EntityInit:
    {
        ASSERT(pAst->pSymRecRef && pAst->pSymRecRef->fullName && pAst->pSymRecRef->pSymTabInternal);
        ASSERT(pAst->pRhs);

        const Ast * pDefiningParent = definingParent(pAst);
        ASSERT(pDefiningParent);

        const SymTab * pClosure = findClosureSymbols(pAst);
        S clParams = closureParams(pClosure);

        // EntityInit class
        code += I + S("class ") + entityInitClass(pAst->str) + S(" : public EntityInit") + LF;
        code += I + S("{") + LF;

        code += I + S("public:") + LF;
        code += I1 + entityInitClass(pAst->str) + S("(") + S(pDefiningParent->pSymRec->fullName) + S(" * pThis, Entity * pEntity");
        if (clParams.size() > 0)
        {
            code += S(", ") + clParams;
        }
        code += S(")") + LF;
        code += I1 + S("  : pThis(pThis)") + LF;
        code += I1 + S("  , mpEntity(pEntity)") + LF;
        code += closureMemberInit(pClosure, indentLevel+1);
        code += I1 + S("{}") + LF;

        code += I1 + S("virtual void init()") + LF;
        code += I1 + S("{") + LF;
        code += codegenInitProperties(pAst->pRhs, pAst->pSymRecRef->pSymTabInternal, "mpEntity->scriptTask()", "pThis->scriptTask()", kSDC_Asset, indentLevel + 2);
        code += I1 + S("}") + LF;

        code += I1 + S("virtual void initIndependentData()") + LF;
        code += I1 + S("{") + LF;
        code += codegenInitProperties(pAst->pRhs, pAst->pSymRecRef->pSymTabInternal, "mpEntity->scriptTask()", "pThis->scriptTask()", kSDC_Independent, indentLevel + 2);
        code += I1 + S("}") + LF;

        code += I1 + S("virtual void initDependentData()") + LF;
        code += I1 + S("{") + LF;
        code += codegenInitProperties(pAst->pRhs, pAst->pSymRecRef->pSymTabInternal, "mpEntity->scriptTask()", "pThis->scriptTask()", kSDC_Dependent, indentLevel + 2);
        code += I1 + S("}") + LF;

        code += I + S("private:") + LF;
        code += I1 + S(pDefiningParent->pSymRec->fullName) + S(" * pThis;") + LF;
        code += I1 + S("Entity * mpEntity;") + LF;
        code += closureMemberDecl(pClosure, indentLevel+1);
        code += I + S("}; // class ") + S(entityInitClass(pAst->str)) + LF;
        code += LF;

        code += I + S("task_id ") + entityInitFunc(pAst->str) + S("(") + clParams + S(")") + LF;
        code += I + S("{") + LF;
        code += I + S("    auto pThis = this;") + LF;
        code += I + S("    Entity * pEntity = get_registry().constructEntity(") + hashLiteral(pAst->pSymRecRef->fullName) + S(", 8, ") + codegenInitParentTask(pAst->pRhs) + S(", self().task().id(), ") + codegenReadyMessage(pAst->pRhs) + S(");") + LF;
        code += I + S("    ") + entityInitClass(pAst->str) + S(" * pEntityInit = GNEW(kMEM_Engine, ") + entityInitClass(pAst->str) + S(", this, pEntity");
        S clArgs = closureArgs(pClosure);
        if (clArgs.size() > 0)
            code += S(", ") + clArgs;
        code += S(");") + LF;
        code += I + S("    pEntity->setEntityInit(pEntityInit);") + LF;
        code += I + S("    task_id tid = pEntity->task().id();") + LF;
        code += I + S("    pEntity->activate();") + LF;
        code += I + S("    return tid;") + LF;
        code += I + S("}") + LF;
        code += LF;

        break;
    }
    default:
        break;
    }

    code += codegenHelperFuncsRecurse(pAst->pLhs);
    code += codegenHelperFuncsRecurse(pAst->pMid);
    code += codegenHelperFuncsRecurse(pAst->pRhs);

    if (pAst->pChildren)
    for (Ast * pChild : pAst->pChildren->nodes)
        code += codegenHelperFuncsRecurse(pChild);

    return code;
}

S CodegenCpp::codegenHelperFuncs(const Ast * pAst)
{
    S entityInits = S("");

    for (Ast * pChild : pAst->pChildren->nodes)
    {
        // only process top level AST nodes from the primary file
        if (pChild->fullPath == pChild->pParseData->fullPath)
        {
            entityInits += codegenHelperFuncsRecurse(pChild);
        }
    }

    if (entityInits.size() > 0)
    {
        entityInits = S("private:\n    // Helper functions and classes\n\n") + entityInits + LF;
    }

    return entityInits;
}

bool CodegenCpp::isTopLevelFunction(const Ast * pFuncAst)
{
    return ((pFuncAst->pParent && pFuncAst->pParent->type == kAST_Root) ||
            (pFuncAst->pSymRec && (pFuncAst->pSymRec->flags & kSRFL_BuiltInFunction)));
}

S CodegenCpp::functionPrototype(const Ast * pFuncAst)
{
    S code = S(pFuncAst->pSymRec->pSymDataType->cppTypeStr) + S(" ") + S(pFuncAst->pSymRec->fullName) + S("(");

    bool isFirst = true;

    for (auto it = pFuncAst->pLhs->pScope->pSymTab->orderedSymRecs.begin();
         it != pFuncAst->pLhs->pScope->pSymTab->orderedSymRecs.end();
         ++it)
    {
        SymRec* pParamSymRec = *it;

        if (!(pParamSymRec->flags & kSRFL_Member))
        {
            if (isFirst)
                isFirst = false;
            else
                code += S(", ");

            code += S(pParamSymRec->pSymDataType->cppTypeStr) + S(" ") + S(pParamSymRec->name);
        }
    }

    if (isTopLevelFunction(pFuncAst))
    {
        // Add in a callback function param to 'self()' so the function body can call system apis
        if (!isFirst)
            code += S(", ");
        code += S("Entity * pThis");
    }

    code += S(")");

    return code;
}

S CodegenCpp::updateDef(const Ast * pUpdateDef, const Ast * pInput, u32 indentLevel)
{
    S code;

    if (pUpdateDef || pInput)
    {
        code += I + S("void update(float delta)\n");
        code += I + S("{\n");
        code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;

        if (pInput)
        {
            code += I1 + S("processInput(delta);") + LF;
            if (pUpdateDef)
                code += LF;
        }

        if (pUpdateDef)
        {
            code += I1 + S("// Update statements") + LF;
            for (Ast * pChild : pUpdateDef->pChildren->nodes)
            {
                code += codegenRecurse(pChild, indentLevel + 1);
            }
        }
        code += I + S("} // update\n");
    }

    return code;
}

S CodegenCpp::inputHandlerName(const char * mode, const char * handler)
{
    return S("input__") + S(mode) + S("__") + S(handler);
}

S CodegenCpp::inputHandlerSpecialName(const char * mode, const char * handler)
{
    return S("input__") + S(mode) + S("__special__") + S(handler);
}

S CodegenCpp::inputBlock(const Ast * pRoot, u32 indentLevel)
{
    S code;

    S procCode;
    procCode += I1 + S("u32 maxInputMatch = 0;") + LF;
    procCode += I1 + S("u32 inputMatch = 0;") + LF;
    procCode += I1 + S("vec4 measure = vec4(0.0f);") + LF;
    procCode += I1 + S("vec4 measureTest = vec4(0.0f);") + LF;
    procCode += I1 + S("void (*pHandler)(DefiningType *, f32, const vec4&) = nullptr;") + LF;
    procCode += I1 + S("f32 repeatDelay = 0.0f;") + LF;
    procCode += LF;

    const Ast * pInput;
    u32 idx = 0;
    u32 compiledCount = 0;
    while (pInput = find_input(pRoot, idx++))
    {
        if (pInput->pChildren->nodes.size() > 0)
        {
            procCode += I1;
            if (compiledCount++ > 0)
                procCode += ("else ");
            procCode += S("if (mode == ") + hashLiteral(pInput->str) + S(")") + LF;
            procCode += I1 + S("{") + LF;

            const Ast * pAnyDef = nullptr;
            const Ast * pNoneDef = nullptr;

            procCode += I2 + S("maxInputMatch = 0;") + LF;
            procCode += I2 + S("inputMatch = 0;") + LF;
            procCode += I2 + S("measure = vec4(0.0f);") + LF;
            procCode += I2 + S("measureTest = vec4(0.0f);") + LF;
            procCode += I2 + S("pHandler = nullptr;") + LF;
            procCode += LF;

            u32 paramIdx = 0;
            for (const Ast * pInputDef : pInput->pChildren->nodes)
            {
                ASSERT(pInputDef->type == kAST_InputDef);

                code += I + S("static void ");
                if (pInputDef->pSymRec->type == kSYMT_Input)
                {
                    code += inputHandlerName(pInput->str, pInputDef->str);
                }
                else
                {
                    ASSERT(pInputDef->pSymRec->type == kSYMT_InputSpecial);
                    if (0 == strcmp(pInputDef->str, "any"))
                    {
                        if (pAnyDef != nullptr)
                            COMP_ERROR(pRoot->pParseData, "More than one 'any' handler in input definition");
                        pAnyDef = pInputDef;
                    }
                    else if (0 == strcmp(pInputDef->str, "none"))
                    {
                        if (pNoneDef != nullptr)
                            COMP_ERROR(pRoot->pParseData, "More than one 'none' handler in input definition");
                        pNoneDef = pInputDef;
                    }
                    code += inputHandlerSpecialName(pInput->str, pInputDef->str);
                }
                code += S("(DefiningType * pThis, f32 delta, const vec4 & measure)") + LF;
                code += I + S("{") + LF;
                for (const Ast * pChild : pInputDef->pChildren->nodes)
                {
                    code += codegenRecurse(pChild, indentLevel + 1);
                }
                code += I + S("}") + LF;

                // processing code
                if (pInputDef->pSymRec->type == kSYMT_Input)
                {
                    procCode += I2 + S("// #") + S(pInputDef->str) + LF;
                    procCode += I2 + S("inputMatch = inputMgr.queryState(self().player(), ") + hashLiteral(pInputDef->str) + S(", &measureTest);") + LF;
                    procCode += I2 + S("if (inputMatch == InputMgr::kPadInputDetected)") + LF;
                    procCode += I2 + S("{") + LF;
                    procCode += I2 + S("    // Always process pad inputs, don't use exclusionary rules which are useful for simplifying keyboard inputs") + LF;
                    procCode += I2 + S("    DefiningType::") + inputHandlerName(pInput->str, pInputDef->str) + S("(this, delta, measureTest);") + LF;
                    procCode += I2 + S("}") + LF;
                    procCode += I2 + S("else if (inputMatch > maxInputMatch)") + LF;
                    procCode += I2 + S("{") + LF;
                    procCode += I2 + S("    // Highest priority input match we've found so far, switch to it") + LF;
                    procCode += I2 + S("    pHandler = &DefiningType::") + inputHandlerName(pInput->str, pInputDef->str) + S(";") + LF;
                    procCode += I2 + S("    repeatDelay = ") + floatLiteral(pInputDef->numf) + S(";") + LF;
                    procCode += I2 + S("    measure = measureTest;") + LF;
                    procCode += I2 + S("    maxInputMatch = inputMatch;") + LF;
                    procCode += I2 + S("}") + LF;
                    procCode += I2 + S("else if (inputMatch == maxInputMatch)") + LF;
                    procCode += I2 + S("{") + LF;
                    procCode += I2 + S("    // We've already found one of this priority, so effectively no input match") + LF;
                    procCode += I2 + S("    pHandler = nullptr;") + LF;
                    procCode += I2 + S("}") + LF;
                    procCode += I2 + S("// #") + S(pInputDef->str) + S(" (END)") + LF;

                    if (pInputDef != pInput->pChildren->nodes.back())
                        procCode += LF;
                }
            }
            procCode += LF;
            procCode += I1 + S("    // If we found a match with uniquely high priority (i.e. no duplicate priorities) call handler.") + LF;
            procCode += I1 + S("    if (pHandler != nullptr)") + LF;
            procCode += I1 + S("    {") + LF;
            procCode += I1 + S("        if ((void*)pHandler == mpLastInputHandler)") + LF;
            procCode += I1 + S("            mLastInputHandlerDelta += delta;") + LF;
            procCode += I1 + S("        else") + LF;
            procCode += I1 + S("            mLastInputHandlerDelta = 0.0;") + LF;
            procCode += I1 + S("        if ((void*)pHandler != mpLastInputHandler || (repeatDelay > 0.0f && mLastInputHandlerDelta > repeatDelay))") + LF;
            procCode += I1 + S("        {") + LF;
            procCode += I1 + S("            mLastInputHandlerDelta = 0.0;") + LF;
            procCode += I1 + S("            pHandler(this, delta, measure);") + LF;
            if (pAnyDef != nullptr)
            {
                procCode += LF;
                procCode += I1 + S("            // At least one input match, and an 'any' handler was specified so call it") + LF;
                procCode += I1 + S("            ") + inputHandlerSpecialName(pInput->str, pAnyDef->str) + S("(this, delta, measure);") + LF;
            }
            procCode += I1 + S("        }") + LF;
            procCode += I1 + S("    }") + LF;
            if (pNoneDef != nullptr)
            {
                procCode += LF;
                procCode += I1 + S("    if (pHandler == nullptr && mpLastInputHandler != nullptr)") + LF;
                procCode += I1 + S("    {") + LF;
                procCode += I1 + S("        // No input matches, and a 'none' handler was specified so call it") + LF;
                procCode += I1 + S("        ") + inputHandlerSpecialName(pInput->str, pNoneDef->str) + S("(this, delta, measure);") + LF;
                procCode += I1 + S("    }") + LF;
            }
            procCode += LF;
            procCode += I1 + S("    mpLastInputHandler = (void*)pHandler;") + LF;
            procCode += I1 + S("} // #") + S(pInput->str) + LF;
        }
    }

    code += I + S("void processInput(f32 delta)") + LF;
    code += I + S("{") + LF;
    code += I + S("    InputMgr & inputMgr = TaskMaster::task_master_for_active_thread().inputMgr();") + LF;
    code += LF;
    code += I + S("    u32 mode = inputMgr.mode();") + LF;
    code += LF;
    code += procCode;
    code += I + S("}") + LF;
    return code;
}

S CodegenCpp::codegenMessageParams(const Ast * pAst, int indentLevel)
{
    static const u32 kScratchSize = kMaxCmpStringLength;
    TLARRAY(char, scratch, kScratchSize+1);

    S code;
    if (pAst->pBlockInfos->blockMemoryItemCount > 0)
    {
        snprintf(scratch,
                 kScratchSize,
                 "u32 startIndex = %u; // location in message to which to copy block memory items\n",
                 pAst->pBlockInfos->blockCount);
        code += I1 + S(scratch);
    }

    // Set non-payload message data into message body
    code += I1 + S("// Write parameters to message\n");
    u32 bmId = 0;
    for (const BlockInfo & bi : pAst->pBlockInfos->items)
    {
        // NOTE: All block memory items are at the end of the BlockInfos
        // since they were sorted as such.
        if (bi.isBlockMemoryType)
        {
            snprintf(scratch,
                     kScratchSize,
                     "bmParam%u.writeMessage(msgw__, startIndex);\n",
                     bmId);
            code += I1 + S(scratch);

            snprintf(scratch,
                     kScratchSize,
                     "startIndex += bmParam%u.blockCount();\n",
                     bmId);
            code += I1 + S(scratch);

            ++bmId;
        }
        else if (!bi.isPayload)
        {
            ASSERT_MSG(bmId == 0, "BlockMemory param placed in message before regular param");

            if (bi.cellCount <= kCellsPerBlock)
            {
                snprintf(scratch,
                         kScratchSize,
                         "*reinterpret_cast<%s*>(&msgw__[%d].cells[%d]) = ",
                         ast_data_type(bi.pAst)->cppTypeStr,
                         bi.blockIndex,
                         bi.cellIndex);
                code += I1 + S(scratch);
                code += codegenRecurse(bi.pAst, 0);
                code += ";\n";
            }
            else
            {
                S val = codegenRecurse(bi.pAst, 0);
                snprintf(scratch,
                         kScratchSize,
                         "msgw__.insertBlocks(%d, %s);\n",
                         bi.blockIndex,
                         val.c_str());
                code += I1 + S(scratch);
            }
        }
    }
    return code;
}

S CodegenCpp::codegenRecurse(const Ast * pAst,
                                    int indentLevel)
{
    ASSERT(pAst);

    static const u32 kScratchSize = kMaxCmpStringLength;
    TLARRAY(char, scratch, kScratchSize+1);

    switch (pAst->type)
    {
    case kAST_FunctionDef:
    {
        S code;

        if (isTopLevelFunction(pAst))
        {
            code += "namespace compose_funcs\n";
            code += "{\n";
        }

        code += I + functionPrototype(pAst);
        code += LF;

        code += I + S("{\n");

        if (!isTopLevelFunction(pAst))
        {
            code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
        }

        if (pAst->pChildren && pAst->pChildren->nodes.size() > 0)
        {
            for (const Ast * pChild : pAst->pChildren->nodes)
            {
                code += codegenRecurse(pChild, indentLevel + 1);
            }
        }

        code += I + S("}\n");

        if (isTopLevelFunction(pAst))
        {
            code += "} // namespace compose_funcs\n";
        }

        code += LF;

        return code;
    }
    case kAST_EntityDef:
    {
        const Ast * pUpdateDef = find_update(pAst);
        const Ast * pInput = find_input(pAst, 0);
        S entName = S(pAst->pSymRec->fullName);
        S entNameScript = entName + "__script";

        S code;
        code += I + S("class ") + entName + S(" : public Entity\n{\n");
        code += I1 + S("typedef ") + entName + S(" DefiningType;") + LF;
        code += codegenHelperFuncs(pAst);

        code += I + S("public:\n");
        code += I + S("    static Entity * construct(u32 childCount, task_id initParentTask, task_id creatorTask, u32 readyMessage)\n");
        code += I + S("    {\n");
        code += I + S("#if HAS(TRACK_HASHES)\n");
        snprintf(scratch, kScratchSize, "        static bool hashRegistration = register_compose_hashes_%s();\n", entName.c_str());
        code += scratch;
        code += I + S("#endif // HAS(TRACK_HASHES)\n");
        code += I + S("        return GNEW(kMEM_Engine, ") + entName + S(", childCount, initParentTask, creatorTask, readyMessage);\n");
        code += I + S("    }\n");
        code += I + S("\n");

        // Update method, if we have one
        if (pUpdateDef || pInput)
        {
            code += updateDef(pUpdateDef, pInput, indentLevel + 1);
            code += LF;
        }

        // Functions declared within us
        for (Ast * pFnc : pAst->pChildren->nodes)
        {
            if (pFnc->type == kAST_FunctionDef)
                code += codegenRecurse(pFnc, indentLevel + 1);
        }

        // Message handlers
        S msgCode = S("");
        for (Ast * pMsg : pAst->pChildren->nodes)
        {
            msgCode += messageDef(pMsg, indentLevel + 2);
        }
        S propCode = setPropertyHandlers(pAst, indentLevel + 2);

        code += I + S("    template <typename T>\n");
        code += I + S("    MessageResult message(const T & msgAcc)\n");
        code += I + S("    {\n");
        code += I + S("        auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;

        const Ast * pCompMembers = find_component_members(pAst);

        code += I + S("        const Message & _msg = msgAcc.message();\n");
        code += I + S("        switch(_msg.msgId)\n");
        code += I + S("        {\n");

        // Add initial component members
        // NOTE: This must happen after mBlocks is initialized to hold
        // the data members of the entity.
        S compMembers = S("");
        S compMembersIndependent = S("");
        S compMembersDependent = S("");
        if (pCompMembers)
        {
            u32 compIdx = 0;
            for (Ast * pCompMember : pCompMembers->pChildren->nodes)
            {
                compMembers += I2 + S("    // Component: ") + S(pCompMember->str) + ("\n");
                compMembers += I2 + S("    {\n");
                compMembers += I2 + S("        Task & compTask = insertComponent(") + hashLiteral(pCompMember->pSymRecRef->fullName) + S(", mComponentCount);\n");
                compMembers += I2 + S("        compTask.message(msgAcc); // propagate init__ into component\n");
                ASSERT(pCompMember->pSymRecRef && pCompMember->pSymRecRef->pSymTabInternal);
                compMembers += codegenInitProperties(pCompMember->pRhs, pCompMember->pSymRecRef->pSymTabInternal, "compTask", "mScriptTask", kSDC_Asset, indentLevel + 4);
                compMembers += I2 + S("    }\n");

                compMembersDependent += I2 + S("    // Component: ") + S(pCompMember->str) + ("\n");
                compMembersDependent += I2 + S("    {\n");
                snprintf(scratch, kScratchSize, "        Task & compTask = mpComponents[%u].scriptTask();\n", compIdx);
                compMembersDependent += I2 + S(scratch);
                compMembersDependent += I2 + S("        {") + LF;
                compMembersDependent += I2 + S("            // Send init_independent_data__ into component") + LF;
                compMembersDependent += I2 + S("            {") + LF;
                compMembersDependent += I2 + S("                StackMessageBlockWriter<0> msgw(HASH::init_independent_data__, kMessageFlag_None, mScriptTask.id(), compTask.id(), to_cell(0));") + LF;
                compMembersDependent += I2 + S("                compTask.message(msgw.accessor());\n");
                compMembersDependent += I2 + S("            }") + LF;
                compMembersDependent += codegenInitProperties(pCompMember->pRhs, pCompMember->pSymRecRef->pSymTabInternal, "compTask", "mScriptTask", kSDC_Independent, indentLevel + 5);
                compMembersDependent += I2 + S("        }") + LF;
                compMembersDependent += LF;
                compMembersDependent += I2 + S("        // Send init_dependent_data__ into component") + LF;
                compMembersDependent += I2 + S("        compTask.message(msgAcc); // propagate init_dependent_data__ into component\n");
                compMembersDependent += codegenInitProperties(pCompMember->pRhs, pCompMember->pSymRecRef->pSymTabInternal, "compTask", "mScriptTask", kSDC_Dependent, indentLevel + 4);
                compMembersDependent += I2 + S("    }\n");

                compIdx++;
            }
        }

        // init__, request_assets__, etc.
        code += initializationMessageHandlers(pAst, compMembers, compMembersIndependent, compMembersDependent, indentLevel);

        // property setters
        code += propCode;

        // explicit message handlers
        code += msgCode;

        code += I + S("        }\n");

        code += I + S("        return MessageResult::Propagate;\n");
        code += I + S("    }\n");
        code += I + S("\n");

        code += I + S("private:\n");

        // Constructor
        code += I + S("    ") + entName + S("(u32 childCount, task_id initParentTask, task_id creatorTask, u32 readyMessage)\n");
        code += I + S("      : Entity(") + hashLiteral(entName.c_str()) + S(", childCount, 36, 36, initParentTask, creatorTask, readyMessage) // LORRTODO use more intelligent defaults for componentsMax and blocksMax\n");
        code += I + S("    {\n");

        // Initialize mBlockSize
        {
            scratch[kScratchSize] = '\0'; // sanity null terminator
            ASSERT(pAst->pBlockInfos);
            snprintf(scratch, kScratchSize, "%d", pAst->pBlockInfos->blockCount);
            code += I2 + S("mBlockCount = ") + S(scratch) + S(";\n");
        }
        // Prep task member
        {
            S createTaskMethod;
            if (pUpdateDef || pInput)
                createTaskMethod = S("create_updatable");
            else
                createTaskMethod = S("create");
            code += I2 + S("mScriptTask = Task::") + createTaskMethod + S("(this, ") + hashLiteral(entNameScript.c_str()) + S(");\n");
        }

        code += I + S("    }\n");
        code += LF;


        // Delete copy constructor, assignment, etc.
        code += I + S("    ") + entName + S("(const ") + entName + S("&)              = delete;\n");
        code += I + S("    ") + entName + S("(") + entName + S("&&)                   = delete;\n");
        code += I + S("    ") + entName + S(" & operator=(const ") + entName + S("&)  = delete;\n");
        code += I + S("    ") + entName + S(" & operator=(") + entName + S("&&)       = delete;\n");

        code += LF;

        // Input helper functions
        if (pInput)
        {
            code += inputBlock(pAst, indentLevel + 1);
        }

        // Property and fields accessors into mpBlocks
        for (Ast * pMsg : pAst->pChildren->nodes)
        {
            if (is_prop_or_field(pMsg->pSymRec))
            {
                code += codegenRecurse(pMsg, indentLevel + 1);
            }
        }

        code += I + S("}; // class ") + entName + S("\n");
        code += I + S("static_assert(sizeof(Entity) == sizeof(") + entName + S("), \"Entity subclass has data members!\");\n");

        code += I + S("\n} // namespace ent\n");
        code += S("\n");

        // Registration
        S reg_func_name = S("register_entity__") + entName;
        code += I + S("void ") + reg_func_name + S("(Registry & registry)\n");
        code += I + "{\n";
        code += (I + S("    if (!registry.registerEntityConstructor(") +
                 hashLiteral(entName.c_str()) + S(", ent::") + entName + S("::construct))\n"));
        code += (I + S("        PANIC(\"Unable to register entity: ") +
                 entName + S("\");\n"));
        code += I + "}\n";

        return S("namespace ent\n{\n\n") + hashRegistration(entName) + code;
    }
    case kAST_ComponentDef:
    {
        const Ast * pUpdateDef = find_update(pAst);
        const Ast * pInput = find_input(pAst, 0);
        S compName = S(pAst->pSymRec->fullName);
        S compNameScript = compName + "__script";

        S code("namespace comp\n{\n\n");
        code += I + S("class ") + compName + S(" : public Component\n{\n");
        code += I1 + S("typedef ") + compName + S(" DefiningType;") + LF;

        code += codegenHelperFuncs(pAst);

        code += I + S("public:\n");
        code += I + S("    static Component * construct(void * place, Entity * pEntity)\n");
        code += I + S("    {\n");
        code += I + S("        return new (place) ") + compName + S("(pEntity);\n");
        code += I + S("    }\n");
        code += I + S("\n");

        // Update method, if we have one
        if (pUpdateDef || pInput)
        {
            code += updateDef(pUpdateDef, pInput, indentLevel + 1);
            code += I + S("\n");
        }

        // Functions declared within us
        for (Ast * pFnc : pAst->pChildren->nodes)
        {
            if (pFnc->type == kAST_FunctionDef)
                code += codegenRecurse(pFnc, indentLevel + 1);
        }

        // Message handlers
        code += I + S("    template <typename T>\n");
        code += I + S("    MessageResult message(const T & msgAcc)\n");
        code += I + S("    {\n");
        code += I + S("        auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
        code += I + S("        const Message & _msg = msgAcc.message();\n");
        code += I + S("        switch(_msg.msgId)\n");
        code += I + S("        {\n");

        // init__, request_assets__, etc.
        code += initializationMessageHandlers(pAst, S(""), S(""), S(""), indentLevel);

        code += setPropertyHandlers(pAst, indentLevel + 2);
        // property setters

        // explicit message handlers
        for (Ast * pMsg : pAst->pChildren->nodes)
        {
            code += messageDef(pMsg, indentLevel + 2);
        }

        code += I + S("        }\n");
        code += I + S("        return MessageResult::Propagate;\n");
        code += I + S("    }\n");
        code += I + S("\n");

        code += I + S("private:\n");

        // Constructor
        code += I + S("    ") + compName + S("(Entity * pEntity)\n");
        code += I + S("      : Component(pEntity)\n");
        code += I + S("    {\n");
        // Create task
        if (pUpdateDef || pInput)
        {
            code += I + S("        mScriptTask = Task::create_updatable(this, ") + hashLiteral(compNameScript.c_str()) + S(");\n");
        }
        else
        {
            code += I + S("        mScriptTask = Task::create(this, ") + hashLiteral(compNameScript.c_str()) + S(");\n");
        }
        {
            scratch[kScratchSize] = '\0'; // sanity null terminator
            ASSERT(pAst->pBlockInfos);
            snprintf(scratch, kScratchSize, "%d", pAst->pBlockInfos->blockCount);
            code += I + S("        mBlockCount = ") + S(scratch) + S(";\n");
        }

        const Ast * pCompMembers = find_component_members(pAst);
        // Based on grammar, we currently allow components section in
        // a component.  We check for this error here for
        // simplicity. Grammar could and maybe should be improved to
        // make this illegal in parsing.
        if (pCompMembers)
            COMP_ERROR(pAst->pParseData, "Components section defined within component");

        code += I + S("    }\n");

        // Delete copy constructor, assignment, etc.
        code += I + S("    ") + compName + S("(const ") + compName + S("&)              = delete;\n");
        code += I + S("    ") + compName + S("(") + compName + S("&&)                   = delete;\n");
        code += I + S("    ") + compName + S(" & operator=(const ") + compName + S("&)  = delete;\n");
        code += I + S("    ") + compName + S(" & operator=(") + compName + S("&&)       = delete;\n");

        code += LF;

        // Input helper functions
        if (pInput)
        {
            code += inputBlock(pAst, indentLevel + 1);
        }

        code += S("\n");

        // Property and fields accessors into mBlocks
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            if (is_prop_or_field(pChild->pSymRec))
            {
                code += codegenRecurse(pChild, indentLevel + 1);
            }
        }

        code += S("\n");

        code += I + S("}; // class ") + compName + S("\n");
        code += I + S("static_assert(sizeof(Component) == sizeof(") + compName + S("), \"Component subclass has data members!\");\n");

        code += I + S("\n} // namespace comp\n");
        code += S("\n");

        // Registration
        S reg_func_name = S("register_component__") + compName;
        code += I + S("void ") + reg_func_name + S("(Registry & registry)\n");
        code += I + "{\n";
        code += (I + S("    if (!registry.registerComponentConstructor(") +
                 hashLiteral(compName.c_str()) + S(", comp::") + compName + S("::construct))\n"));
        code += (I + S("        PANIC(\"Unable to register component: ") +
                 compName + S("\");\n"));
        code += I + "}\n";

        return code;
    }
    case kAST_GlobalConstDef:
    {
        // global consts only need to be defined in the .h file
        return S("");
    }
    case kAST_PropertyDef:
    case kAST_FieldDef:
    {
        ASSERT(pAst->pParent && pAst->pParent->pBlockInfos);
        ASSERT(pAst->pSymRec);

        const BlockInfo * pBlockInfo = pAst->pParent->pBlockInfos->find(pAst);

        if (!pBlockInfo)
        {
            COMP_ERROR(pAst->pParseData, "BlockInfo not found for Ast Node");
            return S("");
        }

        S propName = S(pAst->pSymRec->name);
        S typeStr = S(ast_data_type(pAst)->cppTypeStr);
        S assignedFunc = propName + S("__isAssigned()");
        S releaseFunc = propName + S("__release()");
        int isRefCounted = is_block_memory_type(ast_data_type(pAst));

        S code = I + typeStr + S("& ") + propName + S("()\n");
        code += I + S("{\n");
        code += I + S("    return ") + propertyBlockAccessor(ast_data_type(pAst), *pBlockInfo, "mpBlocks", false, pAst->pParseData) + S(";\n");
        code += I + S("}\n");

        if (pAst->pLhs)
        {
            ASSERT(pAst->pLhs->type == kAST_PropertyPre);
            code += I + S("void ") + propName + S("__pre(") + typeStr + S(" & value)") + LF;
            code += I + S("{") + LF;
            code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
            for (const Ast * pChild : pAst->pLhs->pChildren->nodes)
                code += codegenRecurse(pChild, indentLevel+1);
            code += I + S("}") + LF;
        }
        if (pAst->pMid)
        {
            ASSERT(pAst->pMid->type == kAST_PropertyPost);
            code += I + S("void ") + propName + S("__post(") + typeStr + S(" & value)") + LF;
            code += I + S("{") + LF;
            code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
            for (const Ast * pChild : pAst->pMid->pChildren->nodes)
                code += codegenRecurse(pChild, indentLevel+1);
            code += I + S("}") + LF;
        }

        if (isRefCounted)
        {
            code += I + S("void ") + releaseFunc + LF;
            code += I + S("{\n");
            code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
            code += I + S("    if (") + assignedFunc + S(")\n");
            code += I + S("    {\n");
            code += I + S("        pThis->self().blockMemory().release(") + propName + S("());\n");
            code += I + S("    }\n");
            code += I + S("    ") + assignedFunc + S(" = false;\n");
            code += I + S("}\n");
            code += I + S("void set_") + propName + S("(const ") + typeStr + S("& rhs)\n");
            code += I + S("{\n");
            code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
            code += I + S("    ") + releaseFunc + S(";\n");
            code += I + S("    ") + propName + S("() = rhs;\n");
            code += I + S("    pThis->self().blockMemory().addRef(") + propName + S("());\n");
            code += I + S("    ") + assignedFunc + S(" = true;\n");
            code += I + S("}\n");
        }
        else if (ast_data_type(pAst)->typeDesc.dataType == kDT_asset ||
                 ast_data_type(pAst)->typeDesc.dataType == kDT_asset_handle ||
                 ast_data_type(pAst)->typeDesc.dataType == kDT_handle)
        {
            code += I + S("void ") + releaseFunc + LF;
            code += I + S("{\n");
            code += I + S("    auto pThis = this; // maintain consistency in this pointer name so we can refer to pThis in static funcs") + LF;
            code += I + S("    if (" + propName + "() != nullptr)\n");
            code += I + S("    {\n");
            code += I + S("        ") + propName + S("()->release();\n");
            code += I + S("    }\n");
            code += I + S("}\n");
        }
        code += LF;

        return code;
    }
    case kAST_Block:
    {
        ASSERT(indentLevel > 0);
        S code = indent(indentLevel-1) + S("{\n");
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            code += codegenRecurse(pChild, indentLevel);
        }
        code += indent(indentLevel-1) + S("}\n");
        return code;
    }
    case kAST_ScalarInit:
    {
        snprintf(scratch,
                 kScratchSize,
                 "(%s)",
                 pAst->pSymDataType->cppTypeStr);
        S code(scratch);
        code += codegenRecurse(pAst->pRhs->pChildren->nodes.front(), indentLevel);
        return code;
    }
    case kAST_ColorInit:
    {
        S code = S("Color(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Vec2Init:
    {
        S code = S("vec2(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Vec3Init:
    {
        S code = S("vec3(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Vec4Init:
    {
        S code = S("vec4(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Ivec2Init:
    {
        S code = S("ivec2(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Ivec3Init:
    {
        S code = S("ivec3(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Ivec4Init:
    {
        S code = S("ivec4(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_QuatInit:
    {
        S code = S("quat(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Mat43Init:
    {
        S code = S("mat43(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }

    case kAST_FunctionCall:
    {
        S code = S("");

        if (isTopLevelFunction(pAst->pSymRecRef->pAst))
        {
            code += S("compose_funcs::");
        }
        else if (!(pAst->pSymRecRef->flags & kSRFL_BuiltInFunction))
        {
            code += S("pThis->");
        }

        if (pAst->pSymRecRef->flags & kSRFL_BuiltInFunction)
            code += S(pAst->pSymRecRef->pAst->str);
        else
            code += S(pAst->pSymRecRef->fullName);

        code += S("(");
        u32 paramIdx = 0;
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel+1);
            if (paramIdx < pAst->pRhs->pChildren->nodes.size() - 1)
                code += S(", ");
            paramIdx++;
        }

        // If it's a top level function call, add in our Entity
        // pointer to serve as the pThis pointer so the function can
        // make system_api calls.
        if (isTopLevelFunction(pAst->pSymRecRef->pAst))
        {
            if (pAst->pRhs->pChildren->nodes.size() > 0)
                code += S(", ");
            code += S("&pThis->self()");
        }

        code += S(")");

        return code;
    }
    case kAST_SystemCall:
    {
        S code = S("");

        code += S("system_api::") + S(pAst->str) + S("(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegenRecurse(pParam, indentLevel+1) + S(", ");
        }
        // Always add our entity as last parameter
        code += S("&pThis->self())");

        return code;
    }
    case kAST_SymbolRef:
    {
        if (!pAst->pSymRecRef)
        {
            COMP_ERROR(pAst->pParseData, "Unknown symbol: %s", pAst->str);
            return S("");
        }
        return symref(pAst, pAst->pSymRecRef, pAst->pParseData);
    }
    case kAST_SystemConstRef:
    {
        S code = S("");
        code += S("system_api::") + S(pAst->str);
        return code;
    }
    case kAST_SymbolDecl:
    {
        S code = I + S(ast_data_type(pAst)->cppTypeStr) + S(" ") + S(pAst->pSymRec->name);
        if (pAst->pSymRec->pInitVal)
        {
            // set assignment
            code += S(" = ") + codegenRecurse(pAst->pSymRec->pInitVal, indentLevel);
        }
        return code;
    }

    case kAST_SimpleStmt:
    {
        return I + codegenRecurse(pAst->pLhs, 0) + S(";\n");
    }

    case kAST_If:
    {
        S code;
        if (!pAst->pParent || pAst->pParent->type != kAST_If)
            code += I;
        code += S("if (") + codegenRecurse(pAst->pLhs, 0) + S(")\n");
        code += codegenRecurse(pAst->pMid, indentLevel + 1);
        if (pAst->pRhs)
        {
            if (pAst->pRhs->type == kAST_If)
            {
                code += I + S("else ");
                code += codegenRecurse(pAst->pRhs, indentLevel);
            }
            else
            {
                code += I + S("else\n");
                code += codegenRecurse(pAst->pRhs, indentLevel + 1);
            }
        }

        return code;
    }
    case kAST_While:
    {
        S code = I + S("while (") + codegenRecurse(pAst->pLhs, 0) + S(")\n");
        ASSERT(pAst->pChildren && pAst->pChildren->nodes.size() == 1);
        code += codegenRecurse(pAst->pChildren->nodes.front(), indentLevel + 1);
        return code;
    }
    case kAST_DoWhile:
    {
        S code = I + S("do\n");
        ASSERT(pAst->pChildren && pAst->pChildren->nodes.size() == 1);
        code += codegenRecurse(pAst->pChildren->nodes.front(), indentLevel + 1);
        code += I + S("while (") + codegenRecurse(pAst->pLhs, 0) + S(");\n");
        return code;
    }
    case kAST_For:
    {
        S code = I + S("for (");
        code += codegenRecurse(pAst->pLhs, 0) + S("; ");
        code += codegenRecurse(pAst->pMid, 0) + S("; ");
        code += codegenRecurse(pAst->pRhs, 0) + S(")\n");
        ASSERT(pAst->pChildren && pAst->pChildren->nodes.size() == 1);
        code += codegenRecurse(pAst->pChildren->nodes.front(), indentLevel + 1);
        return code;
    }

    case kAST_Add:
    {
        return binaryOp(pAst, "+");
    }
    case kAST_Sub:
    {
        return binaryOp(pAst, "-");
    }
    case kAST_Mul:
    {
        return binaryOp(pAst, "*");
    }
    case kAST_Div:
    {
        return binaryOp(pAst, "/");
    }
    case kAST_Mod:
    {
        return binaryOp(pAst, "%");
    }
    case kAST_LShift:
    {
        return binaryOp(pAst, "<<");
    }
    case kAST_RShift:
    {
        return binaryOp(pAst, ">>");
    }
    case kAST_And:
    {
        return binaryOp(pAst, "&&");
    }
    case kAST_Or:
    {
        return binaryOp(pAst, "||");
    }
    case kAST_BitOr:
    {
        return binaryOp(pAst, "|");
    }
    case kAST_BitXor:
    {
        return binaryOp(pAst, "^");
    }
    case kAST_BitAnd:
    {
        return binaryOp(pAst, "&");
    }

    case kAST_Eq:
    {
        return binaryOp(pAst, "==");
    }
    case kAST_NEq:
    {
        return binaryOp(pAst, "!=");
    }
    case kAST_LT:
    {
        return binaryOp(pAst, "<");
    }
    case kAST_LTE:
    {
        return binaryOp(pAst, "<=");
    }
    case kAST_GT:
    {
        return binaryOp(pAst, ">");
    }
    case kAST_GTE:
    {
        return binaryOp(pAst, ">=");
    }

    case kAST_Assign:
    {
        return assign(pAst, "=");
    }
    case kAST_AddAssign:
    {
        return assign(pAst, "+=");
    }
    case kAST_SubAssign:
    {
        return assign(pAst, "-=");
    }
    case kAST_MulAssign:
    {
        return assign(pAst, "*=");
    }
    case kAST_DivAssign:
    {
        return assign(pAst, "/=");
    }
    case kAST_ModAssign:
    {
        return assign(pAst, "%=");
    }
    case kAST_LShiftAssign:
    {
        return assign(pAst, "<<=");
    }
    case kAST_RShiftAssign:
    {
        return assign(pAst, ">>=");
    }
    case kAST_AndAssign:
    {
        return assign(pAst, "&=");
    }
    case kAST_XorAssign:
    {
        return assign(pAst, "^=");
    }
    case kAST_OrAssign:
    {
        return assign(pAst, "|=");
    }

    case kAST_Not:
    {
        return unaryOp(pAst, "!");
    }
    case kAST_Complement:
    {
        return unaryOp(pAst, "~");
    }
    case kAST_Negate:
    {
        return unaryOp(pAst, "-");
    }
    case kAST_Hash:
    {
        return hashLiteral(pAst->str);
    }

    case kAST_PreInc:
    {
        return unaryOp(pAst, "++");
    }
    case kAST_PreDec:
    {
        return unaryOp(pAst, "--");
    }
    case kAST_PostInc:
    {
        return unaryOpPost(pAst->pRhs, "++");
    }
    case kAST_PostDec:
    {
        return unaryOpPost(pAst->pRhs, "--");
    }

    case kAST_EntityInit:
    {
        S code = I + entityInitFunc(pAst->str);
        code += S("(");
        const SymTab * pClosure = findClosureSymbols(pAst);
        S clArgs = closureArgs(pClosure);
        code += clArgs;
        code += S(")");
        return code;
    }
    case kAST_StructInit:
    {
        return S("/* LORRTODO: Add support for kAST_StructInit Ast Type */");
    }

    case kAST_Self:
    {
        return S("pThis->self().task().id()");
    }

    case kAST_Creator:
    {
        return S("pThis->self().creator()");
    }

    case kAST_Renderer:
    {
        return S("kRendererTaskId");
    }

    case kAST_Parent:
    {
        return S("pThis->self().parent()->task().id()");
    }

    case kAST_Transform:
    {
        return S("pThis->self().transform()");
    }

    case kAST_IntLiteral:
    {
        scratch[kScratchSize] = '\0'; // sanity null terminator
        snprintf(scratch, kScratchSize, "%d", pAst->numi);
        return S(scratch);
    }
    case kAST_BoolLiteral:
    {
        if (pAst->numi != 0)
            return S("true");
        else
            return S("false");
    }
    case kAST_FloatLiteral:
    {
        return floatLiteral(pAst->numf);
    }
    case kAST_StringLiteral:
    {
        encodeString(scratch, kScratchSize, pAst->str);
        return S("pThis->self().blockMemory().stringAlloc(") + S(scratch) + S(")");
    }
    case kAST_StringInit:
    {
        PANIC("Not Implemented");
        return S("");
    }
    case kAST_StringFormat:
    {
        ASSERT(pAst->pChildren && pAst->pChildren->nodes.size() > 1 && pAst->pChildren->nodes.front()->type == kAST_StringLiteral);
        S code("");
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            if (pChild == pAst->pChildren->nodes.front())
            {
                encodeString(scratch, kScratchSize, pChild->str);
                code += S("pThis->self().blockMemory().stringFormat(") + S(scratch);
            }
            else
            {
                code += codegenRecurse(pChild, 0);

                const SymDataType * pChildDt = ast_data_type(pChild);
                if (pChildDt->typeDesc.dataType == kDT_string)
                {
                    code += ".c_str()"; // pull the c string for snprintf
                }
                else if (is_block_memory_type(pChildDt))
                {
                    PANIC("Need to support formatting of non-string BlockMemory types");
                    return S("");
                }
            }

            if (pChild != pAst->pChildren->nodes.back())
            {
                code += S(", ");
            }
        }
        code += S(")");
        return code;
    }
    case kAST_Identifier:
    {
        return S(pAst->str);
    }
    case kAST_PropertySet:
    {
        PANIC("kAST_PropertySet should have been converted to a kAST_MessageSend during parsing!!!");
        return S("");
    }
    case kAST_MessageSend:
    {
        S code;
        code += I + S("{ // Send Message Block\n");

        ASSERT(pAst->pBlockInfos);

        // Determine our blockCount
        // If we have BlockMemory params, those have to be
        // added in at runtime.
        code += I1 + S("// Compute block size, incorporating any BlockMemory parameters dynamically\n");
        snprintf(scratch, kScratchSize, "const u32 blockCount__ = %u;\n", pAst->pBlockInfos->blockCount);
        code += I1 + S(scratch);

        // Add in blocks for BlockMemory params
        u32 bmId = 0;
        for (const BlockInfo & bi : pAst->pBlockInfos->items)
        {
            if (bi.isBlockMemoryType)
            {
                // declare a local variable to hold value of block memory item
                snprintf(scratch,
                         kScratchSize,
                         "%s bmParam%u = %s;\n",
                         bi.pSymDataType->cppTypeStr,
                         bmId,
                         codegenRecurse(bi.pAst, 0).c_str());
                code += I1 + S(scratch);

                // add this local's block count to our total
                snprintf(scratch,
                         kScratchSize,
                         "blockCount += bmParam%u.blockCount();\n",
                         bmId);

                code += I1 + S(scratch);
                ++bmId;
            }
        }

        code += LF;

        // do we have a payload?
        const BlockInfo * pPayload = pAst->pBlockInfos->find_payload();
        S payload;
        if (pPayload)
            payload = codegenRecurse(pPayload->pAst, indentLevel);
        else
            payload =  S("0 /* no payload */");

        S target;
        code += I1 + S("// Prepare the writer\n");
        // LORRTODO 2019-12-28: Optimize for child entity messages.
        // If we know we're sending to a child entity, we both have
        // access to the entity and we know we're on the same
        // TaskMaster. So... we should just create a BlockWriter and
        // send the message straight to the child entity.
        if (pAst->pLhs == 0) // Sending message to self
            target = S("pThis->self().task().id()");
        else
            target = codegenRecurse(pAst->pLhs, 0);
        snprintf(scratch,
                 kScratchSize,
                 "const u32 target__ = %s;\n",
                 target.c_str());
        code += I1 + S(scratch);
        code += LF;

        snprintf(scratch,
                 kScratchSize,
                 "StackMessageBlockWriter<blockCount__> msgw__(%s, kMessageFlag_None, pThis->self().task().id(), %s, to_cell(%s));\n",
                 codegenRecurse(pAst->pMid, 0).c_str(),
                 target.c_str(),
                 payload.c_str());
        code += I1 + S(scratch);
        code += LF;
        code += codegenMessageParams(pAst, indentLevel);
        code += LF;
        code += I1 + S("Entity * pTarget__ = pThis->self().safeImmediateMessageTarget(target__);\n");
        code += I1 + S("if (pTarget__) // this means the target is self or a child or ours, and we can send directly\n");
        code += I1 + S("{\n");
        code += I1 + S("    pTarget__->message(msgw__.accessor());\n");
        code += I1 + S("}\n");
        code += I1 + S("else // send message to our TM and let it deal with the potential queuing\n");
        code += I1 + S("{\n");
        code += I1 + S("    send_message(msgw__);\n");
        code += I1 + S("}\n");
        code += I + S("}\n");
        return code;
    }
    case kAST_Return:
    {
        S code;

        code += I + S("return ");
        code += codegenRecurse(pAst->pRhs, 0);
        code += S(";\n");
        return code;
    }
    case kAST_InputAssign:
    {
        S code;
        code += I + S("TaskMaster::task_master_for_active_thread().inputMgr().setMode(") + codegenRecurse(pAst->pRhs, 0) + S(");") + LF;
        return code;
    }

    default:
        COMP_ERROR(pAst->pParseData, "codegenRecurse invalid AstType: %d", pAst->type);
        return S("");
    }
}

S CodegenCpp::codegenHeader(const Ast * pRootAst)
{
    ASSERT(pRootAst->type == kAST_Root);

    S codeFuncs = S();
    S codeGlobals = S();

    for (Ast * pAst : pRootAst->pChildren->nodes)
    {
        if (pAst->type == kAST_FunctionDef)
        {
            codeFuncs += functionPrototype(pAst);
            codeFuncs += S(";\n");
        }
        else if (pAst->type == kAST_GlobalConstDef)
        {
            codeGlobals += S("static ");
            codeGlobals += S(pAst->pSymRec->pSymDataType->cppTypeStr);
            codeGlobals += S(" ");
            codeGlobals += S(pAst->pSymRec->fullName);
            codeGlobals += S(" = ");
            codeGlobals += codegenRecurse(pAst->pSymRec->pInitVal, 0);
            codeGlobals += S(";\n");
        }
    }

    if (codeFuncs != S("") || codeGlobals != S(""))
    {
        S headerDecls = S();
        headerDecls += S("#include \"core/base_defines.h\"\n");
        headerDecls += S("#include \"engine/CmpString.h\"\n");
        headerDecls += LF;
        headerDecls += S("namespace gaen\n{\n");
        headerDecls += LF;
        headerDecls += S("class Entity;\n");

        if (codeFuncs != S(""))
        {
            headerDecls += S("namespace compose_funcs\n{\n\n");

            headerDecls += codeFuncs;

            headerDecls += LF;
            headerDecls += S("} // namespace compose_funcs\n\n");
        }

        if (codeGlobals != S(""))
        {
            headerDecls += S("namespace compose_globals\n{\n\n");

            headerDecls += codeGlobals;

            headerDecls += LF;
            headerDecls += S("} // namespace compose_globals\n\n");
        }

        headerDecls += S("} // namespace gaen\n");
        return headerDecls;
    }

    return S("");
}

void CodegenCpp::extractFilenames(const S & cmpFullPath,
                              CodeCpp & codeCpp,
                              ParseData * pParseData)
{
    codeCpp.cmpFullPath = cmpFullPath;

    if (cmpFullPath.substr(cmpFullPath.size() - 4) != S(".cmp"))
        COMP_ERROR(pParseData, "Compose filename doesn't have .cmp extension: %s", cmpFullPath.c_str());

    S scriptsCompose("/scripts/cmp/");

    size_t scriptsComposePos = cmpFullPath.rfind(scriptsCompose);
    if (scriptsComposePos == S::npos)
        COMP_ERROR(pParseData, "Compose filename doesn't contain .../scripts/cmp/... directory: %s", cmpFullPath.c_str());

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

S CodegenCpp::hashRegistration(const S & entName)
{
    S code;

    static const u32 kScratchSize = kMaxCmpStringLength;
    TLARRAY(char, scratch, kScratchSize+1);

    code += "#if HAS(TRACK_HASHES)\n";
    snprintf(scratch, kScratchSize, "static bool register_compose_hashes_%s()\n", entName.c_str());
    code += scratch;
    code += "{\n";
    for (auto & s : mHashes)
    {
        snprintf(scratch,
                 kScratchSize,
                 "    HASH::hash_func(\"%s\");\n",
                 s.c_str());
        code += scratch;
    }
    code += "    return true;\n";
    code += "}\n";
    code += "#endif // HAS(TRACK_HASHES)\n";
    code += LF;
    return code;
}

CodeCpp CodegenCpp::codegen(ParseData * pParseData)
{
    ASSERT(pParseData);

    CodeCpp codeCpp;

    extractFilenames(pParseData->fullPath, codeCpp, pParseData);

    codeCpp.code += S("#include \"hashes/hashes.h\"\n");
    codeCpp.code += S("#include \"math/math.h\"\n");
    codeCpp.code += S("#include \"engine/Block.h\"\n");
    codeCpp.code += S("#include \"engine/BlockMemory.h\"\n");
    codeCpp.code += S("#include \"engine/compose_funcs.h\"\n");
    codeCpp.code += S("#include \"engine/MessageWriter.h\"\n");
    codeCpp.code += S("#include \"engine/TaskMaster.h\"\n");
    codeCpp.code += S("#include \"engine/InputMgr.h\"\n");
    codeCpp.code += S("#include \"engine/Task.h\"\n");
    codeCpp.code += S("#include \"engine/Handle.h\"\n");
    codeCpp.code += S("#include \"engine/Registry.h\"\n");
    codeCpp.code += S("#include \"engine/Component.h\"\n");
    codeCpp.code += S("#include \"engine/Entity.h\"\n");
    codeCpp.code += S("\n");
    codeCpp.code += S("#include \"engine/messages/Handle.h\"\n");
    codeCpp.code += S("#include \"engine/messages/Transform.h\"\n");

    codeCpp.code += S("\n");
    codeCpp.code += S("// system_api declarations\n");

    for (const CompString & inc : *pParseData->pSystemIncludes)
    {
        codeCpp.code += S("#include \"");
        codeCpp.code += inc.c_str();
        codeCpp.code += S("\"\n");
    }

    if (pParseData->scriptIncludes.size() > 0)
    {
        codeCpp.code += S("\n");
        codeCpp.code += S("// script declarations\n");
        for (const CompString & inc : pParseData->scriptIncludes)
        {
            codeCpp.code += S("#include \"");
            codeCpp.code += inc.c_str();
            codeCpp.code += S("\"\n");
        }
    }

    codeCpp.code += LF;

    codeCpp.code += S("namespace gaen\n{\n\n");

    auto codeDefs = S("");
    for (Ast * pAst : pParseData->pRootAst->pChildren->nodes)
    {
        // only process top level AST nodes from the primary file
        if (pAst->fullPath == pParseData->fullPath)
        {
            codeDefs += codegenRecurse(pAst, 0) + LF;
        }
    }
    codeCpp.code += codeDefs;

    codeCpp.code += S("} // namespace gaen\n");

    codeCpp.header = codegenHeader(pParseData->pRootAst);

    return codeCpp;
}

} // namespace gaen
