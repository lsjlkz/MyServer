//
// Created by lsjlkz on 2023/5/17.
//

#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#define LUA_PACKAGE_META "LUA_PACK_META"

#define LUA_ERROR(_L, _b, _msg) \
	if(!(_b)){ \
		luaL_error(_L, _msg); \
	}

#define LUA_ASSERT(_b)\
	if(!(_b)){\
		abort();\
	}

// check first is pack
#define LUA_CHECK_BUF_TYPE(L) \
	((LuaPackBuf*) luaL_checkudata(L, 1, LUA_PACKAGE_META))


// check second
#define LUA_CHECK_TYPE_CAN_PACK(L) \
	(LUA_CHECK_LAST_ARG_TYPE(L,LUA_TSTRING)||LUA_CHECK_LAST_ARG_TYPE(L,LUA_TTABLE)||LUA_CHECK_LAST_ARG_TYPE(L,LUA_TNIL)||LUA_CHECK_LAST_ARG_TYPE(L,LUA_TBOOLEAN)||LUA_CHECK_LAST_ARG_TYPE(L,LUA_TNUMBER))

#define LUA_CHECK_LAST_ARG_TYPE(L,t) \
	(lua_type(L, -1) == t)

#define SHORT_FLAG				1
#define INT_FLAG				2
#define LONG_FLAG				3
#define LONG_LONG_FLAG			4
#define NIL_FLAG				5
#define TRUE_FLAG				6
#define FALSE_FLAG				7
#define TABLE_FLAG				8
#define STRING_FLAG				9


#define LUA_TYPE_FLAG char

#define SHORT short
#define INT int
#define LONG long
#define LONG_LONG long long
#define U_SHORT unsigned short

#define TABLE_SIZE_TYPE U_SHORT

#define MAX_STACK_DEEP			30

#define MAX_BUF_SIZE 65535

typedef struct {
	size_t write_size;
	size_t read_size;
	char buf[MAX_BUF_SIZE];
} LuaPackBuf;


static int stackDeep = 0;

static LuaPackBuf globalIO;


LUA_API int lua_push_buf_to_stack(lua_State* L, void* pHead, size_t size) {
	LUA_ERROR(L, size <= MAX_BUF_SIZE, "to long buf");
	stackDeep = 0;
	memcpy(globalIO.buf, pHead, size);
	globalIO.write_size = size;
	globalIO.read_size = 0;
	unpack_to_arg_help(L, &globalIO);
	return 1;
}

LUA_API int lua_pack_top_to_buf(lua_State* L, void** pHead) {
	stackDeep = 0;
	globalIO.write_size = 0;
	globalIO.read_size = 0;
	LuaPackBuf* io = &globalIO;
	*pHead = io;
	io->write_size = 0;
	if (!LUA_CHECK_TYPE_CAN_PACK(L)) {
		return 0;
	}
	pack_arg_help(L, io, -1);
	return 1;
}


LUA_API int pack_arg(lua_State* L);

LUA_API int unpack_to_arg(lua_State* L);

LUA_API int print(lua_State* L);

LUA_API int new_pack(lua_State* L);

LUA_API int obj_len(lua_State* L);

static luaL_Reg pack_functions[] = {
		{"new", new_pack},
		{"pack", pack_arg},
		{"unpack", unpack_to_arg},
		{NULL, NULL}
};



static luaL_Reg pack_methods[] = {
		{"__len", obj_len},
		{"print", print},
		{NULL, NULL}
};
