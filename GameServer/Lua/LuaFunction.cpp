//
// Created by lsjlkz on 2023/7/19.
//

#include "LuaFunction.h"


LuaFunction::LuaFunction(luabridge::LuaRef callback):Call(callback){

}

// TODO 不知道为什么这个不行
//template<typename ... args>
//luabridge::LuaRef LuaFunction::Call(args &&... arguments) {
//	return function(std::forward<args>(arguments)...);
//}
