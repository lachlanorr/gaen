#include "<<PROJECT_NAME>>_lib/stdafx.h"

#include "<<PROJECT_NAME>>_lib/<<PROJECT_NAME>>_api.h"

namespace gaen
{

namespace system_api
{

// Sample, trivial system_api function.
// See declaration in <<PROJECT_NAME>>_api.h for more details.
u32 fibonacci(u32 n, Entity & caller)
{
    u32 v = 0;
    for (u32 i = 1; i <= n; ++i)
        v += i;
    return v;
}

} // namespace system_api

} // namespace gaen
