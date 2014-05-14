//------------------------------------------------------------------------------
// compiler.cpp - Compose script compiler
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

#include <cstdarg>
#include <cstdlib>

#include "core/platutils.h"

#include "compose/comp_mem.h"
#include "compose/compiler.h"

extern "C" {
#define YYDEBUG 1
#include "compose/compose_parser.h"
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
}

using namespace gaen;

struct SymRec
{
    SymType symType;
    DataType type;
    const char * name;
    Ast * pAst;
    SymTab * pSymTab;
};

struct SymTab
{
    SymTab * pParent;
    Ast * pAst;
    CompHashMap<const char*, SymRec*> dict;
    CompList<SymTab*> children;
};

struct AstList
{
    CompList<Ast*> nodes;
};

struct Ast
{
    AstType type;
    Ast* pParent;
    SymTab* pSymTab;
    SymRec* pSymRec;

    Ast* pLhs;
    Ast* pMid;
    Ast* pRhs;

    int numi;
    float numf;
    const char * str;

    AstList* pChildren;
};

struct ParseData
{
    Ast* pRootAst;
    SymTab* pRootSymTab;
    void * pScanner;
    CompList<SymTab*> symTabStack;
    CompHashSet<CompString> strings;

    // location info
    int line;
    int column;

    const char * filename;
    MessageHandler messageHandler;
};


int parse_int(const char * pStr, int base)
{
    i64 val = strtol(pStr, nullptr, base);
    // LORRTODO - check for over/underflow
    return static_cast<i32>(val);
}

float parse_float(const char * pStr)
{
    f32 val = strtof(pStr, nullptr);
    // LORRTODO - check for over/underflow
    return val;
}

//------------------------------------------------------------------------------
// SymRec
//------------------------------------------------------------------------------
SymRec * symrec_create(SymType symType,
                       DataType type,
                       const char * name,
                       Ast * pAst)
{
    SymRec * pSymRec = COMP_NEW(SymRec);

    pSymRec->symType = symType;
    pSymRec->type = type;
    pSymRec->name = name;
    pSymRec->pAst = pAst;

    pSymRec->pSymTab = nullptr;
    return pSymRec;
}
//------------------------------------------------------------------------------
// SymRec (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// SymTab
//------------------------------------------------------------------------------
SymTab* symtab_create()
{
    SymTab* pSymTab = COMP_NEW(SymTab);
    pSymTab->pParent = nullptr;
    pSymTab->pAst = nullptr;
    return pSymTab;
}

SymTab* symtab_add_symbol(SymTab* pSymTab, SymRec * pSymRec, ParseData * pParseData)
{
    ASSERT(pSymTab && pSymRec);
    ASSERT(pSymRec->pSymTab == nullptr);
    ASSERT(pSymRec->name);

    SymRec * pSymRecSearch = symtab_find_symbol(pSymTab, pSymRec->name);

    if (pSymRecSearch)
    {
        COMP_ERROR("Symbol already defined: %s", pSymRec->name);
        return pSymTab;
    }

    pSymRec->pSymTab = pSymTab;
    pSymTab->dict[pSymRec->name] = pSymRec;
    return pSymTab;
}

SymRec* symtab_find_symbol(SymTab* pSymTab, const char * name)
{
    ASSERT(pSymTab);
    ASSERT(name);

    auto it = pSymTab->dict.find(name);

    if (it != pSymTab->dict.end())
    {
        return it->second;
    }
    else if (pSymTab->pParent)
    {
        return symtab_find_symbol(pSymTab->pParent, name);
    }

    return nullptr;
}

//------------------------------------------------------------------------------
// SymTab (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// AstList
//------------------------------------------------------------------------------
AstList * astlist_create()
{
    AstList * pAstList = COMP_NEW(AstList);
    return pAstList;
}

AstList * astlist_append(AstList * pAstList, Ast * pAst)
{
    if (!pAstList)
    {
        pAstList = astlist_create();
    }
    if (pAst)
    {
        pAstList->nodes.push_back(pAst);
    }
    return pAstList;
}
//------------------------------------------------------------------------------
// AstList (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Ast
//------------------------------------------------------------------------------
Ast * ast_create(AstType astType, ParseData * pParseData)
{
    ASSERT(pParseData);

    Ast * pAst = COMP_NEW(Ast);

    pAst->type = astType;
    pAst->pParent = nullptr;
    pAst->pSymTab = parsedata_current_scope(pParseData);
    pAst->pSymRec = nullptr;

    pAst->pLhs = nullptr;
    pAst->pMid = nullptr;
    pAst->pRhs = nullptr;

    pAst->numi = 0;
    pAst->numf = 0;
    pAst->str = nullptr;

    pAst->pChildren = nullptr;
    
    return pAst;
}

Ast * ast_create_message_def(const char * name, Ast * pBlock, ParseData * pParseData)
{
    ASSERT(pBlock);
    ASSERT(pBlock->pChildren);
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_MessageDef, pParseData);
    ast_add_children(pAst, pBlock->pChildren);
    
    ast_add_child(pParseData->pRootAst, pAst);

    SymTab * pSymTab = parsedata_pop_scope(pParseData);
    ASSERT(pSymTab->pAst == nullptr);
    pSymTab->pAst = pAst;

    pAst->pSymRec = symrec_create(kSYMT_Message,
                                  kDT_Undefined,
                                  name,
                                  pAst);
    
    parsedata_add_local_symbol(pParseData, pAst->pSymRec);

    return pAst;
}

Ast * ast_create_function_def(const char * name, DataType returnType, Ast * pBlock, ParseData * pParseData)
{
    ASSERT(pBlock);
    ASSERT(pBlock->pChildren);
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_FunctionDef, pParseData);
    ast_add_children(pAst, pBlock->pChildren);
    
    ast_add_child(pParseData->pRootAst, pAst);

    SymTab * pSymTab = parsedata_pop_scope(pParseData);
    ASSERT(pSymTab->pAst == nullptr);
    pSymTab->pAst = pAst;

    pAst->pSymRec = symrec_create(kSYMT_Function,
                                  returnType,
                                  name,
                                  pAst);

    parsedata_add_local_symbol(pParseData, pAst->pSymRec);

    return pAst;
}

Ast * ast_create_unary_op(AstType astType, Ast * pRhs, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);

    pAst->pRhs = pRhs;
    pRhs->pParent = pAst;

    return pAst;
}

Ast * ast_create_binary_op(AstType astType, Ast * pLhs, Ast * pRhs, ParseData * pParseData)
{
    Ast * pAst = ast_create(astType, pParseData);

    pAst->pLhs = pLhs;
    pLhs->pParent = pAst;

    pAst->pRhs = pRhs;
    pRhs->pParent = pAst;

    return pAst;
}

Ast * ast_create_assign_op(AstType astType, const char * name, Ast * pRhs, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(astType, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR("Unknown symbol reference in assignment: %s", name);
        return pAst;
    }

    if (pSymRec->symType != kSYMT_Param &&
        pSymRec->symType != kSYMT_Local)
    {
        COMP_ERROR("Invalid use of symbol in assignment: %s", name);
        return pAst;
    }   

    pAst->pSymRec = pSymRec;
    pAst->pRhs = pRhs;
    return pAst;
}

Ast * ast_create_int_literal(int numi, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_IntLiteral, pParseData);
    pAst->numi = numi;
    return pAst;
}

Ast * ast_create_float_literal(float numf, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_FloatLiteral, pParseData);
    pAst->numf = numf;
    return pAst;
}

Ast * ast_create_function_call(const char * name, Ast * pParams, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(kAST_FunctionCall, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR("Unknown symbol reference: %s", name);
        return pAst;
    }

    if (pSymRec->symType != kSYMT_Function)
    {
        COMP_ERROR("Call to non-function symbol: %s", name);
        return pAst;
    }   

    pAst->pSymRec = pSymRec;
    ast_add_children(pAst, pParams->pChildren);
    
    return pAst;
}

Ast * ast_create_symbol_ref(const char * name, ParseData * pParseData)
{
    SymRec * pSymRec = parsedata_find_symbol(pParseData, name);

    Ast * pAst = ast_create(kAST_SymbolRef, pParseData);

    if (!pSymRec)
    {
        COMP_ERROR("Unknown symbol reference: %s", name);
        return pAst;
    }

    if (pSymRec->symType != kSYMT_Param &&
        pSymRec->symType != kSYMT_Local)
    {
        COMP_ERROR("Invalid use of symbol: %s", name);
        return pAst;
    }   

    pAst->pSymRec = pSymRec;
    
    return pAst;
}

Ast * ast_create_if(Ast * pCondition, Ast * pIfBlock, Ast * pElseBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_If, pParseData);

    pAst->pLhs = pCondition;
    pAst->pMid = pIfBlock;
    pAst->pRhs = pElseBlock;
    
    return pAst;
}

Ast * ast_create_while(Ast * pCondition, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_While, pParseData);

    pAst->pLhs = pCondition;
    ast_add_children(pAst, pBlock->pChildren);
    
    return pAst;
}

Ast * ast_create_dowhile(Ast * pCondition, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_DoWhile, pParseData);

    pAst->pLhs = pCondition;
    ast_add_children(pAst, pBlock->pChildren);
    
    return pAst;
}

Ast * ast_create_for(Ast * pInit, Ast * pCondition, Ast * pFin, Ast * pBlock, ParseData * pParseData)
{
    Ast * pAst = ast_create(kAST_For, pParseData);

    pAst->pLhs = pInit;
    pAst->pMid = pCondition;
    pAst->pRhs = pFin;

    ast_add_children(pAst, pBlock->pChildren);
    
    return pAst;
}

Ast * ast_append(AstType astType, Ast * pAst, Ast * pAstNew, ParseData * pParseData)
{
    if (!pAst)
    {
        pAst = ast_create(astType, pParseData);
    }
    pAst->pChildren = astlist_append(pAst->pChildren, pAstNew);
    return pAst;
}

Ast * ast_add_child(Ast * pParent, Ast * pChild)
{
    ASSERT(pParent);
    ASSERT(pChild);
    pParent->pChildren = astlist_append(pParent->pChildren, pChild);
    pChild->pParent = pParent;
    return pParent;
}

Ast * ast_add_children(Ast * pParent, AstList * pChildren)
{
    ASSERT(pParent);
    ASSERT(pChildren);
    ASSERT(pParent->pChildren == nullptr);

    for (Ast * pAstIt : pChildren->nodes)
    {
        pAstIt->pParent = pParent;
    }
    pParent->pChildren = pChildren;
    return pParent;
}

//------------------------------------------------------------------------------
// Ast (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// ParseData
//------------------------------------------------------------------------------
ParseData * parsedata_create(const char * filename, MessageHandler messageHandler)
{
    ParseData * pParseData = COMP_NEW(ParseData);

    pParseData->pRootSymTab = symtab_create();
    pParseData->symTabStack.push_back(pParseData->pRootSymTab);

    pParseData->pRootAst = ast_create(kAST_Root, pParseData);

    pParseData->pRootSymTab->pAst = pParseData->pRootAst;

    pParseData->pScanner = nullptr;

    pParseData->filename = filename;
    pParseData->messageHandler = messageHandler;

    return pParseData;
}

void parsedata_destroy(ParseData * pParseData)
{
    ASSERT(pParseData);
    if (pParseData->pScanner)
    {
        yylex_destroy(pParseData->pScanner);
    }
}

void * parsedata_scanner(ParseData * pParseData)
{
    ASSERT(pParseData);
    return pParseData->pScanner;
}

Ast * parsedata_add_def(ParseData *pParseData, Ast * pAst)
{
    ast_add_child(pParseData->pRootAst, pAst);
    pAst->pSymTab = parsedata_pop_scope(pParseData);
    return pAst;
}

SymTab* parsedata_add_symbol(ParseData * pParseData, SymTab* pSymTab, SymRec * pSymRec)
{
    ASSERT(pParseData);
    if (pSymTab == nullptr)
    {
        pSymTab = symtab_create();
        if (!parsedata_push_scope(pParseData, pSymTab))
        {
            COMP_ERROR("Failed to parsedata_push_scope");
            return pSymTab;
        }
    }
    if (pSymRec)
    {
        if (!symtab_add_symbol(pSymTab, pSymRec, pParseData))
        {
            COMP_ERROR("Failed to symtab_add_symbol: %s", pSymRec->name);
            return pSymTab;
        }
    }
    return pSymTab;
}

SymRec* parsedata_find_symbol(ParseData * pParseData, const char * name)
{
    ASSERT(pParseData);
    ASSERT(name);
    return symtab_find_symbol(pParseData->symTabStack.back(), name);
}

Ast* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec)
{
    ASSERT(pParseData);
    ASSERT(pParseData->symTabStack.size() >= 1);
    ASSERT(pSymRec);

    Ast * pAst = ast_create(kAST_SymbolDecl, pParseData);
    pAst->pSymRec = pSymRec;

    SymTab * pSymTab = pParseData->symTabStack.back();

    if (!symtab_add_symbol(pSymTab, pSymRec, pParseData))
    {
        COMP_ERROR("Failed to symtab_add_symbol: %s", pSymRec->name);
        pSymTab;
    }

    return pAst;
}

SymTab* parsedata_current_scope(ParseData * pParseData)
{
    ASSERT(pParseData);
    ASSERT(pParseData->symTabStack.size() >= 1);

    return pParseData->symTabStack.back();
}


SymTab* parsedata_push_scope(ParseData * pParseData, SymTab * pSymTab)
{
    ASSERT(pSymTab);
    ASSERT(pParseData->symTabStack.size() >= 1);

    if (!pSymTab)
        pSymTab = symtab_create();

    if (pParseData->symTabStack.size() < 1)
    {
        COMP_ERROR("Empty symbol scope stack");
        return pSymTab;
    }

    // make pParseData a child of the top of the stack
    pSymTab->pParent = pParseData->symTabStack.back();
    pSymTab->pParent->children.push_back(pSymTab);

    pParseData->symTabStack.push_back(pSymTab);
    return pSymTab;
}

SymTab * parsedata_pop_scope(ParseData * pParseData)
{
    ASSERT(pParseData->symTabStack.size() >= 1);

    SymTab * pSymTab = pParseData->symTabStack.back();
    pParseData->symTabStack.pop_back();
    return pSymTab;
}

const char * parsedata_add_string(ParseData * pParseData, const char * str)
{
    ASSERT(pParseData);
    return pParseData->strings.emplace(str).first->c_str();
}

void parsedata_formatted_message(ParseData * pParseData,
                                 MessageType messageType,
                                 const char * format, ...)
{
    static const size_t kMessageMax = 1024;
    static thread_local char tMessage[kMessageMax];

    va_list argptr;
    va_start(argptr, format);

    if (format && format[0] != '\0')
    {
        int ret = vsnprintf(tMessage, kMessageMax-1, format, argptr);

        if (ret <= 0)
        {
            strncpy(tMessage, "yyerror was unable to format message", kMessageMax-1);
        }
    }
    else
        tMessage[0] = '\0';

    va_end(argptr);

    tMessage[kMessageMax-1] = '\0';

    pParseData->messageHandler(messageType,
                               tMessage,
                               pParseData->filename,
                               pParseData->line,
                               pParseData->column);
}

void parsedata_set_location(ParseData * pParseData,
                            int line,
                            int column)
{
    pParseData->line = line;
    pParseData->column = column;
}

//------------------------------------------------------------------------------
// ParseData (END)
//------------------------------------------------------------------------------

void parse_init()
{
    comp_reset_mem();

    //yydebug = 1;
}

ParseData * parse(ParseData * pParseData,
                  const char * source,
                  size_t length,
                  const char * filename,
                  MessageHandler messageHandler)
{
    int ret;

    if (!pParseData)
        pParseData = parsedata_create(filename, messageHandler);

    ret = yylex_init_extra(pParseData, &pParseData->pScanner);
    if (ret != 0)
    {
        return nullptr;
    }

    yy_scan_bytes(source, length, pParseData->pScanner);

    ret = yyparse(pParseData);
    if (ret != 0)
    {
        return nullptr;
    }

    return pParseData;
}   

ParseData * parse_file(ParseData * pParseData,
                       const char * filename,
                       MessageHandler messageHandler)
{
    char * source = nullptr;
    i32 length = read_file(filename, &source);
    if (length <= 0)
    {
        PANIC("Unable to read file");
        return nullptr;
    }    

    return parse(nullptr, source, length, filename, messageHandler);
}

void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...)
{
    static const size_t kMessageMax = 1024;
    static thread_local char tMessage[kMessageMax];

    va_list argptr;
    va_start(argptr, format);
    parsedata_formatted_message(pParseData, kMSGT_Error, format, argptr);
    va_end(argptr);
}


void *yyalloc(yy_size_t size, yyscan_t yyscanner)
{
	return COMP_ALLOC(size);
}

void *yyrealloc(void * ptr, yy_size_t size, yyscan_t yyscanner)
{
	return COMP_REALLOC(ptr, size);
}

void yyfree (void * ptr , yyscan_t yyscanner)
{
    // no freeing with comp_mem.h manager
}
