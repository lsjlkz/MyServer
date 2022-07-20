//
// Created by lsjlkz on 2021/12/23.
//

#include "Vector2.h"

Vector2::Vector2(Vector2 const &xy) {
    _x = xy._x;
    _y = xy._y;
}

Vector2::Vector2() {
    _x = 0;
    _y = 0;
}

int lua_get_vector(lua_State* L){
	Vector2** vector = (Vector2**) luaL_checkudata(L, 1, "Vector2");
	luaL_argcheck(L, vector != nullptr, 1, "invalid Vector2");
	lua_settop(L, 0);
	lua_pushnumber(L, (*vector)->GetX());
	lua_pushnumber(L, (*vector)->GetY());
	return 2;
}

int lua_create_vector(lua_State* L){
	luaL_checktype(L, 1, LUA_TNUMBER);
	luaL_checktype(L, 2, LUA_TNUMBER);
	Vector2** vectorRef = (Vector2**) lua_newuserdata(L, sizeof(Vector2*));
	int x = (int) lua_tointeger(L, 1);
	int y = (int) lua_tointeger(L, 2);
	lua_settop(L, 0);
	*vectorRef = new Vector2(x, y);
	luaL_getmetatable(L, "Vector2");
	lua_setmetatable(L, -2);
	return 1;

}

int luaopen_vector_libs(lua_State* L){
	//创建全局元表，如果LUA_REGISTRYINDEX注册表没有这个元表，创建一个新表添加到注册表中
	luaL_newmetatable(L, "Vector2");
	//将元表作为一个副本压栈到位置-1，刚刚创建的元表位置-2
	lua_pushvalue(L, -1);

	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, lua_reg_vector_funcs, 0);
	// 创建新表，压函数
	luaL_newlib(L, lua_reg_vector_create_funcs);
	return 1;
}