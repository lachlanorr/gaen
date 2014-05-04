//------------------------------------------------------------------------------
// compiler_utils.h - C utils for use in Bison/Flex parser (wraps C++)
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

#ifndef GAEN_COMPOSE_COMPILER_UTILS_H
#define GAEN_COMPOSE_COMPILER_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

//#define YYPARSE_PARAM
//#include "compose/parser.h"

// C Declarations, to be used from Bison

typedef struct ast_node ast_node;
typedef struct sym_record sym_record;
typedef struct sym_table sym_table;


ast_node * ast_new(int tokenType);
void ast_delete(ast_node * pAstNode);

sym_record * sr_new(const char * name);
void sr_delete(sym_record* pSymRec);

sym_table* st_new();
void st_delete(sym_table* pSymTab);
void st_add_entry(sym_table* pSymTab, sym_record * pSymRec);
void st_push_scope(sym_table* pSymTab);
void st_pop_scope(sym_table* pSymTab);


#ifdef __cplusplus
} // #ifdef __cplusplus

#endif

#endif // #ifndef GAEN_COMPOSE_COMPILER_UTILS_H
