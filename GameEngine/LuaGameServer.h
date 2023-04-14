//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_LUAGAMESERVER_H
#define MYSERVER_LUAGAMESERVER_H

#include "LuaEngine.h"
#include "GameServer.h"
#include "GENet/GENetPack.h"
#include "GEProcess.h"

static void RegLuaModule();

class LuaGameServer{
private:
	std::unordered_map<GE::Int32, GE::Int32> map;
public:
	static GE::Int32 Init();

	static GE::Int32 GetGameServerID(lua_State* L);
	static GE::Int32 CreateNetwork(lua_State* L);
	static GE::Int32 SetConnectParam(lua_State* L);
	static GE::Int32 SetFSCoutName(lua_State* L);
	static GE::Int32 LuaPrint(lua_State* L);
//	static GE::Int32 CallLuaFunc(lua_State* L);		// 暂时没发现有什么用，或许跨lua线程调用会有用，但是现在不是很必须
	static GE::Int32 PackMsg(lua_State* L);
	static GE::Int32 ReceiveMsg(char* bufHead);
	static GE::Int32 DebugReceiveMsg(lua_State* L);
	static GE::Int32 Year(lua_State* L);
	static GE::Int32 Month(lua_State* L);
	static GE::Int32 Day(lua_State* L);
	static GE::Int32 Hour(lua_State* L);
	static GE::Int32 Minute(lua_State* L);
	static GE::Int32 Second(lua_State* L);
	static GE::Int32 Seconds(lua_State* L);
	static GE::Int32 SetServerStop(lua_State* L);
	static GE::Int32 LuaObjToString(lua_State* L);
};


static const luaL_Reg lua_reg_gameserver_func[] = {
		{"CreateNetwork", LuaGameServer::CreateNetwork},
		{"SetFSCoutName", LuaGameServer::SetFSCoutName},
		{"LuaPrint", LuaGameServer::LuaPrint},
		{"GetGameServerID", LuaGameServer::GetGameServerID},
		{"PackMsg", LuaGameServer::PackMsg},
		{"DebugReceiveMsg", LuaGameServer::DebugReceiveMsg},
		{"Year", LuaGameServer::Year},
		{"Month", LuaGameServer::Month},
		{"Day", LuaGameServer::Day},
		{"Hour", LuaGameServer::Hour},
		{"Minute", LuaGameServer::Minute},
		{"Second", LuaGameServer::Second},
		{"Seconds", LuaGameServer::Seconds},
		{"SetServerStop", LuaGameServer::SetServerStop},
		{"LuaObjToString", LuaGameServer::LuaObjToString},
		{"SetConnectParam", LuaGameServer::SetConnectParam},
		{NULL, NULL}
};


LUA_API GE::Int32 (luaopen_luagameserver_libs)(lua_State* L);


static luaL_Reg lua_reg_libs[] = {
		{"cGameServer", luaopen_luagameserver_libs},
		{NULL, NULL}
};

static void RegLuaModule(){
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	const luaL_Reg* lua_reg = lua_reg_libs;
	// 注册lua库
	for(;lua_reg->func; ++lua_reg){
		luaL_requiref(L, lua_reg->name, lua_reg->func, 1);
		lua_pop(L, 1);
	}
}

#endif //MYSERVER_LUAGAMESERVER_H
