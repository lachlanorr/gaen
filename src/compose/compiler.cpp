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

#include "compose/comp_mem.h"

#include "compose/compiler.h"

extern "C" {
#include "compose/compose_parser.h"
#define YY_NO_UNISTD_H
#include "compose/compose_scanner.h"
}

using namespace gaen;

struct SymRec
{
    SymbolScope scope;
    yytokentype type;
    const char * name;
    Ast * pValue;
    SymTab * pSymTab;
};

struct SymTab
{
    SymTab * pParent;
    Ast * pAst;
    CompHashMap<CompString, SymRec*> dict;
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

    Ast* pLhs;
    Ast* pRhs;

    AstList* pChildren;
};

struct ParseData
{
    Ast* pRootAst;
    SymTab* pRootSymTab;
    void * pScanner;
    CompList<SymTab*> symTabStack;
    CompHashSet<CompString> strings;
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
SymRec * symrec_create(SymbolScope scope,
                       enum yytokentype type,
                       const char * name,
                       Ast * pValue)
{
    SymRec * pSymRec = COMP_NEW(SymRec);

    pSymRec->scope = scope;
    pSymRec->type = type;
    pSymRec->name = name;
    pSymRec->pValue = pValue;

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

SymTab* symtab_add_record(SymTab* pSymTab, SymRec * pSymRec)
{
    ASSERT(pSymTab && pSymRec);
    ASSERT(pSymRec->pSymTab == nullptr);

    pSymRec->pSymTab = pSymTab;
    pSymTab->dict[pSymRec->name] = pSymRec;
    return pSymTab;
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
    if (pAstList == nullptr)
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
    return pAst;
}

Ast * ast_create_message_def(AstList * pStmtList, ParseData * pParseData)
{
    ASSERT(pStmtList);
    ASSERT(pParseData);

    Ast * pAst = ast_create(kAST_MessageDef, pParseData);
    ast_add_child(pParseData->pRootAst, pAst);

    parsedata_pop_scope(pParseData);

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

Ast * ast_add_child(Ast * pAst, Ast * pChild)
{
    ASSERT(pAst);
    ASSERT(pChild);
    pAst->pChildren = astlist_append(pAst->pChildren, pAst);
    pChild->pParent = pAst;
    return pAst;
}

Ast * ast_add_children(Ast * pAst, AstList * pChildren)
{
    ASSERT(pAst);
    ASSERT(pChildren);
    //for (auto it = pChildren->nodes.begin(); it != pChildren->nodes.end(); ++it)
    for (Ast * pAstIt : pChildren->nodes)
    {
        pAstIt->pParent = pAst;
    }
    pAst->pChildren = pChildren;
    return pAst;
}

//------------------------------------------------------------------------------
// Ast (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// ParseData
//------------------------------------------------------------------------------
ParseData * parsedata_create()
{
    ParseData * pParseData = COMP_NEW(ParseData);

    pParseData->pRootSymTab = symtab_create();
    pParseData->symTabStack.push_back(pParseData->pRootSymTab);

    pParseData->pRootAst = ast_create(kAST_Root, pParseData);

    pParseData->pScanner = nullptr;

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
            PANIC("Failed to parsedata_push_scope");
            return nullptr;
        }
    }
    if (pSymRec)
    {
        if (!symtab_add_record(pSymTab, pSymRec))
        {
            PANIC("Failed to symtab_add_record");
        }
    }
    return pSymTab;
}

SymTab* parsedata_add_local_symbol(ParseData * pParseData, SymRec * pSymRec)
{
    ASSERT(pParseData);
    ASSERT(pParseData->symTabStack.size() >= 1);
    ASSERT(pSymRec);

    SymTab * pSymTab = pParseData->symTabStack.back();

    if (!symtab_add_record(pSymTab, pSymRec))
    {
        PANIC("Failed to symtab_add_record");
    }

    return pSymTab;
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

    if (pParseData->symTabStack.size() < 1)
    {
        PANIC("Empty symbol scope stack");
        return nullptr;
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
    if (pParseData->symTabStack.size() <= 1)
    {
        PANIC("parsedata_pop_scope of empty stack");
        return nullptr;
    }
    SymTab * pSymTab = pParseData->symTabStack.back();
    pParseData->symTabStack.pop_back();
    return pSymTab;
}

const char * parsedata_add_string(ParseData * pParseData, const char * str)
{
    ASSERT(pParseData);
    return pParseData->strings.emplace(str).first->c_str();
}

//------------------------------------------------------------------------------
// ParseData (END)
//------------------------------------------------------------------------------

void parse_init()
{
    comp_reset_mem();
}

ParseData * parse(const char * source, size_t length)
{
    int ret;
    ParseData * pParseData = parsedata_create();

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

void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * format, ...)
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

    fprintf(stderr, "%s\n", tMessage);
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
