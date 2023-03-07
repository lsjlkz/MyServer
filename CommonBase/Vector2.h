//
// Created by lsjlkz on 2021/12/23.
//

#ifndef MYSERVER_VECTOR2_H
#define MYSERVER_VECTOR2_H
#include "LuaEngine.h"

GE::Int32 lua_get_vector(lua_State* L);
GE::Int32 lua_create_vector(lua_State* L);

static const luaL_Reg lua_reg_vector_create_funcs[] = {
		{"Create", lua_create_vector},
		{NULL, NULL},
};

static const luaL_Reg lua_reg_vector_funcs[] = {
		{"GetXY", lua_get_vector},
		{NULL, NULL},
};

GE::Int32 luaopen_vector_libs(lua_State* L);

class Vector2 {
private:
    GE::Int32 _x=0;
    GE::Int32 _y=0;

public:
    Vector2(GE::Int32 x, GE::Int32 y):_x(x), _y(y){};
    Vector2(Vector2 const &xy);
    Vector2();

    void IncX(GE::Int32 x){
        _x += x;
    }
    void IncY(GE::Int32 y){
        _y += y;
    }
    void IncXY(GE::Int32 x, GE::Int32 y){
        _x += x;
        _y += y;
    }

    GE::Int32 GetX(){
        return _x;
    }
    GE::Int32 GetY(){
        return _y;
    }
};


#endif //MYSERVER_VECTOR2_H
