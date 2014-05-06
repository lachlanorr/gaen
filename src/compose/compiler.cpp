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


#include "core/mem.h"

#include "compose/compiler.h"

#include "compose/parser.h"
#define YY_NO_UNISTD_H
#include "compose/scanner.h"

using namespace gaen;

struct SymRec
{

};

struct SymTab
{

};

struct Ast
{

};

struct ParseData
{
    Ast * pRoot;
    SymTab * pSymTab;
    void * pScanner;
};


//------------------------------------------------------------------------------
// SymRec
//------------------------------------------------------------------------------
SymRec * symrec_create(const char * name)
{
    return nullptr;
}

void symrec_destroy(SymRec* pSymRec)
{
}
//------------------------------------------------------------------------------
// SymRec (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// SymTab
//------------------------------------------------------------------------------
SymTab* symtab_create()
{
    return nullptr;
}

void symtab_destroy(SymTab* pSymTab)
{
}

void symtab_add_entry(SymTab* pSymTab, SymRec * pSymRec)
{
}

void symtab_push_scope(SymTab* pSymTab)
{
}

void symtab_pop_scope(SymTab* pSymTab)
{
}
//------------------------------------------------------------------------------
// SymTab (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Ast
//------------------------------------------------------------------------------
Ast * ast_create(AstType astType)
{
    return nullptr;
}

void ast_destroy(Ast * pAst)
{
}
//------------------------------------------------------------------------------
// Ast (END)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// ParseData
//------------------------------------------------------------------------------
ParseData * parsedata_create()
{
    return nullptr;
}

void parsedata_destroy(ParseData * pParseData)
{
}

void * parsedata_scanner(ParseData * pParseData)
{
    return pParseData->pScanner;
}
//------------------------------------------------------------------------------
// ParseData (END)
//------------------------------------------------------------------------------



ParseData * parse(const char * source, size_t length)
{
    int ret;
    ParseData * pParseData = parsedata_create();

    ret = yylex_init_extra(pParseData, &pParseData->pScanner);
    if (ret != 0)
    {
        GFREE(pParseData);
        return nullptr;
    }

    yy_scan_bytes(source, length, pParseData->pScanner);

    ret = yyparse(pParseData);
    if (ret != 0)
    {
        GFREE(pParseData);
        return nullptr;
    }

    return pParseData;
}   

void yyerror(YYLTYPE * pLoc, ParseData * pParseData, const char * msg)
{
    fprintf(stderr, "%s\n", msg);
}


