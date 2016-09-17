//------------------------------------------------------------------------------
// compiler.h - Compose script compiler
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

#ifndef GAEN_COMPOSE_COMPILER_H
#define GAEN_COMPOSE_COMPILER_H

#ifdef __cplusplus
extern "C" {
#endif

// C Declarations, to be used from Bison

#include <stddef.h>
#include <stdio.h> // LORRTEMP

// Define our own YYLLOC_DEFAULT so we can crab the location info
// to use in error reporting.
#define YYLLOC_DEFAULT(Current, Rhs, N)                                 \
    do                                                                  \
      {                                                                 \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
       parsedata_set_location(pParseData,                               \
                              (Current).first_line,                     \
                              (Current).first_column);                  \
       }                                                                \
    while (0)


#define COMP_ERROR(pParseData, format, ...)   parsedata_formatted_message(pParseData,kMSGT_Error,format,##__VA_ARGS__)
#define COMP_WARNING(pParseData, format, ...) parsedata_formatted_message(pParseData,kMSGT_Warning,format,##__VA_ARGS__)
#define COMP_INFO(pParseData, format, ...)    parsedata_formatted_message(pParseData,kMSGT_Info,format,##__VA_ARGS__)

typedef enum
{
    kAST_Undefined = 0,

    kAST_Root,

    kAST_BasicType,

    kAST_UsingList,
    kAST_UsingStmt,
    kAST_DottedId,
    kAST_DottedIdPart,
    kAST_FunctionDef,
    kAST_SystemApiDef,
    kAST_EntityDef,
    kAST_ComponentDef,
    kAST_MessageDef,
    kAST_PropertyDef,
    kAST_PropertyDefaultAssign,
    kAST_PropertyPre,
    kAST_PropertyPost,
    kAST_FieldDef,
    kAST_ComponentBlock,
    kAST_ComponentMemberList,
    kAST_ComponentMember,
    kAST_UpdateDef,
    kAST_Input,
    kAST_InputAssign,
    kAST_InputDef,
    kAST_PropInitList,
    kAST_PropInit,
    kAST_TransformInit,
    kAST_ReadyInit,
    kAST_TypeId,

    kAST_Block,
    kAST_FunctionDecl,
    kAST_FunctionArg,
    kAST_FunctionParams,
    kAST_FunctionCall,
    kAST_SystemCall,
    kAST_SymbolRef,
    kAST_SymbolDecl,

    kAST_SimpleStmt,

    kAST_Self,
    kAST_Transform,

    kAST_If,
    kAST_While,
    kAST_DoWhile,
    kAST_For,
    
    kAST_Add,
    kAST_Sub,
    kAST_Mul,
    kAST_Div,
    kAST_Mod,
    kAST_LShift,
    kAST_RShift,
    kAST_And,
    kAST_Or,

    kAST_BitAnd,
    kAST_BitOr,
    kAST_BitXor,

    kAST_Eq,
    kAST_NEq,
    kAST_LT,
    kAST_LTE,
    kAST_GT,
    kAST_GTE,

    kAST_Assign,
    kAST_AddAssign,
    kAST_SubAssign,
    kAST_MulAssign,
    kAST_DivAssign,
    kAST_ModAssign,
    kAST_LShiftAssign,
    kAST_RShiftAssign,
    kAST_AndAssign,
    kAST_XorAssign,
    kAST_OrAssign,

    kAST_Not,
    kAST_Complement,
    kAST_Negate,
    kAST_Hash,

    kAST_PreInc,
    kAST_PreDec,
    kAST_PostInc,
    kAST_PostDec,

    kAST_ScalarInit,
    kAST_ColorInit,
    kAST_Vec2Init,
    kAST_Vec3Init,
    kAST_Vec4Init,
    kAST_Ivec2Init,
    kAST_Ivec3Init,
    kAST_Ivec4Init,
    kAST_QuatInit,
    kAST_Mat43Init,
    kAST_StringInit,
    kAST_StringFormat,

    kAST_EntityInit,
    kAST_StructInit,
    
    kAST_IntLiteral,
    kAST_BoolLiteral,
    kAST_FloatLiteral,
    kAST_StringLiteral,

    kAST_Identifier,
    kAST_PropertySet,
    kAST_MessageSend,
    kAST_Return,

    // Meta AST types are those that don't represent a language
    // construct directly, but instruct the compiler to perform
    // specially. For example, an "asset" top level ast defines
    // both the string path as well as the handle to store the
    // asset, thus both should be added to the parse tree.
    kAST_MetaAstMulti

} AstType;

typedef enum
{
    kSYMT_Undefined = 0,
    kSYMT_Function,
    kSYMT_SystemApi,
    kSYMT_Entity,
    kSYMT_Component,
    kSYMT_Type,
    kSYMT_Message,
    kSYMT_Property,
    kSYMT_PropertyDefault,
    kSYMT_PropertyPre,
    kSYMT_PropertyPost,
    kSYMT_Update,
    kSYMT_InputMode,
    kSYMT_Input,
    kSYMT_InputSpecial,
    kSYMT_Field,
    kSYMT_Param,
    kSYMT_MessageParam,
    kSYMT_Local
} SymType;


// This enum must match the %token <dataType> definition in compose.y
typedef enum
{
    kDT_Undefined = 0,

    kDT_void      = 1,
    kDT_bool      = 2,
    kDT_int       = 3,
    kDT_float     = 4,
    kDT_color     = 5,

    kDT_vec2      = 6,
    kDT_vec3      = 7,
    kDT_vec4      = 8,

    kDT_ivec2     = 9,
    kDT_ivec3     = 10,
    kDT_ivec4     = 11,

    kDT_quat      = 12,
    kDT_mat3      = 13,
    kDT_mat43     = 14,
    kDT_mat4      = 15,

    kDT_handle       = 16,
    kDT_asset_handle = 17,

    kDT_entity    = 18,

    // Dynamic data structures
    kDT_string    = 19,
    kDT_asset     = 20,

    kDT_COUNT

} DataType;

typedef enum
{
    kMSGT_Error = 0,
    kMSGT_Warning = 1,
    kMSGT_Info = 2,
} MessageType;

typedef void(*MessageHandler)(MessageType messageType, const char * message, const char * filename, int line, int column);

typedef struct SymStructField SymStructField;
typedef struct TypeDesc TypeDesc;
typedef struct SymDataType SymDataType;
typedef struct SymRec SymRec;
typedef struct SymTab SymTab;
typedef struct AstList AstList;
typedef struct Ast Ast;
typedef struct Scope Scope;
typedef struct ParseData ParseData;
typedef struct Using Using;
typedef struct RelatedTypes RelatedTypes;

int parse_int(const char * pStr, int base);
float parse_float(const char * pStr);
const char * parse_identifier(const char * str, ParseData * pParseData);

size_t mangle_function_len(const char * name, const AstList * pParamList);
void mangle_function(char * mangledName, int mangledNameSize, const char * name, const AstList * pParamList);
const char * unmangle(const char * mangledName);
size_t mangle_type_len(const char * name);
void mangle_type(char * mangledName, size_t mangledNameSize, const char * name, int isConst, int isReference);

size_t mangle_param_len(const char * name);
void mangle_param(char * mangledName, size_t mangledNameSize, const char * name, int isConst, int isReference);

SymDataType * symdatatype_create(DataType dataType,
                                 const char * name,
                                 const char * cppName,
                                 unsigned int cellCount,
                                 int isConst,
                                 int isReference,
                                 ParseData * pParseData);

void symdatatype_add_field(SymDataType * pSdt, SymDataType * pFieldSdt, const char * fieldName);
void symdatatype_add_field_related(RelatedTypes * pRelatedTypes, SymDataType * pFieldSdt, const char * fieldName);
const SymStructField * symdatatype_find_field(const SymDataType * pSdt, const char * fieldName);

SymRec * symrec_create(SymType symType,
                       const SymDataType * pSdt,
                       const char * name,
                       Ast * pAst,
                       Ast * pInitVal,
                       ParseData * pParseData);

SymTab* symtab_create(ParseData * pParseData);
SymTab* symtab_add_symbol(SymTab* pSymTab, SymRec * pSymRec, ParseData * pParseData);
SymTab* symtab_add_symbol_with_fields(SymTab* pSymTab, SymRec * pSymRec, ParseData * pParseData);
SymRec* symtab_find_symbol(SymTab* pSymTab, const char * name);
SymRec* symtab_find_symbol_recursive(SymTab* pSymTab, const char * name);
const SymDataType* symtab_find_type_recursive(SymTab* pSymTab, DataType dataType, int isConst, int isReference);
SymRec* symtab_find_type(SymTab* pSymTab, const char * name);
SymTab* symtab_transfer(SymTab* pDest, SymTab* pSrc, ParseData* pParseData);

AstList * astlist_create();
AstList * astlist_append(AstList * pAstList, Ast * pAst);


Ast * ast_create(AstType astType, ParseData * pParseData);
Ast * ast_create_with_child_list(AstType astType, ParseData * pParseData);

Ast * ast_create_with_str(AstType astType, const char * str, ParseData * pParseData);
Ast * ast_create_with_numi(AstType astType, int numi, ParseData * pParseData);
Ast * ast_create_dotted_id(Ast * pItems, ParseData * pParseData);

Ast * ast_create_input_mode(const char * name, Ast * pInputList, ParseData * pParseData);

void ast_create_using_list(Ast * pUsingList, ParseData * pParseData);
Ast * ast_create_using_stmt(Ast * pUsingDottedId, Ast * pAsDottedId, ParseData * pParseData);
Ast * ast_create_function_def(const char * name, const SymDataType * pReturnType, SymTab * pSymTab, Ast * pBlock, ParseData * pParseData);
Ast * ast_create_update_def(Ast * pBlock, ParseData * pParseData);
Ast * ast_create_input_def(const char * name, Ast * pBlock, ParseData * pParseData);
Ast * ast_create_input_special_def(const char * name, Ast * pBlock, ParseData * pParseData);
Ast * ast_create_entity_def(const char * name, Ast * pBlock, ParseData * pParseData);
Ast * ast_create_component_def(const char * name, Ast * pBlock, ParseData * pParseData);

Ast * ast_create_message_def(const char * name, SymTab * pSymTab, Ast * pBlock, ParseData * pParseData);
Ast * ast_create_field_def(const char * name, const SymDataType * pDataType, Ast * pInitVal, ParseData * pParseData);

Ast * ast_create_property_def(Ast * pPropDecl, Ast * pInitVal, ParseData * pParseData);
Ast * ast_create_property_complex_def(Ast * pPropDecl, Ast * pDefList, ParseData * pParseData);
Ast * ast_create_property_decl(const char * name, const SymDataType * pDataType, ParseData * pParseData);
Ast * ast_create_property_default_assign(Ast * pRhs, ParseData * pParseData);
Ast * ast_create_property_pre(Ast * pBlock, ParseData * pParseData);
Ast * ast_create_property_post(Ast * pBlock, ParseData * pParseData);

Ast * ast_create_function_arg(const char * name, SymRec * pDataTypeSymRec, ParseData * pParseData);

Ast * ast_create_component_members(Ast * pAst, ParseData * pParseData);
Ast * ast_create_component_member(Ast * pDottedId, Ast * pPropInitList, ParseData * pParseData);
Ast * ast_create_prop_init(const char * name, Ast * pVal, ParseData * pParseData);
Ast * ast_create_transform_init(Ast * pVal, ParseData * pParseData);
Ast * ast_create_ready_init(Ast * pVal, ParseData * pParseData);

Ast * ast_create_simple_stmt(Ast * pExpr, ParseData * pParseData);
Ast * ast_create_input_assign(Ast * pExpr, ParseData * pParseData);

Ast * ast_create_unary_op(AstType astType, Ast * pRhs, ParseData * pParseData);
Ast * ast_create_binary_op(AstType astType, Ast * pLhs, Ast * pRhs, ParseData * pParseData);
Ast * ast_create_hash(const char * name, ParseData * pParseData);

Ast * ast_create_assign_op(AstType astType, Ast * pDottedId, Ast * pRhs, ParseData * pParseData);

Ast * ast_create_scalar_init(DataType dataType, Ast * pParams, ParseData * pParseData);
Ast * ast_create_color_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_vec2_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_vec3_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_vec4_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_ivec2_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_ivec3_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_ivec4_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_quat_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_mat43_init(Ast * pParams, ParseData * pParseData);
Ast * ast_create_string_init(Ast * pParams, ParseData * pParseData);

Ast * ast_create_type_init(DataType dataType, Ast * pParams, ParseData * pParseData);
Ast * ast_create_entity_init(Ast * pDottedId, Ast * pParams, ParseData * pParseData);

Ast * ast_create_int_literal(int numi, ParseData * pParseData);
Ast * ast_create_bool_literal(int numi, ParseData * pParseData);
Ast * ast_create_float_literal(float numf, ParseData * pParseData);
Ast * ast_create_string_literal(const char * str, ParseData * pParseData);

Ast * ast_create_function_call(Ast * pFuncName, Ast * pParams, ParseData * pParseData);
Ast * ast_create_system_api_call(const char * pApiName, Ast * pParams, ParseData * pParseData);
Ast * ast_create_symbol_ref(Ast * pDottedId, ParseData * pParseData);

Ast * ast_create_if(Ast * pCondition, Ast * pIfBody, Ast * pElseBody, ParseData * pParseData);
Ast * ast_create_while(Ast * pCondition, Ast * pBody, ParseData * pParseData);
Ast * ast_create_dowhile(Ast * pCondition, Ast * pBody, ParseData * pParseData);
Ast * ast_create_for(Ast * pInit, Ast * pCondition, Ast * pUpdate, Ast * pBody, ParseData * pParseData);

Ast * ast_create_block(Ast* pBlock, ParseData * pParseData);

Ast * ast_create_identifier(const char * name, ParseData * pParseData);
Ast * ast_create_property_set(Ast *pTarget, Ast * pProp, Ast *pRhs, ParseData *pParseData);
Ast * ast_create_message_send(Ast *pTarget, Ast * pMessage, Ast *pParams, ParseData *pParseData);
Ast * ast_create_return(Ast *pExpr, ParseData *pParseData);

Ast * ast_append(AstType astType, Ast * pAst, Ast * pAstNew, ParseData * pParseData);
Ast * ast_add_child(Ast * pParent, Ast * pChild);
Ast * ast_add_children(Ast * pParent, AstList * pChildren);

void ast_set_lhs(Ast * pParent, Ast * pLhs);
void ast_set_mid(Ast * pParent, Ast * pMid);
void ast_set_rhs(Ast * pParent, Ast * pRhs);

const SymDataType * ast_data_type(const Ast * pAst);
const SymDataType * const_data_type(ParseData * pParseData, const SymDataType * pSdt);
const SymDataType * non_const_data_type(ParseData * pParseData, const SymDataType * pSdt);
int is_block_memory_type(const SymDataType * pSdt);
int is_integral_type(const SymDataType * pSdt);

void parsedata_destroy(ParseData * pParseData);
const char * parsedata_dotted_to_path(ParseData * pParseData, const char * dottedId);
void parsedata_prep_paths(ParseData * pParseData, const char * fullPath);
void *  parsedata_scanner(ParseData * pParseData);

void parsedata_formatted_message(ParseData * pParseData,
                                 MessageType messageType,
                                 const char * format, ...);

// Adds to a specific symbol table, or to a new one if you send NULL
// This is used for parameters of functions, or complex statements
// (like a for loop with variable initialization)
SymTab* parsedata_add_param(ParseData * pParseData, SymTab* pSymTab, SymRec * pSymRec);

// Adds to the symbol table on top of the stack currently.
// A new symbol table should already have been added to the stack.
Ast* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec);
Ast* parsedata_add_root_symbol(ParseData * pParseData, SymRec * pSymRec);

Ast * parsedata_add_root_ast(ParseData * pParseData, Ast * pAst);

const SymDataType * parsedata_find_basic_type(ParseData * pParseData, DataType dataType, int isConst, int isReference);
const SymDataType * parsedata_find_basic_type_desc(ParseData * pParseData, const TypeDesc * pTypeDesc);
void parsedata_register_basic_type(ParseData * pParseData, SymDataType * pSymDataType);

SymRec* parsedata_find_symbol(ParseData * pParseData, Ast * pAst);

SymRec* parsedata_find_function_symbol(ParseData * pParseData, const char * name, Ast * pParams);
SymRec* parsedata_find_type_symbol(ParseData * pParseData, const char * name, int isConst, int isReference);
const SymDataType* parsedata_find_type(ParseData * pParseData, const char * name, int isConst, int isReference);
SymRec* parsedata_find_type_symbol_from_dotted_id(ParseData * pParseData, const Ast * pAstId, int isConst, int isReference);
const SymDataType* parsedata_find_type_from_dotted_id(ParseData * pParseData, const Ast * pAstId, int isConst, int isReference);

Scope* parsedata_current_scope(ParseData * pParseData);
Scope* parsedata_push_scope(ParseData * pParseData);
Scope* parsedata_pop_scope(ParseData * pParseData);

Scope* parsedata_push_update_scope(ParseData * pParseData);
Scope* parsedata_push_input_scope(ParseData * pParseData);
Scope* parsedata_push_prop_prepost_scope(ParseData * pParseData);

const char * parsedata_add_string(ParseData * pParseData, const char * str);

void parsedata_set_location(ParseData * pParseData,
                            int line,
                            int column);

const Using * parsedata_parse_using(ParseData * pParseData,
                                    const char * namespace_,
                                    const char * fullPath);


void parse_init();

typedef struct YYLTYPE YYLTYPE;
void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...);

#ifdef __cplusplus
} // extern "C"

#include "compose/comp_mem.h"
namespace gaen
{

void register_basic_types(ParseData * pParseData);
void register_system_apis(ParseData * pParseData);

ParseData * parse_file(const char * fullPath,
                       u32 apiIncludesCount,
                       const char ** pApiIncludes,
                       MessageHandler messageHandler);

} // namespace gaen
#endif

#endif // #ifndef GAEN_COMPOSE_COMPILER_H

