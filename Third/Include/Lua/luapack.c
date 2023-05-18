//
// Created by lsjlkz on 2023/5/17.
//

#include "luapack.h"

static int stackDeep = 0;

#define LUA_ERROR(__L, __b, __msg) \
	if(!(__b)){ \
		luaL_error(__L, __msg); \
	}

#define LUA_CHECK_OVERFLOW(__L, __io, __size) LUA_ERROR(L, (BUF_EMPTY((__io)) > (__size)), "err pack overflow")


#define CPY_ADD_SIZE(io, value) \
	memcpy((io->buf + io->size), &value, (sizeof(value)));\
	io->size = (io->size + sizeof(value));


LUA_API int pack_arg(lua_State* L){
	stackDeep = 0;
	Buf *io = (Buf*)LUA_CHECK_BUF_TYPE(L);
	if(!LUA_CHECK_TYPE_CAN_PACK(L)){
		return 0;
	}
	pack_arg_help(L, io, -1);
	return 1;
}


LUA_API int new_pack(lua_State* L) {
	Buf* io = (Buf * )lua_newuserdata(L, sizeof(Buf));
	io->size = 0;

	luaL_getmetatable(L, LUA_PACKAGE_META);
	lua_setmetatable(L, -2);
	return 1;
}




LUA_API int obj_len(lua_State* L) {
	Buf* io = (Buf*)LUA_CHECK_BUF_TYPE(L);
	lua_pushinteger(L, io->size);
	return 1;
}




//



void pack_type(Buf* io, char value) {
	CPY_ADD_SIZE(io, value);
}

void pack_number(Buf* io, NUMBER value) {
	// TODO this can judge short int long and longlong
	CPY_ADD_SIZE(io, value);
}

void pack_string(Buf* io, const char* s, size_t size) {
	memcpy(io->buf + io->size, s, size);
	io->size += size;
}

//TABLE_SIZE_TYPE* pack_size(Buf* io, TABLE_SIZE_TYPE size) {
//	TABLE_SIZE_TYPE* p = io->buf + io->size;
//	*p = size;
//	io->size += sizeof(TABLE_SIZE_TYPE);
//	return p;
//}

void pack_size(Buf* io, TABLE_SIZE_TYPE size) {
	TABLE_SIZE_TYPE* p = io->buf + io->size;
	*p = size;
	io->size += sizeof(TABLE_SIZE_TYPE);
}






int pack_arg_help(lua_State* L, Buf* io, int index) {
	stackDeep++;
	int top = lua_gettop(L);
	int lt = lua_type(L, index);
	switch (lt) {
	case(LUA_TNIL):
	{
		LUA_CHECK_OVERFLOW(L, io, sizeof(LUA_TYPE_FLAG));
		pack_type(io, NIL_FLAG);
		break;
	}
	case(LUA_TBOOLEAN):
	{
		LUA_CHECK_OVERFLOW(L, io, sizeof(LUA_TYPE_FLAG));
		int b = lua_toboolean(L, index);
		if (b) {
			pack_type(io, TRUE_FLAG);
		}
		else {
			pack_type(io, FALSE_FLAG);
		}
		break;
	}
	case(LUA_TSTRING): {
		LUA_CHECK_OVERFLOW(L, io, sizeof(LUA_TYPE_FLAG));
		pack_type(io, STRING_FLAG);

		size_t size = 0;
		const char* s = lua_tolstring(L, index, &size);
		LUA_CHECK_OVERFLOW(L, io, size);
		pack_string(io, s, size);
		break;
	}
	case(LUA_TNUMBER):
	{

		LUA_CHECK_OVERFLOW(L, io, sizeof(LUA_TYPE_FLAG));
		pack_type(io, NUMBER_FLAG);

		NUMBER value = lua_tointeger(L, index);
		LUA_CHECK_OVERFLOW(L, io, sizeof(NUMBER));
		pack_number(io, value);
		return 1;
	}
	case(LUA_TTABLE):
	{
		LUA_CHECK_OVERFLOW(L, io, sizeof(LUA_TYPE_FLAG));
		pack_type(io, TABLE_FLAG);
		// table size
		int table_index = lua_absindex(L, index);
		int table_size = luaL_len(L, table_index);
		LUA_ERROR(L, table_size <= USHRT_MAX, "table size over flow");
		// table size must lt unsigned short max
		TABLE_SIZE_TYPE usize = (TABLE_SIZE_TYPE)table_size;
		//TABLE_SIZE_TYPE* p = pack_size(io, usize);
		pack_size(io, usize);
		lua_checkstack(L, top + 2);
		// first key
		lua_pushnil(L);
		// push key and value
		while (lua_next(L, table_index)) {
			// pack key
			pack_arg_help(L, io, -2);
			// pack value
			pack_arg_help(L, io, -1);
			// pop value
			lua_pop(L, 1);
		}
		break;
	}
	stackDeep--;
	return 1;
	}
}