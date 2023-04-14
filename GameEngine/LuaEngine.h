//
// Created by lsjlkz on 2022/5/23.
//

#pragma once
#ifndef MYSERVER_LUAENGINE_H
#define MYSERVER_LUAENGINE_H

#include "Head.h"
#include "GENet/GENetPack.h"
#include <unordered_set>

class LuaEngine:public GESingleton<LuaEngine>{
private:
	// lua_State是主线程、栈、和数据结构
	// 用于函数调用与和c通信
	lua_State *MainL;
	// 全局状态机，管理全局数据
	// static global_State *G;
	std::unordered_set<std::string> HasLoadFile;
public:
	lua_State* GetMainLuaState(){
		return MainL;
	}
	bool Init();
	bool Del();

	GE::Int32 LoadFile(const char* filepath);
	GE::Int32 DoFile(const char* filepath);


	// 执行一段lua code
	GE::Int32 executeString(const char* codes);

};


#endif //MYSERVER_LUAENGINE_H
