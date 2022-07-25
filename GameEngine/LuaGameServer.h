//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_LUAGAMESERVER_H
#define MYSERVER_LUAGAMESERVER_H

#include "LuaEngine.h"
#include "GameServer.h"
#include "GENetPack.h"

static void RegLuaModule();

class LuaGameServer{
private:
	std::unordered_map<int, int> map;
public:
	static int Init();

	static int GetGameServerID(lua_State* L);
	static int CreateNetwork(lua_State* L);
	static int CallLuaFunc(lua_State* L);		//
	static int PackMsg(lua_State* L);
	static int ReceiveMsg(char* bufHead);
	static int DebugReceiveMsg(lua_State* L);
	static int Year(lua_State* L);
	static int Month(lua_State* L);
	static int Day(lua_State* L);
	static int Hour(lua_State* L);
	static int Minute(lua_State* L);
	static int Second(lua_State* L);
	static int Seconds(lua_State* L);
};


static const luaL_Reg lua_reg_gameserver_func[] = {
		{"CreateNetwork", LuaGameServer::CreateNetwork},
		{"GetGameServerID", LuaGameServer::GetGameServerID},
		{"CallLuaFunc", LuaGameServer::CallLuaFunc},
		{"PackMsg", LuaGameServer::PackMsg},
		{"DebugReceiveMsg", LuaGameServer::DebugReceiveMsg},
		{"Year", LuaGameServer::Year},
		{"Month", LuaGameServer::Month},
		{"Day", LuaGameServer::Day},
		{"Hour", LuaGameServer::Hour},
		{"Minute", LuaGameServer::Minute},
		{"Second", LuaGameServer::Second},
		{"Seconds", LuaGameServer::Seconds},
		{NULL, NULL}
};


LUA_API int (luaopen_luagameserver_libs)(lua_State* L);


static luaL_Reg lua_reg_libs[] = {
		{"cGameServer", luaopen_luagameserver_libs},
		{NULL, NULL}
};

static void RegLuaModule(){
	lua_State* L = LuaEngine::Instance()->GetLuaState();
	const luaL_Reg* lua_reg = lua_reg_libs;
	// 注册lua库
	for(;lua_reg->func; ++lua_reg){
		luaL_requiref(L, lua_reg->name, lua_reg->func, 1);
		lua_pop(L, 1);
	}
}

#endif //MYSERVER_LUAGAMESERVER_H
