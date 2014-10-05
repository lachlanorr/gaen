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

#include <algorithm>

#include "engine/hashes.h"
#include "engine/Block.h"
#include "compose/system_api_meta.h"
#include "compose/codegen_cpp.h"
#include "compose/compiler_structs.h"
#include "compose/codegen_utils.h"

#include "core/List.h"

namespace gaen
{

#define S String<kMEM_Compose>
#define LF S("\n")
#define I indent(indentLevel)

static S indent(u32 level);
static const char * type_str(DataType dt);
static const char * cpp_type_str(DataType dt, ParseData * pParseData);
static const char * cell_field_str(DataType dt, ParseData * pParseData);
static S property_block_accessor(DataType dataType, const BlockInfo & blockInfo, const char * blockVarName, ParseData * pParseData);
static S binary_op(const Ast * pAst, const char * op);
static S unary_op(const Ast * pAst, const char* op);
static S unary_op_post(const Ast * pAst, const char* op);
static S assign(const Ast * pAst, const char * op);

static S symref(const SymRec * pSymRec);

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

static const char * type_str(DataType dt, ParseData * pParseData)
{
    switch (RAW_DT(dt))
    {
    case kDT_char:
        return IS_DT_CONST(dt) ? "const char" : "char";
    case kDT_byte:
        return IS_DT_CONST(dt) ? "const byte" : "byte";
    case kDT_short:
        return IS_DT_CONST(dt) ? "const short" : "short";
    case kDT_ushort:
        return IS_DT_CONST(dt) ? "const ushort" : "ushort";
    case kDT_int:
        return IS_DT_CONST(dt) ? "const int" : "int";
    case kDT_uint:
        return IS_DT_CONST(dt) ? "const uint" : "uint";
    case kDT_long:
        return IS_DT_CONST(dt) ? "const long" : "long";
    case kDT_ulong:
        return IS_DT_CONST(dt) ? "const ulong" : "ulong";
    case kDT_half:
        return IS_DT_CONST(dt) ? "const half" : "half";
    case kDT_float:
        return IS_DT_CONST(dt) ? "const float" : "float";
    case kDT_double:
        return IS_DT_CONST(dt) ? "const double" : "double";
    case kDT_bool:
        return IS_DT_CONST(dt) ? "const bool" : "bool";
    case kDT_color:
        return IS_DT_CONST(dt) ? "const color" : "color";
    case kDT_vec2:
        return IS_DT_CONST(dt) ? "const vec2" : "vec2";
    case kDT_vec3:
        return IS_DT_CONST(dt) ? "const vec3" : "vec3";
    case kDT_vec4:
        return IS_DT_CONST(dt) ? "const vec4" : "vec4";
    case kDT_mat3:
        return IS_DT_CONST(dt) ? "const mat3" : "mat3";
    case kDT_mat34:
        return IS_DT_CONST(dt) ? "const mat34" : "mat34";
    case kDT_mat4:
        return IS_DT_CONST(dt) ? "const mat4" : "mat4";
    case kDT_void:
        return IS_DT_CONST(dt) ? "const void" : "void";
    case kDT_handle:
        return IS_DT_CONST(dt) ? "const handle" : "handle";
    default:
        COMP_ERROR(pParseData, "type_str invalid DataType: %d", dt);
        return "";
    }
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
    return symref(pAst->pSymRec) + S(" ") + S(op) + S(" ") + codegen_recurse(pAst->pRhs, 0);
}

static S symref(const SymRec * pSymRec)
{
    ASSERT(pSymRec);

    S code;

    if (pSymRec->type == kSYMT_MessageParam)
    {
        const BlockInfo * pBlockInfo = pSymRec->pAst->pBlockInfos->find(pSymRec->pAst);
        code = S("/*") + S(pSymRec->name) + S("*/") + property_block_accessor(ast_data_type(pSymRec->pAst), *pBlockInfo, "msgAcc", pSymRec->pAst->pParseData);
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
        code += indent(indentLevel) + S("case HASH::") + S("set_property:\n");
        code += indent(indentLevel+1) + S("switch (_msg.payload.u)\n");
        code += indent(indentLevel+1) + S("{\n");
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

                code += indent(indentLevel+1) + S("case HASH::") + S(pSymRec->name) + S(":\n");
                code += indent(indentLevel+1) + S("{\n");
                snprintf(scratch, kScratchSize, "u32 requiredBlockCount = %d;\n", blockCount);
                code += indent(indentLevel+2) + S(scratch);
                code += indent(indentLevel+2) + S("if (_msg.blockCount >= requiredBlockCount)\n");
                code += indent(indentLevel+2) + S("{\n");

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
                    code += indent(indentLevel+3) + S(scratch);
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
                        code += indent(indentLevel+3) + S(scratch);
                    }
                }
                code += indent(indentLevel+3) + S("return MessageResult::Consumed;\n");
                code += indent(indentLevel+2) + S("}\n");
                code += indent(indentLevel+1) + S("}\n");
            }
        }
        code += indent(indentLevel+1) + S("}\n");
        code += indent(indentLevel+1) + S("return MessageResult::Propogate; // Invalid property\n");
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
            code += indent(indentLevel+1) + symref(pSymRec);
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
    }
    return code;
}

static S message_def(const Ast * pAst, int indentLevel)
{
    S code;

    if (pAst->type == kAST_MessageDef &&
        pAst->pSymRec &&
        0 != strcmp(pAst->pSymRec->name, "update"))
    {
        code += I + S("case HASH::") + S(pAst->str) + S(":\n");
        code += I + S("{\n");
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            code += codegen_recurse(pChild, indentLevel + 1);
        }
        code += I + S("    return MessageResult::Consumed;\n");
        code += I + S("}\n");
    }

    return code;
}

static S codegen_recurse(const Ast * pAst,
                         int indentLevel)
{
    ASSERT(pAst);

    switch (pAst->type)
    {
    case kAST_FunctionDef:
    {
        return S("");
    }
    case kAST_EntityDef:
    {
        const Ast * pUpdateDef = find_update_message_def(pAst);
        S entName = S(pAst->pSymRec->name);

        S code("namespace ent\n{\n\n");
        code += I + S("class ") + entName + S(" : public Entity\n{\n");

        code += I + S("public:\n");
        code += I + S("    static Entity * construct(u32 childCount)\n");
        code += I + S("    {\n");
        code += I + S("        return GNEW(kMEM_Engine, ") + entName + S(", childCount);\n");
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

        // property setters
        code += set_property_handlers(pAst, indentLevel + 2);

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
        code += I + S("    ") + entName + S("(u32 childCount)\n");
        code += I + S("      : Entity(HASH::") + entName + S(", childCount, 36, 36)\n");
        code += I + S("    {\n");
        // Initialize fields and properties
        code += init_data(pAst, indentLevel + 1);
        code += S("\n");

        // Initialize mBlockSize
        {
            static const u32 kScratchSize = 32;
            char scratch[kScratchSize+1];
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
                code += I + S("            Task & compTask = insertComponent(HASH::") + S(pCompMember->str) + S(", mComponentCount);\n");
                if (pCompMember->pRhs && pCompMember->pRhs->pChildren)
                {
                    for (Ast * pPropInit : pCompMember->pRhs->pChildren->nodes)
                    {
                        code += I + S("            // Init Property: ") + S(pPropInit->str) + ("\n");
                        code += S("            {\n");
                        DataType rhsDataType = ast_data_type(pPropInit->pRhs);
                        u32 valCellCount = data_type_cell_count(rhsDataType, pAst->pParseData);
                        u32 blockCount = block_count(1 + valCellCount); // +1 for property name hash
                        static const u32 kScratchSize = 256;
                        char scratch[kScratchSize+1];
                        snprintf(scratch,
                                 kScratchSize,
                                 "                StackMessageBlockWriter<%u> msgw(HASH::%s, kMessageFlag_None, mScriptTask.id(), mScriptTask.id(), to_cell(HASH::%s));\n",
                                 blockCount,
                                 "set_property",
                                 pPropInit->str);
                        code += S(scratch);

                        switch (ast_data_type(pPropInit->pRhs))
                        {
                        case kDT_float:
                            code += S("                msgw[0].cells[0].f = ") + codegen_recurse(pPropInit->pRhs, indentLevel);
                            break;
                        case kDT_int:
                            code += S("                msgw[0].cells[0].i = ") + codegen_recurse(pPropInit->pRhs, indentLevel);
                            break;
                        case kDT_uint:
                            code += S("                msgw[0].cells[0].u = ") + codegen_recurse(pPropInit->pRhs, indentLevel);
                            break;
                        default:
                            COMP_ERROR(pAst->pParseData, "Unsupported type for codegen component property init, type: %d", pPropInit->pRhs->type);
                        }
                        code += S(";\n");
                        // Future work... support for multi cell values
                        //code += S("            cell * pCellDestStart = &msgw[0].cells[0]\n");
                        //code += S("            cell * pCellSrcStart = 
                        code += S("                compTask.message(msgw.accessor());\n");
                        code += S("            }\n");
                    }
                }
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
        S reg_func_name = S("register_entity_") + entName;
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
        S compName = S(pAst->pSymRec->name);

        S code("namespace comp\n{\n\n");
        code += I + S("class ") + compName + S(" : public Component\n{\n");
        
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
            static const u32 kScratchSize = 32;
            char scratch[kScratchSize+1];
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
        S reg_func_name = S("register_component_") + compName;
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

        S code = I + S(cpp_type_str(RAW_DT(ast_data_type(pAst)), pAst->pParseData)) + S("& ") + propName + S("()\n");
        code += I + S("{\n");
        code += I + S("    return ") + property_block_accessor(ast_data_type(pAst), *pBlockInfo, "mpBlocks", pAst->pParseData) + S(";\n");
        code += I + S("}\n");

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
        return symref(pAst->pSymRec);
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

    case kAST_IntLiteral:
    {
        static const u32 kScratchSize = 64;
        char scratch[kScratchSize+1];
        scratch[kScratchSize] = '\0'; // sanity null terminator
        snprintf(scratch, kScratchSize, "%d", pAst->numi);
        return S(scratch);
    }
    case kAST_FloatLiteral:
    {
        static const u32 kScratchSize = 64;
        char scratch[kScratchSize+1];
        scratch[kScratchSize] = '\0'; // sanity null terminator
        snprintf(scratch, kScratchSize, "%ff", pAst->numf);
        return S(scratch);
    }
    case kAST_StringLiteral:
    {
        return S("");
    }

//    case kAST_Identifier:
//    {
//    }

    case kAST_PropertySet:
    {
        return S("");
    }
    case kAST_MessageSend:
    {
        static const u32 kScratchSize = 255;

        S code;
        code += I + S("{\n");

        ASSERT(pAst->pBlockInfos);

        if (!pAst->pLhs) // send to our own entity
        {
            code += indent(indentLevel+1);
            char scratch[kScratchSize+1];
            snprintf(scratch,
                kScratchSize,
                "StackMessageBlockWriter<%u> msgw(HASH::%s, kMessageFlag_None, mpEntity->task().id(), ",
                pAst->pBlockInfos->blockCount,
                pAst->str);

            code += S(scratch);

            if (pAst->pLhs == 0)
                code += S("mpEntity->task().id()");
            else
                code += codegen_recurse(pAst->pLhs, indentLevel);

            code += S(", to_cell(");

            const BlockInfo * pPayload = pAst->pBlockInfos->find_payload();
            if (pPayload)
                code += codegen_recurse(pPayload->pAst, indentLevel);
            else
                code += S("0");

            code += S("));\n");

            // Send stack message bock directly to our entity
            code += indent(indentLevel+1) + S("mpEntity->message(msgw.accessor());\n");
        }
        else
        {
            COMP_ERROR(pAst->pParseData, "Specific target not implemented yet");
        }

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
    codeCpp.code += S("#include \"engine/MessageWriter.h\"\n");
    codeCpp.code += S("#include \"engine/Task.h\"\n");
    codeCpp.code += S("#include \"engine/Handle.h\"\n");
    codeCpp.code += S("#include \"engine/Registry.h\"\n");
    codeCpp.code += S("#include \"engine/Component.h\"\n");
    codeCpp.code += S("#include \"engine/Entity.h\"\n");
    codeCpp.code += S("#include \"engine/system_api.h\"\n");
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
