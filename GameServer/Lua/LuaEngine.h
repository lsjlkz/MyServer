//
// Created by lsjlkz on 2022/5/23.
//

#pragma once
#ifndef MYSERVER_LUAENGINE_H
#define MYSERVER_LUAENGINE_H


#include <unordered_set>


#include "GESingleton.h"
#include "GEDefine.h"
#include "LuaInclude.h"

class LuaEngine:public GESingleton<LuaEngine>{
public:
	LuaEngine();

	lua_State* GetMainLuaState(){ return m_pMainLuaState;}
	bool Init();
	bool Del();

	GE::Int32 LoadFile(const char* filepath);
	GE::Int32 DoFile(const char* filepath);


	// 执行一段lua code
	GE::Int32 executeString(const char* codes);


private:
	// lua_State是主线程、栈、和数据结构
	// 用于函数调用与和c通信
	lua_State *m_pMainLuaState;
	// 全局状态机，管理全局数据
	// static global_State *G;
	std::unordered_set<std::string> HasLoadFile;

};


#endif //MYSERVER_LUAENGINE_H
