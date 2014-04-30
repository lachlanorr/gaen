//------------------------------------------------------------------------------
// main_gac.cpp - Gaen console program for logging and sending commands
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

extern "C"
{
#define YYPARSE_PARAM
#define YY_NO_UNISTD_H
#include "compose/parser.h"
#include "compose/scanner.h"
}

/*extern "C" 
{
YY_BUFFER_STATE yy_scan_string(yyconst char *yy_str, yyscan_t yyscanner);
int yylex_init(yyscan_t* scanner);
int yyparse(void *scanner);
}
*/
namespace gaen
{



} // namespace gaen

int main(int argc, char ** argv)
{
    int ret;
    yyscan_t scanner;
    ret = yylex_init(&scanner);
    YY_BUFFER_STATE state;
    state = yy_scan_string("abc = 5", scanner);
    ret = yyparse(scanner);
    
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

    printf("compc running\n");
}
