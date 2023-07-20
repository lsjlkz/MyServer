//
// Created by lsjlkz on 2023/7/19.
//

#ifndef MYSERVER_LUAFUNCTION_H
#define MYSERVER_LUAFUNCTION_H

#include "LuaInclude.h"
#include "LuaBridge/LuaBridge.h"

// 包装了一个lua的function的代理类


class LuaFunction {

public:
	LuaFunction(luabridge::LuaRef callback);


	// 模板的实现不能放到cpp
	template<typename... args>
	luabridge::LuaRef Call(args &&... arguments){
		return function(std::forward<args>(arguments)...);
	}

public:
	luabridge::LuaRef function;
};


#endif //MYSERVER_LUAFUNCTION_H
