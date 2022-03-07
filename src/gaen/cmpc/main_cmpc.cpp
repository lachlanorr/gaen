//------------------------------------------------------------------------------
// main_cmpc.cpp - Compose command line compiler
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

#include <cstdio>

#include "gaen/core/base_defines.h"
#include "gaen/core/mem.h"

#include "gaen/compose/compiler.h"
#include "gaen/compose/compiler_structs.h"
#include "gaen/compose/CodegenCpp.h"
#include "gaen/compose/comp_mem.h"

namespace gaen
{

void messageHandler(MessageType messageType,
                    const char * message,
                    const char * filename,
                    int line,
                    int column)
{
    switch (messageType)
    {
    case kMSGT_Info:
        fprintf(stderr, "%s\n", message);
        break;
    case kMSGT_Warning:
        fprintf(stderr, "%s(%d:%d): WARNING - %s\n", filename, line, column, message);
        break;
    case kMSGT_Error:
        fprintf(stderr, "%s(%d): ERROR - %s\n", filename, line, message);
        exit(1);
        break;
    }
}

} // namespace gaen

void usage_and_exit()
{
    printf("Usage: cmpc [-i api_header] input_cmp_file\n");
    exit(1);
}

int main(int argc, char ** argv)
{
    using namespace gaen;

    if (argc < 2)
    {
        usage_and_exit();
    }

    parse_init();

    CompList<CompString> systemIncludes;

    // parse command line args
    for (i32 i = 1; i < argc - 2; ++i)
    {
        if (argv[i][0] == '-' || argv[i][0] == '/')
        {
            switch (argv[i][1])
            {
            case 'i':
                systemIncludes.emplace_back(argv[i+1]);
                i++;
                break;
            default:
                usage_and_exit();
                break;
            }
        }
    }

    const char * inFile = argv[argc-1];

    ParseData * pParseData = parse_file(inFile,
                                        &systemIncludes,
                                        &messageHandler);

    if (!pParseData || pParseData->hasErrors)
    {
        fprintf(stderr, "Compilation failed due to errors\n");
        exit(1);
    }

    CodeCpp codeCpp;
    if (pParseData)
    {
        CodegenCpp codegen;
        codeCpp = codegen.codegen(pParseData);
    }

    if (codeCpp.header != "")
    {
        puts("/// .H SECTION");
        puts(codeCpp.header.c_str());
    }

    puts("/// .CPP SECTION");
    puts(codeCpp.code.c_str());
}
