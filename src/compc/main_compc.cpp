//------------------------------------------------------------------------------
// main_compc.cpp - Compose command line compiler
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

#include <cstdio>

#include "core/base_defines.h"
#include "core/mem.h"

#include "compose/compiler.h"

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
        printf(message);
        printf("\n");
        break;
    case kMSGT_Warning:
        printf("%s(%d:%d): WARNING - %s\n", filename, line, column, message);
        break;
    case kMSGT_Error:
        printf("%s(%d:%d): ERROR - %s\n", filename, line, column, message);
        break;
    }
}

} // namespace gaen

int main(int argc, char ** argv)
{
    using namespace gaen;

    parse_init();
    ParseData * pParseData = parse_file(nullptr,
                                        "C:/code/gaen/src/compc/test_programs/prog2.cmp",
                                        &messageHandler);

/*    bool shouldLogListen = false;
    
    // parse args
    for (int i = 1; i < argc; ++i)
    {
        if (0 == strcmp(argv[i], "-l"))
        {
            shouldLogListen = true;
        }
    }


    if (shouldLogListen)
        log_listen_and_print();
*/
    char c = getchar();
}
