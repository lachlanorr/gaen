//------------------------------------------------------------------------------
// codegen_utils.h - Shared utilities used during code generation
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2022 Lachlan Orr
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

#ifndef GAEN_COMPOSE_CODEGEN_UTILS_H
#define GAEN_COMPOSE_CODEGEN_UTILS_H

#include "gaen/compose/compiler.h"
#include "gaen/compose/compiler_structs.h"

namespace gaen
{

enum ScriptDataCategory
{
    kSDC_None,
    kSDC_Asset,
    kSDC_Independent,
    kSDC_Dependent
};

const Ast * find_update(const Ast * pSearchAst);
const Ast * find_input(const Ast * pSearchAst, u32 idx);
const Ast * find_component_members(const Ast * pSearchAst);

u32 calc_cell_count(const Ast * pAst);

inline bool is_prop_or_field(const SymRec * pSymRec)
{
    return (pSymRec && (pSymRec->type == kSYMT_Property || pSymRec->type == kSYMT_Field) && !(pSymRec->flags & kSRFL_Member));
}

ScriptDataCategory data_category(const Ast * pAst);

inline bool is_prop(const SymRec * pSymRec)
{
    return (pSymRec && pSymRec->type == kSYMT_Property && !(pSymRec->flags & kSRFL_Member));
}

inline bool is_prop(const Ast * pAst)
{
    return (pAst && pAst->pSymRec && is_prop(pAst->pSymRec));
}

inline bool is_field(const SymRec * pSymRec)
{
    return (pSymRec && pSymRec->type == kSYMT_Field && !(pSymRec->flags & kSRFL_Member));
}

inline bool is_asset_prop_or_field(const SymRec * pSymRec)
{
    return (pSymRec->flags & kSRFL_AssetRelated) == kSRFL_AssetRelated;
}

BlockInfos * block_pack_props_and_fields(Ast *pAst);
BlockInfos * block_pack_message_params(Ast * pAst);
BlockInfos * block_pack_message_def_params(SymTab * pSymTab, ParseData * pParseData);

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_CODEGEN_UTILS_H

