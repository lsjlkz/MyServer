//
// Created by lsjlkz on 2022/7/22.
//

#include "LuaEvent.h"

int LuaEvent::CCallLuaPerSecond() {
	lua_State *L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Server/GSDateTime.lua");
	if(ret){
		return ret;
	}
	lua_getglobal(L, "__G__GSDateTimeTable");
	lua_pushstring(L, "CCallPerSecond");
	lua_gettable(L, -2);
	ret = lua_pcall(L, 0, 0, 0);
	if(ret){
		std::cout << "lua call err:" << lua_tostring(L, -1) << std::endl;
		return ret;
	}
	return 0;
}

int LuaEvent::CCallLuaPerMinute() {
	lua_State *L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Server/GSDateTime.lua");
	if(ret){
		return ret;
	}
	lua_getglobal(L, "__G__GSDateTimeTable");
	lua_pushstring(L, "CCallPerMinute");
	lua_gettable(L, -2);
	ret = lua_pcall(L, 0, 0, 0);
	if(ret){
		std::cout << "lua call err:" << lua_tostring(L, -1) << std::endl;
		return ret;
	}
	return 0;
}

int LuaEvent::CCallLuaPerHour() {
	lua_State *L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Server/GSDateTime.lua");
	if(ret){
		return ret;
	}
	lua_getglobal(L, "__G__GSDateTimeTable");
	lua_pushstring(L, "CCallPerHour");
	lua_gettable(L, -2);
	ret = lua_pcall(L, 0, 0, 0);
	if(ret){
		std::cout << "lua call err:" << lua_tostring(L, -1) << std::endl;
		return ret;
	}
	return 0;
}

int LuaEvent::CCallLuaPerDay() {
	lua_State *L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Server/GSDateTime.lua");
	if(ret){
		return ret;
	}
	lua_getglobal(L, "__G__GSDateTimeTable");
	lua_pushstring(L, "CCallPerDay");
	lua_gettable(L, -2);
	ret = lua_pcall(L, 0, 0, 0);
	if(ret){
		std::cout << "lua call err:" << lua_tostring(L, -1) << std::endl;
		return ret;
	}
	return 0;
}
