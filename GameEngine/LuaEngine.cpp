//
// Created by lsjlkz on 2022/5/23.
//


#include "LuaEngine.h"

bool LuaEngine::Del() {
	if(GE_IS_POINT_NULL(MainL)){
		GELog::Instance()->Log("close lua_state repeat");
		return false;
	}
	lua_close(MainL);
	MainL = nullptr;
	return true;
}

bool LuaEngine::Init() {
	if(GE_IS_POINT_NOT_NULL(MainL)){
		GELog::Instance()->Log("new lua_state repeat");
		return false;
	}
	MainL = luaL_newstate();
	luaL_openlibs(MainL);
	executeString("LuaEngine Init Successfully");
	return true;
}

GE::Int32 LuaEngine::executeString(const char *codes) {
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	if(GE_IS_POINT_NULL(L)){
		GELog::Instance()->Log("lua engine not init");
		return 0;
	}
	luaL_dostring(L, codes);
	return 0;
}


GE::Int32 LuaEngine::LoadFile(const char *filepath) {
	if(this->HasLoadFile.count(filepath) != 0){
		// 已经加载过了
		return 0;
	}
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	if(GE_IS_POINT_NULL(L)){
		GELog::Instance()->Log("lua engine not init");
		return 1;
	}
	GE::Int32 ret = luaL_loadfile(this->GetMainLuaState(), filepath);
	if(ret){
		GELog::Instance()->Log("load lua file error:", filepath);
	}
	this->HasLoadFile.insert(filepath);
	return ret;
}

GE::Int32 LuaEngine::DoFile(const char *filepath) {
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	if(GE_IS_POINT_NULL(L)){
		GELog::Instance()->Log("lua engine not init");
		return 1;
	}
	GE::Int32 ret = luaL_dofile(L, filepath);
	if(ret){
		GELog::Instance()->Log("load lua file error:", filepath);
	}
	return ret;
}

