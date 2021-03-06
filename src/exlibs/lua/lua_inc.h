// ======================================================================================
// File         : lua_inc.h
// Author       : Wu Jie 
// Last Change  : 08/29/2010 | 17:06:47 PM | Sunday,August
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef LUA_INC_H_1283072809
#define LUA_INC_H_1283072809
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#if EX_USE_LUA

// lua
extern "C" {
#include <lua-5.1.4/src/lua.h>
#include <lua-5.1.4/src/lauxlib.h>
#include <lua-5.1.4/src/lualib.h>
}

//
#include "lua_global.h"
#include "lua_proxy.h"

#endif

// #################################################################################
#endif // END LUA_INC_H_1283072809
// #################################################################################


