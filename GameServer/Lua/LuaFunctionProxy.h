//
// Created by lsjlkz on 2023/7/19.
//

#ifndef MYSERVER_LUAFUNCTIONPROXY_H
#define MYSERVER_LUAFUNCTIONPROXY_H

#include "LuaInclude.h"
#include "LuaBridge/LuaBridge.h"

// 包装了一个lua的function的代理类


class LuaFunctionProxy {

public:
	explicit LuaFunctionProxy(const luabridge::LuaRef& callback): r_luaFunction(callback){};


	// 模板的实现不能放到cpp
	template<typename... args>
	luabridge::LuaRef Call(args &&... arguments){
		return r_luaFunction(std::forward<args>(arguments)...);
	}

public:
	luabridge::LuaRef r_luaFunction;
};


#endif //MYSERVER_LUAFUNCTIONPROXY_H
