//
// Created by lsjlkz on 2022/5/23.
//


#include "LuaEngine.h"

bool LuaEngine::Del() {
	if(L == nullptr){
		std::cout << "close lua_state repeat" << std::endl;
		return false;
	}
	lua_close(L);
	L = nullptr;
	return true;
}

bool LuaEngine::Init() {
	if(L != nullptr){
		std::cout << "new lua_state repeat" << std::endl;
		return false;
	}
	L = luaL_newstate();
	luaL_openlibs(L);
	executeString("LuaEngine Init Successfully");
	return true;
}

int LuaEngine::executeString(const char *codes) {
	if(L == nullptr){
		std::cout << "lua engine not init" << std::endl;
		return 0;
	}
	luaL_dostring(L, codes);
	return 0;
}


int LuaEngine::LoadFile(const char *filepath) {
	int ret = luaL_loadfile(L, filepath);
	if(ret){
		std::cout << "load lua file error:(" << filepath << ")" << std::endl;
	}
	return ret;
}

int LuaEngine::DoFile(const char *filepath) {
	if(luaL_dofile(L, filepath)){
		std::cout << lua_tostring(L, -1) << std::endl;;
	}
	return 0;
}

