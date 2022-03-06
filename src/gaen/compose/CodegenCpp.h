//------------------------------------------------------------------------------
// CodegenCpp.h - C++ code generation
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_COMPOSE_CODEGEN_CPP_H
#define GAEN_COMPOSE_CODEGEN_CPP_H

#include "gaen/core/String.h"
#include "gaen/core/Set.h"

#include "gaen/compose/compiler.h"
#include "gaen/compose/compiler_structs.h"
#include "gaen/compose/codegen_utils.h"

namespace gaen
{

typedef String<kMEM_Compose> S;

struct CodeCpp
{
    S cmpFilename;
    S cmpRelPath;
    S cmpFullPath;
    S cppFilename;
    S cppRelPath;
    S hFilename;
    S hRelPath;
    S header;
    S code;
};

class CodegenCpp
{
public:
    CodeCpp codegen(ParseData * pParseData);
private:
    Set<kMEM_Compose, S> mHashes;

    S indent(u32 level);
    const char * cellFieldStr(const SymDataType * pSdt, ParseData * pParseData);
    void encodeString(char * enc, size_t encSize, const char * str);
    S propertyBlockAccessor(const SymDataType * pSdt, const BlockInfo & blockInfo, const char * blockVarName, bool isConst, ParseData * pParseData);
    S messageBlockAccessor(const SymDataType * pSdt, const BlockInfo & blockInfo, const char * blockVarName, bool isConst, ParseData * pParseData);
    S binaryOp(const Ast * pAst, const char * op);
    S unaryOp(const Ast * pAst, const char* op);
    S unaryOpPost(const Ast * pAst, const char* op);
    S assign(const Ast * pAst, const char * op);
    S messageParamSymref(SymRec * pSymRec);
    S symref(const Ast * pAst, SymRec * pSymRec, ParseData * pParseData);
    S hashLiteral(const char * str);
    S floatLiteral(f32 val);
    S setPropertyHandlers(const Ast * pAst, int indentLevel);
    S dataTypeInitValue(const SymDataType * pSdt, ParseData * pParseData);
    S initData(const Ast * pAst, int indentLevel, ScriptDataCategory dataCategory);
    S initAssets(const Ast * pAst, int indentLevel);
    S assetReady(const Ast * pAst, int indentLevel);
    S fin(const Ast * pAst, int indentLevel);
    S initializationMessageHandlers(const Ast * pAst, const S & postInit, const S & postInitIndependent, const S & postInitDependent, int indentLevel);
    S messageDef(const Ast * pAst, int indentLevel);
    S codegenInitProperties(Ast * pAst, SymTab * pPropsSymTab, const char * taskName, const char * scriptTaskName, ScriptDataCategory dataCategory, int indentLevel);
    S codegenInitParentTask(const Ast * pAst);
    S codegenReadyMessage(const Ast * pAst);
    S entityInitClass(const char * str);
    S entityInitFunc(const char * str);
    const Ast * definingParent(const Ast * pAst);
    const void findRefsRecurse(SymTab * pSymTab, const Ast * pAst);
    const SymTab * findClosureSymbols(const Ast * pAst);
    S closureParams(const SymTab * pClosure);
    S closureArgs(const SymTab * pClosure);
    S closureMemberInit(const SymTab * pClosure, u32 indentLevel);
    S closureMemberDecl(const SymTab * pClosure, u32 indentLevel);
    S codegenHelperFuncsRecurse(const Ast * pAst);
    S codegenHelperFuncs(const Ast * pAst);
    bool isTopLevelFunction(const Ast * pFuncAst);
    S functionPrototype(const Ast * pFuncAst);
    S updateDef(const Ast * pUpdateDef, const Ast * pInput, u32 indentLevel);
    S inputHandlerName(const char * mode, const char * handler);
    S inputHandlerSpecialName(const char * mode, const char * handler);
    S inputBlock(const Ast * pRoot, u32 indentLevel);
    S codegenMessageParams(const Ast * pAst, int indentLevel);
    S codegenRecurse(const Ast * pAst, int indentLevel);
    S codegenHeader(const Ast * pRootAst);
    void extractFilenames(const S & cmpFullPath, CodeCpp & codeCpp, ParseData * pParseData);
    S hashRegistration(const S & entName);
};

} // namespace gaen

#endif // #ifndef GAEN_COMPOSE_CODEGEN_CPP_H
