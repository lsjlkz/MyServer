//
// Created by lsjlkz on 2021/12/23.
//

#ifndef MYSERVER_VECTOR2_H
#define MYSERVER_VECTOR2_H
#include "LuaEngine.h"

int lua_get_vector(lua_State* L);
int lua_create_vector(lua_State* L);

static const luaL_Reg lua_reg_vector_create_funcs[] = {
		{"Create", lua_create_vector},
		{NULL, NULL},
};

static const luaL_Reg lua_reg_vector_funcs[] = {
		{"GetXY", lua_get_vector},
		{NULL, NULL},
};

int luaopen_vector_libs(lua_State* L);

class Vector2 {
private:
    int _x=0;
    int _y=0;

public:
    Vector2(int x, int y):_x(x), _y(y){};
    Vector2(Vector2 const &xy);
    Vector2();

    void IncX(int x){
        _x += x;
    }
    void IncY(int y){
        _y += y;
    }
    void IncXY(int x, int y){
        _x += x;
        _y += y;
    }

    int GetX(){
        return _x;
    }
    int GetY(){
        return _y;
    }
};


#endif //MYSERVER_VECTOR2_H
