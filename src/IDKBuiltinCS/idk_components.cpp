#include "idk_components.hpp"
#include "sys-physx.hpp"

int
idk::ScriptCmp_getNumParams( idk::ScriptCmp &cmp )
{
    lua_State *L = cmp.L;

    if (luaL_dofile(L, cmp.filepath) != 0)
    {
        std::cerr << "Error loading Lua script: " << lua_tostring(L, -1) << std::endl;
    }

    lua_getglobal(L, "ScriptEntry");

    if (lua_isfunction(L, -1) == false)
    {
        std::cerr << "Error: 'ScriptEntry' is not a Lua function" << std::endl;
        lua_pop(L, 1);
    }

    lua_Debug ar;
    lua_pushvalue(L, -1);
    lua_getinfo(L, ">u", &ar);
    std::cout <<  int(ar.nparams) << "\n";

    return int(ar.nparams);
}

