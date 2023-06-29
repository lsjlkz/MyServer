//
// Created by lsjlkz on 2023/4/14.
//

#ifndef MYSERVER_LUAINCLUDE_H
#define MYSERVER_LUAINCLUDE_H

// 需要加上这一行才行
#pragma comment(lib, "Lua.lib")
#pragma comment(lib, "libmysql.lib")

extern "C"{
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}


#endif //MYSERVER_LUAINCLUDE_H
