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


using namespace gaen;

//------------------------------------------------------------------------------
// C Fuctions, used by Bison/Flex code
//------------------------------------------------------------------------------
ast_node * ast_new(int tokenType)
{
    return GNEW(kMEM_Compose, AstNode, static_cast<TokenType>(tokenType));
}

void ast_delete(ast_node * pAstNode)
{
    GDELETE(AstNode::convert(pAstNode));
}


sym_record * sr_new(const char * name)
{
    return GNEW(kMEM_Compose, SymRecord, name);
}

void sr_delete(sym_record* pSymRec)
{
    GDELETE(SymRecord::convert(pSymRec));;
}


sym_table* st_new()
{
    return GNEW(kMEM_Compose, SymTable);
}

void st_delete(sym_table* pSymTab)
{
    GDELETE(SymTable::convert(pSymTab));
}

void st_add_entry(sym_table* pSymTab, sym_record * pSymRec)
{
    SymTable::convert(pSymTab)->addEntry(SymRecord::convert(pSymRec));
}

void st_push_scope(sym_table* pSymTab)
{
    SymTable::convert(pSymTab)->pushScope();
}

void st_pop_scope(sym_table* pSymTab)
{
    SymTable::convert(pSymTab)->popScope();
}
//------------------------------------------------------------------------------
// C Fuctions (END)
//------------------------------------------------------------------------------


namespace gaen
{

//------------------------------------------------------------------------------
// SymTable
//------------------------------------------------------------------------------
void SymTable::pushScope()
{

}

void SymTable::popScope()
{

}

void SymTable::addEntry(SymRecord * pSymRecord)
{

}
//------------------------------------------------------------------------------
// SymTable (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Program
//------------------------------------------------------------------------------
Program::Program(AstNode * pRoot, SymTable * pSymTable)
  : mpRoot(pRoot)
  , mpSymTable(pSymTable)
{
}

Program::~Program()
{
    GDELETE(mpRoot);
    GDELETE(mpSymTable);
}
//------------------------------------------------------------------------------
// Program (END)
//------------------------------------------------------------------------------

} // namespace gaen

