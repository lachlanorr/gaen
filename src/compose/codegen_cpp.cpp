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
static S cpp_type_str(DataType dt);
static S property_block_accessor(DataType dataType, const BlockInfo & blockInfo);
static S binary_op(const Ast * pAst, const char * op);
static S unary_op(const Ast * pAst, const char* op);
static S unary_op_post(const Ast * pAst, const char* op);
static S assign(const Ast * pAst, const char * op);

static S message_id(const Ast *pMsgSend);

static S symref(const SymRec * pSymRec);

static S codegen_recurse(const Ast * pAst,
                         int indentLevel);

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
    case kDT_char:
        return S("char");
    case kDT_byte:
        return S("byte");
    case kDT_short:
        return S("short");
    case kDT_ushort:
        return S("ushort");
    case kDT_int:
        return S("int");
    case kDT_uint:
        return S("uint");
    case kDT_long:
        return S("long");
    case kDT_ulong:
        return S("ulong");
    case kDT_half:
        return S("half");
    case kDT_float:
        return S("float");
    case kDT_double:
        return S("double");
    case kDT_bool:
        return S("bool");
    case kDT_vec2:
        return S("vec2");
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
    case kDT_handle:
        return S("handle");
    default:
        PANIC("type_str invalid DataType: %d", dt);
        return S("");
    }
}

static S cpp_type_str(DataType dt)
{
    switch (dt)
    {
    case kDT_char:
        return S("i8");
    case kDT_byte:
        return S("u8");
    case kDT_short:
        return S("i16");
    case kDT_ushort:
        return S("u16");
    case kDT_int:
        return S("i32");
    case kDT_uint:
        return S("u32");
    case kDT_long:
        return S("i64");
    case kDT_ulong:
        return S("u64");
    case kDT_half:
        return S("f16");
    case kDT_float:
        return S("f32");
    case kDT_double:
        return S("f64");
    case kDT_bool:
        return S("bool");
    case kDT_vec2:
        return S("Vec2");
    case kDT_vec3:
        return S("Vec3");
    case kDT_vec4:
        return S("Vec4");
    case kDT_mat3:
        return S("Mat3");
    case kDT_mat34:
        return S("Mat34");
    case kDT_mat4:
        return S("Mat4");
    case kDT_void:
        return S("void");
    case kDT_handle:
        return S("handle");
    default:
        PANIC("cpp_type_str invalid DataType: %d", dt);
        return S("");
    }
}

static S property_block_accessor(DataType dataType, const BlockInfo & blockInfo)
{
    static const u32 kScratchSize = 255;
    char scratch[kScratchSize+1];
    scratch[kScratchSize] = '\0';
        
    switch (dataType)
    {
    case kDT_int:
        snprintf(scratch, kScratchSize, "mpBlocks[%u].cells[%u].i", blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    case kDT_uint:
        snprintf(scratch, kScratchSize, "mpBlocks[%u].cells[%u].u", blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    case kDT_float:
        snprintf(scratch, kScratchSize, "mpBlocks[%u].cells[%u].f", blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    case kDT_bool:
        snprintf(scratch, kScratchSize, "mpBlocks[%u].cells[%u].b", blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    case kDT_char:
        snprintf(scratch, kScratchSize, "mpBlocks[%u].cells[%u].c", blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    case kDT_color:
        snprintf(scratch, kScratchSize, "mpBlocks[%u].cells[%u].color", blockInfo.blockIndex, blockInfo.cellIndex);
        return S(scratch);
    case kDT_vec3:
        ASSERT(blockInfo.cellIndex == 0);
        snprintf(scratch, kScratchSize, "*static_cast<Vec3*>(&mpBlocks[%u].qCell)", blockInfo.blockIndex);
        return S(scratch);
    case kDT_vec4:
        ASSERT(blockInfo.cellIndex == 0);
        snprintf(scratch, kScratchSize, "*static_cast<Vec4*>(&mpBlocks[%u].qCell)", blockInfo.blockIndex);
        return S(scratch);
    case kDT_mat3:
        ASSERT(blockInfo.cellIndex == 0);
        snprintf(scratch, kScratchSize, "*static_cast<Mat3*>(&mpBlocks[%u].qCell)", blockInfo.blockIndex);
        return S(scratch);
    case kDT_mat34:
        ASSERT(blockInfo.cellIndex == 0);
        snprintf(scratch, kScratchSize, "*static_cast<Mat34*>(&mpBlocks[%u].qCell)", blockInfo.blockIndex);
        return S(scratch);
    case kDT_mat4:
        ASSERT(blockInfo.cellIndex == 0);
        snprintf(scratch, kScratchSize, "*static_cast<Mat4*>(&mpBlocks[%u].qCell)", blockInfo.blockIndex);
        return S(scratch);
    default:
        PANIC("Invalid dataType: %d", dataType);
        return S("");
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

static S message_id(const Ast *pMsgSend)
{

}

static S symref(const SymRec * pSymRec)
{
    ASSERT(pSymRec);

    S code(pSymRec->name);

    if (is_prop_or_field(pSymRec))
    {
        code += S("()"); // properties and fields are accessed from mpBlocks using generated private accessors
    }
    return code;
}

static S message_and_params(const Ast * pAst)
{
    ASSERT(pAst->type == kAST_MessageSend);
    ASSERT(pAst->pRhs && pAst->pRhs->type == kAST_FunctionParams);

    S name = S(pAst->str);

    if (pAst->pRhs->pChildren->nodes.size() > 0)
    {
        name += S("_");
        for (const Ast * pChild : pAst->pRhs->pChildren->nodes)
        {
            name += S("_") + type_str(ast_data_type(pChild));
        }
    }

    return name;
}

static S set_property_handlers(const Ast * pAst, int indentLevel)
{
    ASSERT(pAst->type == kAST_EntityDef || pAst->type == kAST_ComponentDef);

    bool propTypes[kDT_COUNT];
    for (u32 i = 0; i < kDT_COUNT; ++i)
        propTypes[i] = false;

    // Determine which types we have properties for
    for (const auto & kv : pAst->pScope->pSymTab->dict)
    {
        SymRec * pSymRec = kv.second;
        if (is_prop_or_field(pSymRec))
        {
            propTypes[pSymRec->dataType] = true;
        }
    }

    S code = S("");
    for (int i = 0; i < kDT_COUNT; ++i)
    {
        if (propTypes[i])
        {
            code += indent(indentLevel) + S("case HASH::") + S("set_property__") + type_str((DataType)i) + S(":\n");
            code += indent(indentLevel+1) + S("switch (msgAcc.message().payload.u)\n");
            code += indent(indentLevel+1) + S("{\n");
            for (const auto & kv : pAst->pScope->pSymTab->dict)
            {
                SymRec * pSymRec = kv.second;
                if (is_prop_or_field(pSymRec) && pSymRec->dataType == i)
                {
                    code += indent(indentLevel+1) + S("case HASH::") + S(pSymRec->name) + S(":\n");
                    code += indent(indentLevel+2) + S(pSymRec->name) + S("() = *reinterpret_cast<const ") + S(cpp_type_str((DataType)i)) + S("*>(&msgAcc[0].cells[0].u);\n");
                    code += indent(indentLevel+2) + S("return MessageResult::Consumed;\n");
                }
            }
            code += indent(indentLevel+1) + S("}\n");
            code += indent(indentLevel+1) + S("return MessageResult::Propogate; // Invalid property\n");
        }
    }
    return code;
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
            code += indent(indentLevel+1) + symref(pSymRec) + S(" = ");
            ASSERT(pSymRec->pAst);
            code += codegen_recurse(pSymRec->pAst, 0);
            code += S(";\n");
        }
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
        code += I + S("        switch(msgAcc.message().msgId)\n");
        code += I + S("        {\n");

        // property setters
        code += set_property_handlers(pAst, indentLevel + 2);

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
                createTaskMethod = S("createUpdatable");
            else
                createTaskMethod = S("create");
            code += I + S("        ") + S("mTask = Task::") + createTaskMethod + S("(this, HASH::") + entName + S(");\n");
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
                        u32 valCellCount = data_type_cell_count(rhsDataType);
                        u32 blockCount = block_count(1 + valCellCount); // +1 for property name hash
                        static const u32 kScratchSize = 256;
                        char scratch[kScratchSize+1];
                        snprintf(scratch,
                                 kScratchSize,
                                 "                StackMessageBlockWriter<%u> msgw(HASH::%s__%s, kMessageFlag_None, mTask.id(), mTask.id(), to_cell(HASH::%s));\n",
                                 blockCount,
                                 "set_property",
                                 type_str(rhsDataType).c_str(),
                                 pPropInit->str);
                        code += S(scratch);

                        switch (pPropInit->pRhs->type)
                        {
                        case kAST_FloatLiteral:
                            code += S("                msgw[0].cells[0].f = ") + codegen_recurse(pPropInit->pRhs, indentLevel);
                            break;
                        case kAST_IntLiteral:
                            code += S("                msgw[0].cells[0].i = ") + codegen_recurse(pPropInit->pRhs, indentLevel);
                            break;
                        case kAST_Hash:
                            code += S("                msgw[0].cells[0].u = ") + codegen_recurse(pPropInit->pRhs, indentLevel);
                            break;
                        default:
                            PANIC("Unsupported type for codegen component property init, type: %d", pPropInit->pRhs->type);
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
        for (Ast * pChild : pAst->pChildren->nodes)
        {
            if (is_prop_or_field(pChild->pSymRec))
            {
                code += codegen_recurse(pChild, indentLevel + 1);
            }
        }

        code += I + S("}; // class ") + entName + S("\n");

        code += I + S("\n} // namespace ent\n");
        code += S("\n");

        // Registration
        S reg_func_name = S("register_entity_") + entName;
        code += I + S("void ") + reg_func_name + S("()\n");
        code += I + "{\n";
        code += (I + S("    if (!EntityRegistry::register_constructor(HASH::") +
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
        code += I + S("    static Component * construct(void * place)\n");
        code += I + S("    {\n");
        code += I + S("        return new (place) ") + compName + S("();\n");
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
        code += I + S("        switch(msgAcc.message().msgId)\n");
        code += I + S("        {\n");

        // #init_data
        code += I + S("        case HASH::") + S("init_data:\n");
        code += init_data(pAst, indentLevel + 2);
        code += I + S("            ") + S("return MessageResult::Consumed;\n");

        // property setters
        code += set_property_handlers(pAst, indentLevel + 2);

        code += I + S("        }\n");
        code += I + S("        return MessageResult::Propogate;\n");
        code += I + S("}\n");
        code += I + S("\n");

        code += I + S("private:\n");

        // Constructor
        code += I + S("    ") + compName + S("()\n");
        code += I + S("    {\n");
        // Create task
        if (pUpdateDef)
        {
            code += I + S("        mTask = Task::createUpdatable(this, HASH::") + compName + S(");\n");
        }
        else
        {
            code += I + S("        mTask = Task::create(this, HASH::") + compName + S(");\n");
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
            PANIC("Components section defined within component");

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
        code += I + S("void ") + reg_func_name + S("()\n");
        code += I + "{\n";
        code += (I + S("    if (!ComponentRegistry::register_constructor(HASH::") +
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
            PANIC("Not implemented yet");
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
            PANIC("BlockInfo not found for Ast Node");
            return S("");
        }

        S propName = S(pAst->pSymRec->name);

        S code = I + cpp_type_str(ast_data_type(pAst)) + S("& ") + propName + S("()\n");
        code += I + S("{\n");
        code += I + S("    return ") + property_block_accessor(ast_data_type(pAst), *pBlockInfo) + S(";\n");
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
//    case kAST_FunctionParams:
//    {
//        return S("");
//    }
    case kAST_FunctionCall:
    {
        return S("");
    }
    case kAST_SymbolRef:
    {
        return symref(pAst->pSymRec);
    }
    case kAST_SymbolDecl:
    {
        S code = I + cpp_type_str(ast_data_type(pAst)) + S(" ") + S(pAst->pSymRec->name);
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

        S full_message_name = message_and_params(pAst);

        S code;
        code += I + S("{\n");
        S target;
        if (!pAst->pLhs)
            target = S("mpCompDesc->entityTaskId");
        else
            PANIC("Specific target not implemented yet");

        ASSERT(pAst->pBlockInfos);
        
        code += indent(indentLevel+1);
        char scratch[kScratchSize+1];
        snprintf(scratch,
                 kScratchSize,
                 "StackMessageBlockWriter<%u> msgw(HASH::%s, kMessageFlag_None, mEntityTaskId, ",
                 pAst->pBlockInfos->blockCount,
                 full_message_name.c_str());

        code += S(scratch);

        if (pAst->pLhs == 0)
            code += S("mEntityTaskId");
        else
            code += codegen_recurse(pAst->pLhs, indentLevel);

        code += S(", to_cell(");

        const BlockInfo * pPayload = pAst->pBlockInfos->find_payload();
        if (pPayload)
            code += codegen_recurse(pPayload->pAst, indentLevel);
        else
            code += S("0");

        code += S("));\n");

        code += I + S("}\n");
        return code;
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
    
    codeCpp.header = S("");

    codeCpp.code += S("#include \"engine/hashes.h\"\n");
    codeCpp.code += S("#include \"engine/Block.h\"\n");
    codeCpp.code += S("#include \"engine/MessageWriter.h\"\n");
    codeCpp.code += S("#include \"engine/Task.h\"\n");
    codeCpp.code += S("#include \"engine/Registry.h\"\n");
    codeCpp.code += S("#include \"engine/Component.h\"\n");
    codeCpp.code += S("#include \"engine/Entity.h\"\n");
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
