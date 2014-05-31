//------------------------------------------------------------------------------
// codegen_utils.cpp - Shared utilities used during code generation
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

#include "compose/codegen_utils.h"

namespace gaen
{

bool is_update_message_def(const Ast * pAst)
{
    return (pAst->type == kAST_MessageDef &&
            pAst->pSymRec &&
            0 == strcmp(pAst->pSymRec->name, "update"));
}

const Ast * find_update_message_def(const Ast * pAst)
{
    for (Ast * pChild : pAst->pChildren->nodes)
    {
        if (is_update_message_def(pChild))
        {
            return pChild;
        }
    }
    return nullptr;
}

u32 props_and_fields_count(const Ast * pAst)
{
    u32 count = 0;
    for (Ast *pChild : pAst->pChildren->nodes)
    {
        if (pChild->type == kAST_PropertyDef || pChild->type == kAST_FieldDef)
            count++;
    }
    return count;
}

PropsAndFields build_props_and_fields(const Ast *pAst)
{
    u32 count = props_and_fields_count(pAst);
    PropsAndFields propsAndFields(count);

    for (Ast *pChild : pAst->pChildren->nodes)
    {
        if (pChild->type == kAST_PropertyDef)
        {
            
        }
        else if (pChild->type == kAST_FieldDef)
        {
        }
    }
    return propsAndFields;
}

} // namespace gaen


