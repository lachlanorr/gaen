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

#include "core/base_defines.h"
#include "core/mem.h"

#include "compose/compiler.h"

namespace gaen
{

i32 read_file(const char * path, char ** output)
{
    *output = nullptr;
    i32 length = 0;
    char * source = nullptr;
    FILE *fp = fopen(path, "r");

    if (!fp)
    {
        ERR("Failed to read file: %s", path);
        return -1;
    }

    // go to end of file
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
        // get sizeof file
        long bufsize = ftell(fp);
        if (bufsize == -1)
        {
            ERR("Invalid file size: %s", path);
            fclose(fp);
            return -1;
        }

        source = static_cast<char*>(GALLOC(kMEM_Unspecified, (bufsize + 1)));

        // seek backto start
        if (fseek(fp, 0L, SEEK_SET) != 0)
        {
            ERR("Unable to seek back to start: %s", path);
            GFREE(source);
            fclose(fp);
            return -1;
        }

        length = static_cast<i32>(fread(source, sizeof(char), bufsize, fp));
        if (length == 0)
        {
            ERR("Error reading file: %s", path);
            GFREE(source);
            fclose(fp);
            return -1;
        }
        else
        {
            source[length+1] = '\0';
        }
    }
    fclose(fp);

    *output = source;
    return length;
}

} // namespace gaen

int main(int argc, char ** argv)
{
    using namespace gaen;

    char * source = nullptr;
    i32 length = gaen::read_file("C:/code/gaen/src/compc/test_programs/prog0.cmp", &source);
    ASSERT(length > 0);

    parse_init();
    ParseData * pParseData = parse(source, length);


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
