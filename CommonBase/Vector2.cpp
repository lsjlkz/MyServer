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

GE::Int32 lua_get_vector(lua_State* L){
	// 检查元表，强制转换成指针的指针
	Vector2** vector = (Vector2**) luaL_checkudata(L, 1, "Vector2");
	luaL_argcheck(L, vector != nullptr, 1, "invalid Vector2");
	lua_settop(L, 0);
	// 通过指针来访问成员函数
	lua_pushnumber(L, (*vector)->GetX());
	lua_pushnumber(L, (*vector)->GetY());
	return 2;
}

GE::Int32 lua_create_vector(lua_State* L){
	luaL_checktype(L, 1, LUA_TNUMBER);
	luaL_checktype(L, 2, LUA_TNUMBER);
	// 创建指针的指针，并压入到栈顶中
	Vector2** vectorRef = (Vector2**) lua_newuserdata(L, sizeof(Vector2*));
	GE::Int32 x = (GE::Int32) lua_tointeger(L, 1);
	GE::Int32 y = (GE::Int32) lua_tointeger(L, 2);
	// 通过指针实例化对象
	*vectorRef = new Vector2(x, y);
	// 获取全局元表放到栈顶
	luaL_getmetatable(L, "Vector2");
	// 把元表设置到刚才的对象上（栈顶的指针）
	lua_setmetatable(L, -2);
	return 1;

}

GE::Int32 luaopen_vector_libs(lua_State* L){
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