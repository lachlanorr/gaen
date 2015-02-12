//------------------------------------------------------------------------------
// codegen_cpp.cpp - C++ code generation
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

#include <algorithm>

#include "core/thread_local.h"
#include "core/List.h"
#include "engine/hashes.h"
#include "engine/Block.h"
#include "engine/BlockMemory.h"
#include "compose/system_api_meta.h"
#include "compose/codegen_cpp.h"
#include "compose/compiler_structs.h"
#include "compose/codegen_utils.h"

namespace gaen
{

#define S String<kMEM_Compose>
#define LF S("\n")
#define I indent(indentLevel)
#define I1 indent(indentLevel+1)
#define I2 indent(indentLevel+2)
#define I3 indent(indentLevel+3)

static S indent(u32 level);
static const char * cpp_type_str(DataType dt, ParseData * pParseData);
static const char * cell_field_str(DataType dt, ParseData * pParseData);
static S property_block_accessor(DataType dataType, const BlockInfo & blockInfo, const char * blockVarName, ParseData * pParseData);
static S binary_op(const Ast * pAst, const char * op);
static S unary_op(const Ast * pAst, const char* op);
static S unary_op_post(const Ast * pAst, const char* op);
static S assign(const Ast * pAst, const char * op);

static S symref(const SymRec * pSymRec, ParseData * pParseData);

static S codegen_recurse(const Ast * pAst,
                         int indentLevel);

static S codegen_header(const Ast * pRootAst);

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

static const char * cpp_type_str(DataType dt, ParseData * pParseData)
{
    switch (RAW_DT(dt))
    {
    case kDT_char:
        return IS_DT_CONST(dt) ? "const i8" : "i8";
    case kDT_byte:
        return IS_DT_CONST(dt) ? "const u8" : "u8";
    case kDT_short:
        return IS_DT_CONST(dt) ? "const i16" : "i16";
    case kDT_ushort:
        return IS_DT_CONST(dt) ? "const u16" : "u16";
    case kDT_int:
        return IS_DT_CONST(dt) ? "const i32" : "i32";
    case kDT_uint:
        return IS_DT_CONST(dt) ? "const u32" : "u32";
    case kDT_long:
        return IS_DT_CONST(dt) ? "const i64" : "i64";
    case kDT_ulong:
        return IS_DT_CONST(dt) ? "const u64" : "u64";
    case kDT_half:
        return IS_DT_CONST(dt) ? "const f16" : "f16";
    case kDT_float:
        return IS_DT_CONST(dt) ? "const f32" : "f32";
    case kDT_double:
        return IS_DT_CONST(dt) ? "const f64" : "f64";
    case kDT_bool:
        return IS_DT_CONST(dt) ? "const bool" : "bool";
    case kDT_color:
        return IS_DT_CONST(dt) ? "const Color" : "Color";
    case kDT_vec2:
        return IS_DT_CONST(dt) ? "const Vec2" : "Vec2";
    case kDT_vec3:
        return IS_DT_CONST(dt) ? "const Vec3" : "Vec3";
    case kDT_vec4:
        return IS_DT_CONST(dt) ? "const Vec4" : "Vec4";
    case kDT_mat3:
        return IS_DT_CONST(dt) ? "const Mat3" : "Mat3";
    case kDT_mat34:
        return IS_DT_CONST(dt) ? "const Mat34" : "Mat34";
    case kDT_mat4:
        return IS_DT_CONST(dt) ? "const Mat4" : "Mat4";
    case kDT_void:
        return IS_DT_CONST(dt) ? "const void" : "void";
    case kDT_handle:
        return IS_DT_CONST(dt) ? "const Handle" : "Handle";
    case kDT_entity:
        return IS_DT_CONST(dt) ? "const task_id" : "task_id";
    case kDT_string:
        return IS_DT_CONST(dt) ? "const CmpString" : "CmpString";
    default:
        COMP_ERROR(pParseData, "cpp_type_str invalid DataType: %d", dt);
        return "";
    }
}

static const char * cell_field_str(DataType dt, ParseData * pParseData)
{
    switch (RAW_DT(dt))
    {
    case kDT_char:
        return "c";
    case kDT_int:
        return "i";
    case kDT_uint:
        return "u";
    case kDT_float:
        return "f";
    case kDT_bool:
        return "b";
    case kDT_color:
        return "color";
    default:
        COMP_ERROR(pParseData, "cell_field_str invalid DataType: %d", dt);
        return "";
    }
}

void encode_string(char * enc, size_t encSize, const char * str)
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

static S property_block_accessor(DataType dataType, const BlockInfo & blockInfo, const char * blockVarName, ParseData * pParseData)
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
                 cell_field_str(dataType, pParseData));
        return S(scratch);
    }
    else
    {
        switch (RAW_DT(dataType))
        {
        case kDT_int:
        case kDT_uint:
        case kDT_float:
        case kDT_bool:
        case kDT_char:
        case kDT_color:
            snprintf(scratch, kScratchSize, "%s[%u].cells[%u].%s", blockVarName, blockInfo.blockIndex, blockInfo.cellIndex, cell_field_str(dataType, pParseData));
            return S(scratch);
        case kDT_vec3:
        case kDT_vec4:
        case kDT_mat3:
        case kDT_mat34:
        case kDT_mat4:
        case kDT_handle:
            ASSERT(blockInfo.cellIndex == 0);
            snprintf(scratch, kScratchSize, "*reinterpret_cast<%s*>(&%s[%u].qCell)", cpp_type_str(dataType, pParseData), blockVarName, blockInfo.blockIndex);
            return S(scratch);
        case kDT_string:
            snprintf(scratch, kScratchSize, "*reinterpret_cast<%s*>(&%s[%u].cells[%u])", cpp_type_str(dataType, pParseData), blockVarName, blockInfo.blockIndex, blockInfo.cellIndex);
            return S(scratch);
        default:
            COMP_ERROR(pParseData, "Invalid dataType: %d", dataType);
            return S("");
        }
    }
}


static S binary_op(const Ast * pAst, const char * op)
{
    return S("(") + codegen_recurse(pAst->pLhs, 0) + S(" ") + S(op) + S(" ") + codegen_recurse(pAst->pRhs, 0) + S(")");
}

static S unary_op(const Ast * pAst, const char* op)
{
    return S(op) + S("(") + codegen_recurse(pAst->pRhs, 0) + S(")");
}

static S unary_op_post(const Ast * pAst, const char* op)
{
    return S("(") + codegen_recurse(pAst->pRhs, 0) + S(")") + S(op);
}

static S assign(const Ast * pAst, const char * op)
{
    if (!pAst->pSymRec)
    {
        COMP_ERROR(pAst->pParseData, "Unknown symbol: %s", pAst->str);
        return S("");
    }


    if (!is_block_memory_type(pAst->pSymRec->dataType))
    {
        return symref(pAst->pSymRec, pAst->pParseData) + S(" ") + S(op) + S(" ") + codegen_recurse(pAst->pRhs, 0);
    }
    else
    {
        if (strcmp(op, "=") != 0)
        {
            COMP_ERROR(pAst->pParseData, "Invalid assignment op %s for dataType %d", op, pAst->pSymRec->dataType);
            return S("");
        }
        // call set function for ref counted types so addref/release can be done properly
        return S("set_") + S(pAst->pSymRec->name) + S("(") + codegen_recurse(pAst->pRhs, 0) + S(")");
    }
}

static S symref(const SymRec * pSymRec, ParseData * pParseData)
{
    static const u32 kScratchSize = 256;
    char scratch[kScratchSize+1];

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
        const BlockInfo * pBlockInfo = pSymRec->pAst->pBlockInfos->find(pSymRec->pAst);
        if (!pBlockInfo->isBlockMemoryType)
        {
            code = S("/*") + S(pSymRec->name) + S("*/") + property_block_accessor(ast_data_type(pSymRec->pAst), *pBlockInfo, "msgAcc", pSymRec->pAst->pParseData);
        }
        else
        {
            switch (pBlockInfo->dataType)
            {
            case kDT_string:
                ASSERT(pBlockInfo->blockMemoryIndex != -1);

                snprintf(scratch,
                         kScratchSize,
                         "entity().blockMemory().stringReadMessage(msgAcc, %u, %u)",
                         pSymRec->pAst->pBlockInfos->blockCount,
                         pBlockInfo->blockMemoryIndex);
                code = S(scratch);
                break;
            default:
                PANIC("Unsupported DataType: %u", pBlockInfo->dataType);
                code = S("");
                break;
            }
        }
    }
    else
    {
        code = S(pSymRec->name);

        if (is_prop_or_field(pSymRec))
        {
            code += S("()"); // properties and fields are accessed from mpBlocks using generated private accessors
        }
    }
    return code;
}

static S set_property_handlers(const Ast * pAst, int indentLevel)
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
        code += I + S("case HASH::") + S("set_property:\n");
        code += I1 + S("switch (_msg.payload.u)\n");
        code += I1 + S("{\n");
        for (const auto & kv : pAst->pScope->pSymTab->dict)
        {
            SymRec * pSymRec = kv.second;
            if (is_prop(pSymRec))
            {
                static const u32 kScratchSize = 128;
                char scratch[kScratchSize+1];

                DataType dt = RAW_DT(pSymRec->dataType);
                u32 cellCount = data_type_cell_count(dt, pAst->pParseData);
                u32 blockCount = block_count(cellCount);

                code += I1 + S("case HASH::") + S(pSymRec->name) + S(":\n");
                code += I1 + S("{\n");
                if (!is_block_memory_type(dt))
                {
                    snprintf(scratch, kScratchSize, "u32 requiredBlockCount = %d;\n", blockCount);
                    code += I2 + S(scratch);
                    code += I2 + S("if (_msg.blockCount >= requiredBlockCount)\n");
                    code += I2 + S("{\n");

                    u32 fullBlocksToCopy = blockCount;
                    if (cellCount % kCellsPerBlock != 0)
                        fullBlocksToCopy--;

                    // copy full blocks
                    for (u32 i = 0; i < fullBlocksToCopy; ++i)
                    {
                        snprintf(scratch,
                                 kScratchSize,
                                 "reinterpret_cast<Block*>(&%s())[%d] = msgAcc[%d];\n",
                                 pSymRec->name,
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
                                     "reinterpret_cast<Block*>(&%s())[%d].cells[%d] = msgAcc[%d].cells[%d];\n",
                                     pSymRec->name,
                                     lastBlockIdx,
                                     i,
                                     lastBlockIdx,
                                     i);
                            code += I3 + S(scratch);
                        }
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
                    code += I2 + S("if (pBlockData->type != ") + S(compose_type_to_block_type(dt)) + S(") break; // incorrect BlockData type\n");
                    code += I2 + S("u32 requiredBlockCount = pBlockData->blockCount;\n");
                    code += I2 + S("if (_msg.blockCount >= requiredBlockCount)\n");
                    code += I2 + S("{\n");
                    code += I2 + S("    Address addr = entity().blockMemory().allocCopy(pBlockData);\n");
                    code += I2 + S("    set_") + S(pSymRec->name) + S("(entity().blockMemory().string(addr));\n");
                    code += I2 + S("    return MessageResult::Consumed;\n");
                    code += I2 + S("}\n");
                    code += I2 + S("break;\n");
                    code += I1 + S("}\n");
                }
            }
        }
        code += I1 + S("}\n");
        code += I1 + S("return MessageResult::Propogate; // Invalid property\n");
    }
    return code;
}

static S data_type_init_value(DataType dataType, ParseData * pParseData)
{
    switch (RAW_DT(dataType))
    {
    case kDT_int:
    case kDT_uint:
        return S("0");
    case kDT_float:
        return S("0.0f");
    case kDT_bool:
    case kDT_char:
        return S("0");
    case kDT_vec3:
        return S("Vec3(0.0f, 0.0f, 0.0f)");
    case kDT_vec4:
        return S("Vec4(0.0, 0.0f, 0.0f, 1.0f)");
    case kDT_mat3:
        return S("Mat3(1.0f)");
    case kDT_mat34:
        return S("Mat34(1.0f)");
    case kDT_mat4:
        return S("Mat4(1.0f)");
    case kDT_handle:
        return S("Handle::null()");
    case kDT_string:
        return S("entity().blockMemory().stringAlloc(\"\")");
    default:
        COMP_ERROR(pParseData, "Unknown initial value for datatype: %d", dataType);
        return S("");
    }
}

static S init_data(const Ast * pAst, int indentLevel)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    S code = S("");
    for (const auto & kv : pAst->pScope->pSymTab->dict)
    {
        SymRec * pSymRec = kv.second;
        if (is_prop_or_field(pSymRec))
        {
            if (!is_block_memory_type(pSymRec->dataType))
            {
                code += I1 + symref(pSymRec, pAst->pParseData);
                code += S(" = ");

                // Does the script initiialize this with a value?
                if (pSymRec->pAst)
                {
                    code += codegen_recurse(pSymRec->pAst, 0);
                }
                else
                {
                    // initialzie with a default value based on the type
                    code += data_type_init_value(pSymRec->dataType, pAst->pParseData);
                }
                code += S(";\n");
            }
            else // ref counted go through set methods so we can addref/release properly
            {
                code += I + S("    set_") + pSymRec->name + S("(");

                // Does the script initialize this with a value?
                if (pSymRec->pAst)
                {
                    code += codegen_recurse(pSymRec->pAst, 0);
                }
                else
                {
                    // initialize with a default value based on the type
                    code += data_type_init_value(pSymRec->dataType, pAst->pParseData);
                }
                code += S(");\n");
            }
        }
    }
    return code;
}

static S message_def(const Ast * pAst, int indentLevel)
{
    S code;

    static const u32 kScratchSize = 256;
    char scratch[kScratchSize+1];

    if (pAst->type == kAST_MessageDef &&
        pAst->pSymRec &&
        0 != strcmp(pAst->pSymRec->name, "update"))
    {
        code += I + S("case HASH::") + S(pAst->str) + S(":\n");
        code += I + S("{\n");

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
            code += I1 + S("    return MessageResult::Propogate;\n");
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
                             compose_type_to_block_type(bi.dataType));
                    code += I1 + S(scratch);
                    code += I1 + S("expectedBlockSize += blockMemCount;\n");

                    code += I1 + S("if (blockMemCount == 0 || expectedBlockSize > msgAcc.available())\n");
                    code += I1 + S("    return MessageResult::Propogate;\n");
                    code += LF;
                }
            }
            code += LF;
        }

        code += I1 + S("// Params look compatible, message body follows\n");
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            code += codegen_recurse(pChild, indentLevel + 1);
        }
        code += I + S("    return MessageResult::Consumed;\n");
        code += I + S("}\n");
    }

    return code;
}

static S codegen_init_properties(Ast * pAst, SymTab * pPropsSymTab, const char * taskName, int indentLevel)
{
    S code = S("");
    if (pAst && pAst->pChildren)
    {
        for (Ast * pPropInit : pAst->pChildren->nodes)
        {
            // Ensure the property is valid
            SymRec * pSymRec = symtab_find_symbol(pPropsSymTab, pPropInit->str);
            if (!pSymRec || pSymRec->type != kSYMT_Property)
            {
                COMP_ERROR(pAst->pParseData, "Invalid property: '%s'", pPropInit->str);
            }

            code += I + S("// Init Property: ") + S(pPropInit->str) + ("\n");
            code += I + S("{\n");
            DataType rhsDataType = ast_data_type(pPropInit->pRhs);

            if (!is_block_memory_type(rhsDataType))
            {
                u32 valCellCount = data_type_cell_count(rhsDataType, pAst->pParseData);
                u32 blockCount = block_count(valCellCount);
                static const u32 kScratchSize = 256;
                char scratch[kScratchSize+1];
                snprintf(scratch,
                         kScratchSize,
                         "    StackMessageBlockWriter<%u> msgw(HASH::%s, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::%s));\n",
                         blockCount,
                         "set_property",
                         pPropInit->str);

                code += I + S(scratch);
                DataType dt = ast_data_type(pPropInit->pRhs);
                switch (dt)
                {
                case kDT_float:
                    code += I + S("    msgw[0].cells[0].f = ") + codegen_recurse(pPropInit->pRhs, 0);
                    break;
                case kDT_int:
                    code += I + S("    msgw[0].cells[0].i = ") + codegen_recurse(pPropInit->pRhs, 0);
                    break;
                case kDT_uint:
                    code += I + S("    msgw[0].cells[0].u = ") + codegen_recurse(pPropInit->pRhs, 0);
                    break;
                case kDT_color:
                    code += I + S("    msgw[0].cells[0].color = ") + codegen_recurse(pPropInit->pRhs, 0);
                    break;
                case kDT_vec2:
                case kDT_vec3:
                case kDT_vec4:
                case kDT_mat3:
                case kDT_mat34:
                case kDT_mat4:
                    code += I + S("    *reinterpret_cast<") + S(cpp_type_str(dt, pAst->pParseData)) + S("*>(&msgw[0].cells[0].u) = ") + codegen_recurse(pPropInit->pRhs, 0);
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
                snprintf(scratch,
                         kScratchSize,
                         "    %s val = %s;\n",
                         cpp_type_str(rhsDataType, pAst->pParseData),
                         codegen_recurse(pPropInit->pRhs, 0).c_str());
                code += I + S(scratch);
                snprintf(scratch,
                         kScratchSize,
                         "    ThreadLocalMessageBlockWriter msgw(HASH::%s, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::%s), val.blockCount());\n",
                         "set_property",
                         pPropInit->str);
                code += I + S(scratch);
                code += I + S("    val.writeMessage(msgw.accessor(), 0);\n");
            }
            code += I + S("    ") + S(taskName) + S(".message(msgw.accessor());\n");
            code += I + S("}\n");
        }
    }

    code += I + S("// Send init message\n");
    code += I + S("StackMessageBlockWriter<0> msgBW(HASH::init, kMessageFlag_None, ") + S(taskName) + S(".id(), ") + S(taskName) + S(".id(), to_cell(0));\n");
    code += I + S(taskName) + S(".message(msgBW.accessor());\n");

    return code;
}

static S codegen_helper_funcs_recurse(const Ast * pAst)
{
    S code = S("");

    if (!pAst)
        return code;

    int indentLevel = 1;

    switch (pAst->type)
    {
    case kAST_EntityInit:
    {
        ASSERT(pAst->pSymRec && pAst->pSymRec->full_name && pAst->pSymRec->pSymTabInternal);
        ASSERT(pAst->pRhs);

        code += I + S("task_id entity_init__") + S(pAst->str) + S("()") + LF;
        code += I + S("{") + LF;
        code += I + S("    Entity * pEnt = get_registry().constructEntity(HASH::") + S(pAst->pSymRec->full_name) + S(", 8);") + LF;
        code += codegen_init_properties(pAst->pRhs, pAst->pSymRec->pSymTabInternal, "pEnt->task()", indentLevel + 1);
        code += LF;
        code += I + S("    stageEntity(pEnt);") + LF;
        code += I + S("    return pEnt->task().id();") + LF;
        code += I + S("}") + LF;
        code += LF;
        break;
    }
    default:
        break;
    }

    code += codegen_helper_funcs_recurse(pAst->pLhs);
    code += codegen_helper_funcs_recurse(pAst->pMid);
    code += codegen_helper_funcs_recurse(pAst->pRhs);

    if (pAst->pChildren)
    for (Ast * pChild : pAst->pChildren->nodes)
        code += codegen_helper_funcs_recurse(pChild);

    return code;
}

static S codegen_helper_funcs(const Ast * pAst)
{
    S entityInits = S("");

    for (Ast * pAst : pAst->pChildren->nodes)
    {
        // only process top level AST nodes from the primary file
        if (pAst->fullPath == pAst->pParseData->fullPath)
        {
            entityInits += codegen_helper_funcs_recurse(pAst);
        }
    }

    if (entityInits.size() > 0)
    {
        entityInits = S("private:\n    // Helper functions\n") + entityInits + LF;
    }

    return entityInits;
}

static S codegen_recurse(const Ast * pAst,
                         int indentLevel)
{
    ASSERT(pAst);

    static const u32 kScratchSize = kMaxCmpStringLength;
    TLARRAY(char, scratch, kScratchSize+1);

    switch (pAst->type)
    {
    case kAST_FunctionDef:
    {
        return S("");
    }
    case kAST_EntityDef:
    {
        const Ast * pUpdateDef = find_update_message_def(pAst);
        S entName = S(pAst->pSymRec->full_name);

        S code("namespace ent\n{\n\n");
        code += I + S("class ") + entName + S(" : public Entity\n{\n");

        code += codegen_helper_funcs(pAst);

        code += I + S("public:\n");
        code += I + S("    static Entity * construct(u32 childCount)\n");
        code += I + S("    {\n");
        code += I + S("        return GNEW(kMEM_Engine, ") + entName + S(", childCount);\n");
        code += I + S("    }\n");
        code += I + S("\n");

        // Update method, if we have one
        if (pUpdateDef)
        {
            code += codegen_recurse(pUpdateDef, indentLevel + 1);
            code += I + S("\n");
        }


        // Message handlers
        S msgCode = S("");
        for (Ast * pMsg : pAst->pChildren->nodes)
        {
            msgCode += message_def(pMsg, indentLevel + 2);
        }
        S propCode = set_property_handlers(pAst, indentLevel + 2);

        code += I + S("    template <typename T>\n");
        code += I + S("    MessageResult message(const T & msgAcc)\n");
        code += I + S("    {\n");
        // Only insert message related code if there are message handlers to add
        if (msgCode.size() > 0 || propCode.size() > 0)
        {
            code += I + S("        const Message & _msg = msgAcc.message();\n");
            code += I + S("        switch(_msg.msgId)\n");
            code += I + S("        {\n");

            // property setters
            code += propCode;

            // explicit message handlers
            code += msgCode;

            code += I + S("        }\n");
        }
        code += I + S("        return MessageResult::Propogate;\n");
        code += I + S("    }\n");
        code += I + S("\n");

        code += I + S("private:\n");

        // Constructor
        code += I + S("    ") + entName + S("(u32 childCount)\n");
        code += I + S("      : Entity(HASH::") + entName + S(", childCount, 36, 36) // LORRTODO use more intelligent defaults for componentsMax and blocksMax\n"); 
        code += I + S("    {\n");
        // Initialize fields and properties
        code += init_data(pAst, indentLevel + 1);

        // Initialize mBlockSize
        {
            scratch[kScratchSize] = '\0'; // sanity null terminator
            ASSERT(pAst->pBlockInfos);
            snprintf(scratch, kScratchSize, "%d", pAst->pBlockInfos->blockCount);
            code += I + S("        mBlockCount = ") + S(scratch) + S(";\n");
        }
        // Prep task member
        {
            S createTaskMethod;
            if (pUpdateDef)
                createTaskMethod = S("create_updatable");
            else
                createTaskMethod = S("create");
            code += I + S("        ") + S("mScriptTask = Task::") + createTaskMethod + S("(this, HASH::") + entName + S(");\n");
        }

        // Add initial component members
        // NOTE: This must happen after mBlocks is initialized to hold
        // the data members of the entity.
        const Ast * pCompMembers = find_component_members(pAst);
        if (pCompMembers)
        {
            for (Ast * pCompMember : pCompMembers->pChildren->nodes)
            {
                code += S("\n");
                code += I + S("        // Component: ") + S(pCompMember->str) + ("\n");
                code += I + S("        {\n");
                code += I + S("            Task & compTask = insertComponent(HASH::") + S(pCompMember->pSymRec->full_name) + S(", mComponentCount);\n");
                ASSERT(pCompMember->pSymRec && pCompMember->pSymRec->pSymTabInternal);
                code += codegen_init_properties(pCompMember->pRhs, pCompMember->pSymRec->pSymTabInternal, "compTask", indentLevel + 3);

                code += S("        }\n");
            }
        }
        
        code += I + S("    }\n");

        // Delete copy constructor, assignment, etc.
        code += I + S("    ") + entName + S("(const ") + entName + S("&)              = delete;\n");
        code += I + S("    ") + entName + S("(const ") + entName + S("&&)             = delete;\n");
        code += I + S("    ") + entName + S(" & operator=(const ") + entName + S("&)  = delete;\n");
        code += I + S("    ") + entName + S(" & operator=(const ") + entName + S("&&) = delete;\n");

        code += S("\n");

        // Property and fields accessors into mpBlocks
        for (Ast * pMsg : pAst->pChildren->nodes)
        {
            if (is_prop_or_field(pMsg->pSymRec))
            {
                code += codegen_recurse(pMsg, indentLevel + 1);
            }
        }

        code += I + S("}; // class ") + entName + S("\n");

        code += I + S("\n} // namespace ent\n");
        code += S("\n");

        // Registration
        S reg_func_name = S("register_entity__") + entName;
        code += I + S("void ") + reg_func_name + S("(Registry & registry)\n");
        code += I + "{\n";
        code += (I + S("    if (!registry.registerEntityConstructor(HASH::") +
                 entName + S(", ent::") + entName + S("::construct))\n"));
        code += (I + S("        PANIC(\"Unable to register entity: ") +
                 entName + S("\");\n"));
        code += I + "}\n";

        return code;
    }
    case kAST_ComponentDef:
    {
        const Ast * pUpdateDef = find_update_message_def(pAst);
        S compName = S(pAst->pSymRec->full_name);

        S code("namespace comp\n{\n\n");
        code += I + S("class ") + compName + S(" : public Component\n{\n");
        
        code += codegen_helper_funcs(pAst);

        code += I + S("public:\n");
        code += I + S("    static Component * construct(void * place, Entity * pEntity)\n");
        code += I + S("    {\n");
        code += I + S("        return new (place) ") + compName + S("(pEntity);\n");
        code += I + S("    }\n");
        code += I + S("    \n");

        // Update method, if we have one
        if (pUpdateDef)
        {
            code += codegen_recurse(pUpdateDef, indentLevel + 1);
            code += I + S("\n");
        }
        
        // Message handlers
        code += I + S("    template <typename T>\n");
        code += I + S("    MessageResult message(const T & msgAcc)\n");
        code += I + S("    {\n");
        code += I + S("        const Message & _msg = msgAcc.message();\n");
        code += I + S("        switch(_msg.msgId)\n");
        code += I + S("        {\n");

        // #init_data
        code += I + S("        case HASH::") + S("init_data:\n");
        code += init_data(pAst, indentLevel + 2);
        code += I + S("            ") + S("return MessageResult::Consumed;\n");

        code += set_property_handlers(pAst, indentLevel + 2);
        // property setters

        // explicit message handlers
        for (Ast * pMsg : pAst->pChildren->nodes)
        {
            code += message_def(pMsg, indentLevel + 2);
        }

        code += I + S("        }\n");
        code += I + S("        return MessageResult::Propogate;\n");
        code += I + S("}\n");
        code += I + S("\n");

        code += I + S("private:\n");

        // Constructor
        code += I + S("    ") + compName + S("(Entity * pEntity)\n");
        code += I + S("      : Component(pEntity)\n");
        code += I + S("    {\n");
        // Create task
        if (pUpdateDef)
        {
            code += I + S("        mScriptTask = Task::create_updatable(this, HASH::") + compName + S(");\n");
        }
        else
        {
            code += I + S("        mScriptTask = Task::create(this, HASH::") + compName + S(");\n");
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
        code += I + S("    ") + compName + S("(const ") + compName + S("&&)             = delete;\n");
        code += I + S("    ") + compName + S(" & operator=(const ") + compName + S("&)  = delete;\n");
        code += I + S("    ") + compName + S(" & operator=(const ") + compName + S("&&) = delete;\n");

        code += S("\n");

        // Property and fields accessors into mBlocks
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            if (is_prop_or_field(pChild->pSymRec))
            {
                code += codegen_recurse(pChild, indentLevel + 1);
            }
        }

        code += S("\n");
        
        code += I + S("}; // class ") + compName + S("\n");

        code += I + S("\n} // namespace comp\n");
        code += S("\n");

        // Registration
        S reg_func_name = S("register_component__") + compName;
        code += I + S("void ") + reg_func_name + S("(Registry & registry)\n");
        code += I + "{\n";
        code += (I + S("    if (!registry.registerComponentConstructor(HASH::") +
                 compName + S(", comp::") + compName + S("::construct))\n"));
        code += (I + S("        PANIC(\"Unable to register component: ") +
                 compName + S("\");\n"));
        code += I + "}\n";

        return code;
    }
    case kAST_MessageDef:
    {
        S msgName = S(pAst->pSymRec->name);
        S code;
        
        if (is_update_message_def(pAst))
        {
            code += I + S("void update(float deltaSecs)\n");
            code += I + S("{\n");
            for (Ast * pChild : pAst->pChildren->nodes)
            {
                code += codegen_recurse(pChild, indentLevel + 1);
            }
            code += I + S("}\n");
        }
        else
        {
            // We should never get here, as non-update related message defs are
            // generated elsewhere without recursing.
            COMP_ERROR(pAst->pParseData, "Attempt to codegen non update message def in kAST_MessageDef case, which shouldn't be possible");
            return code;
        }
        return code;
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
        S typeStr = S(cpp_type_str(RAW_DT(ast_data_type(pAst)), pAst->pParseData));
        S assignedVar = S("mIs_") + propName + S("_Assigned");
        int isRefCounted = is_block_memory_type(ast_data_type(pAst));

        S code = I + typeStr + S("& ") + propName + S("()\n");
        code += I + S("{\n");
        code += I + S("    return ") + property_block_accessor(ast_data_type(pAst), *pBlockInfo, "mpBlocks", pAst->pParseData) + S(";\n");
        code += I + S("}\n");

        if (isRefCounted)
        {
            code += I + S("bool ") + assignedVar + S(" = false;\n");
            code += I + S("void set_") + propName + S("(const ") + typeStr + S("& rhs)\n");
            code += I + S("{\n");
            code += I + S("    if (") + assignedVar + S(")\n");
            code += I + S("    {\n");
            code += I + S("        entity().blockMemory().release(") + propName + S("());\n");
            code += I + S("    }\n");
            code += I + S("    else\n");
            code += I + S("    {\n");
            code += I + S("        ") + assignedVar + S(" = true;\n");
            code += I + S("    }\n");
            code += I + S("    ") + propName + S("() = rhs;\n");
            code += I + S("    entity().blockMemory().addRef(") + propName + S("());\n");
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
            code += codegen_recurse(pChild, indentLevel);
        }
        code += indent(indentLevel-1) + S("}\n");
        return code;
    }
    case kAST_ColorInit:
    {
        S code = S("Color(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegen_recurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Vec3Init:
    {
        S code = S("Vec3(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegen_recurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }
    case kAST_Mat34Init:
    {
        S code = S("Mat34(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegen_recurse(pParam, indentLevel);
            if (pParam != pAst->pRhs->pChildren->nodes.back())
                code += S(", ");
        }
        code += S(")");
        return code;
    }

//    case kAST_FunctionParams:
//    {
//        return S("");
//    }
    case kAST_FunctionCall:
    {
        return S("");
    }
    case kAST_SystemCall:
    {
        S code = S("");
        if (pAst->pRhs->pChildren->nodes.size() > kMaxApiParams)
        {
            COMP_ERROR(pAst->pParseData,"Too many parameters to a system api: %u", pAst->pRhs->pChildren->nodes.size());
            return code;
        }

        const ApiSignature * pSig = find_api(pAst->str, pAst->pParseData);
        if (!pSig)
        {
            COMP_ERROR(pAst->pParseData, "Cannot find api: %s", pAst->str);
            return code;
        }

        bool paramsMatch = true;
        u32 idx = 0;
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            if (!are_types_compatible(ast_data_type(pParam), pSig->paramTypes[idx]))
            {
                paramsMatch = false;
                break;
            }
            idx++;
        }

        if (!paramsMatch)
        {
            COMP_ERROR(pAst->pParseData, "Paramters do not match for system api call to %s", pAst->str);
            return code;
        }

        code += S("system_api::") + S(pAst->str) + S("(");
        for (Ast * pParam : pAst->pRhs->pChildren->nodes)
        {
            code += codegen_recurse(pParam, indentLevel+1) + S(", ");
        }
        // Always add our entity as last parameter
        code += S("entity())");

        return code;
    }
    case kAST_SymbolRef:
    {
        if (!pAst->pSymRec)
        {
            COMP_ERROR(pAst->pParseData, "Unknown symbol: %s", pAst->str);
            return S("");
        }
        return symref(pAst->pSymRec, pAst->pParseData);
    }
    case kAST_SymbolDecl:
    {
        S code = I + S(cpp_type_str(ast_data_type(pAst), pAst->pParseData)) + S(" ") + S(pAst->pSymRec->name);
        if (pAst->pSymRec->pAst)
        {
            // set assignment
            code += S(" = ") + codegen_recurse(pAst->pSymRec->pAst, indentLevel);
        }
        return code;
    }

    case kAST_SimpleStmt:
    {
        return I + codegen_recurse(pAst->pLhs, 0) + S(";\n");
    }

    case kAST_If:
    {
        S code = I + S("if (") + codegen_recurse(pAst->pLhs, 0) + S(")\n");
        code += codegen_recurse(pAst->pMid, indentLevel + 1);
        if (pAst->pRhs)
        {
            code += I + S("else\n");
            code += codegen_recurse(pAst->pRhs, indentLevel + 1);
        }

        return code;
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
        return binary_op(pAst, "+");
    }
    case kAST_Sub:
    {
        return binary_op(pAst, "-");
    }
    case kAST_Mul:
    {
        return binary_op(pAst, "*");
    }
    case kAST_Div:
    {
        return binary_op(pAst, "/");
    }
    case kAST_Mod:
    {
        return binary_op(pAst, "%");
    }
    case kAST_LShift:
    {
        return binary_op(pAst, "<<");
    }
    case kAST_RShift:
    {
        return binary_op(pAst, ">>");
    }
    case kAST_And:
    {
        return binary_op(pAst, "&&");
    }
    case kAST_Or:
    {
        return binary_op(pAst, "||");
    }
    case kAST_BitOr:
    {
        return binary_op(pAst, "|");
    }
    case kAST_BitXor:
    {
        return binary_op(pAst, "^");
    }
    case kAST_BitAnd:
    {
        return binary_op(pAst, "&");
    }

    case kAST_Eq:
    {
        return binary_op(pAst, "==");
    }
    case kAST_NEq:
    {
        return binary_op(pAst, "!=");
    }
    case kAST_LT:
    {
        return binary_op(pAst, "<");
    }
    case kAST_LTE:
    {
        return binary_op(pAst, "<=");
    }
    case kAST_GT:
    {
        return binary_op(pAst, ">");
    }
    case kAST_GTE:
    {
        return binary_op(pAst, ">=");
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
        return unary_op(pAst, "!");
    }
    case kAST_Complement:
    {
        return unary_op(pAst, "~");
    }
    case kAST_Negate:
    {
        return unary_op(pAst, "-");
    }
    case kAST_Hash:
    {
        return S("HASH::") + S(pAst->str);
    }

    case kAST_PreInc:
    {
        return unary_op(pAst, "++");
    }
    case kAST_PreDec:
    {
        return unary_op(pAst, "--");
    }
    case kAST_PostInc:
    {
        return unary_op_post(pAst->pRhs, "++");
    }
    case kAST_PostDec:
    {
        return unary_op_post(pAst->pRhs, "--");
    }

    case kAST_EntityInit:
    {
        S code = I + S("entity_init__") + S(pAst->str) + S("()");
        return code;
    }
    case kAST_StructInit:
    {
        return S("/* LORRTODO: Add support for kAST_StructInit Ast Type */");
    }

    case kAST_Transform:
    {
        return S("transform()");
    }

    case kAST_IntLiteral:
    {
        scratch[kScratchSize] = '\0'; // sanity null terminator
        snprintf(scratch, kScratchSize, "%d", pAst->numi);
        return S(scratch);
    }
    case kAST_FloatLiteral:
    {
        scratch[kScratchSize] = '\0'; // sanity null terminator
        snprintf(scratch, kScratchSize, "%1.8ef", pAst->numf);
        return S(scratch);
    }
    case kAST_StringLiteral:
    {
        encode_string(scratch, kScratchSize, pAst->str);
        return S("entity().blockMemory().stringAlloc(") + S(scratch) + S(")");
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
                encode_string(scratch, kScratchSize, pChild->str);
                code += S("entity().blockMemory().stringFormat(") + S(scratch);
            }
            else
            {
                code += codegen_recurse(pChild, 0);

                DataType childDt = ast_data_type(pChild);
                if (childDt == kDT_string)
                {
                    code += ".c_str()"; // pull the c string for snprintf
                }
                else if (is_block_memory_type(childDt))
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
        snprintf(scratch, kScratchSize, "u32 blockCount = %u;\n", pAst->pBlockInfos->blockCount);
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
                         cpp_type_str(bi.dataType, pAst->pParseData),
                         bmId,
                         codegen_recurse(bi.pAst, 0).c_str());
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
            payload = codegen_recurse(pPayload->pAst, indentLevel);
        else
            payload =  S("0 /* no payload */");

        S target;
        if (pAst->pLhs == 0)
            target = S("entity().task().id()");
        else
            target = codegen_recurse(pAst->pLhs, 0);

        code += I1 + S("// Prepare the queue writer\n");
        snprintf(scratch,
                 kScratchSize,
                 "MessageQueueWriter msgw(HASH::%s, kMessageFlag_None, entity().task().id(), %s, to_cell(%s), blockCount);\n",
                 pAst->str,
                 target.c_str(),
                 payload.c_str());

        code += I1 + S(scratch);
        code += LF;

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
        bmId = 0;
        for (const BlockInfo & bi : pAst->pBlockInfos->items)
        {
            // NOTE: All block memory items are at the end of the BlockInfos
            // since they were sorted as such.
            if (bi.isBlockMemoryType)
            {
                snprintf(scratch,
                         kScratchSize,
                         "bmParam%u.writeMessage(msgw, startIndex);\n",
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
                snprintf(scratch,
                         kScratchSize,
                         "*reinterpret_cast<%s*>(&msgw[%d].cells[%d]) = ",
                         cpp_type_str(ast_data_type(bi.pAst), pAst->pParseData),
                         bi.blockIndex,
                         bi.cellIndex);
                code += I1 + S(scratch);
                code += codegen_recurse(bi.pAst, 0);
                code += ";\n";
            }
        }

        code += LF;

        // Send stack message bock directly to our entity
        code += I1 + S("// MessageQueueWriter will send message through RAII when this scope is exited\n");

        code += I + S("}\n");
        return code;
    }

    default:
        COMP_ERROR(pAst->pParseData, "codegen_recurse invalid AstType: %d", pAst->type);
        return S("");
    }
}

static S codegen_header(const Ast * pRootAst)
{
    ASSERT(pRootAst->type == kAST_Root);

    S code = S();
    for (Ast * pFuncAst : pRootAst->pChildren->nodes)
    {
        if (pFuncAst->type == kAST_FunctionDef)
        {
            code += S(cpp_type_str(pFuncAst->pSymRec->dataType, pRootAst->pParseData)) + S(" ") + S(pFuncAst->pSymRec->name) + S("(");

            for (auto it = pFuncAst->pScope->pSymTab->orderedSymRecs.begin();
                 it != pFuncAst->pScope->pSymTab->orderedSymRecs.end();
                 ++it)
            {
                SymRec* pParamSymRec = *it;
                code += S(cpp_type_str(pParamSymRec->dataType, pRootAst->pParseData)) + S(" ") + S(pParamSymRec->name);
                if (pParamSymRec != pFuncAst->pScope->pSymTab->orderedSymRecs.back())
                    code += S(", ");
            }
            code += S(");\n");
        }
    }

    if (code != S(""))
    {

    S headerDecls = S();
    headerDecls += S("#include \"core/base_defines.h\"\n");
    headerDecls += LF;
    headerDecls += S("namespace gaen\n{\n");

    headerDecls += S("namespace funcs\n{\n\n");

    headerDecls += code;

    headerDecls += LF;
    headerDecls += S("} // namespace func\n");
    headerDecls += S("} // namespace gaen\n");
    return headerDecls;
    }

    return S("");
}

static void extract_filenames(const S & cmpFullPath,
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
                       

CodeCpp codegen_cpp(ParseData * pParseData)
{
    ASSERT(pParseData);

    CodeCpp codeCpp;

    extract_filenames(pParseData->fullPath, codeCpp, pParseData);
    
    codeCpp.code += S("#include \"engine/hashes.h\"\n");
    codeCpp.code += S("#include \"engine/Block.h\"\n");
    codeCpp.code += S("#include \"engine/BlockMemory.h\"\n");
    codeCpp.code += S("#include \"engine/MessageWriter.h\"\n");
    codeCpp.code += S("#include \"engine/Task.h\"\n");
    codeCpp.code += S("#include \"engine/Handle.h\"\n");
    codeCpp.code += S("#include \"engine/Registry.h\"\n");
    codeCpp.code += S("#include \"engine/Component.h\"\n");
    codeCpp.code += S("#include \"engine/Entity.h\"\n");

    if (pParseData->pApiIncludes)
    {
        for (const CompString & inc : *pParseData->pApiIncludes)
        {
            codeCpp.code += S("#include \"");
            codeCpp.code += S(inc.c_str());
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
            codeDefs += codegen_recurse(pAst, 0) + LF;
        }
    }
    codeCpp.code += codeDefs;
    codeCpp.code += S("} // namespace gaen\n");

    codeCpp.header = codegen_header(pParseData->pRootAst);

    return codeCpp;
}

} // namespace gaen
