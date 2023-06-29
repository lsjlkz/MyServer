//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_LUAGAMESERVER_H
#define MYSERVER_LUAGAMESERVER_H

#include "LuaEngine.h"
#include "GameServer.h"
#include "GELog.h"

#include <LuaBridge/LuaBridge.h>

static void RegLuaModule();

class LuaGameServer{
private:
	std::unordered_map<GE::Int32, GE::Int32> map;
public:
	static GE::Int32 Init();
	static GE::Int32 DoInitGSInit();

	static GE::Int32 GetGameServerID(lua_State* L);
	static GE::Int32 CreateNetwork(lua_State* L);
	static GE::Int32 SetConnectParam(lua_State* L);
	static GE::Int32 SetProcessName(lua_State* L);
	static GE::Int32 SetClientRedirect(lua_State* L); // 客户端重定向标记
	static GE::Int32 LuaPrint(lua_State* L);
	static GE::Int32 Connect(lua_State* L);
//	static GE::Int32 CallLuaFunc(lua_State* L);		// 暂时没发现有什么用，或许跨lua线程调用会有用，但是现在不是很必须
	static GE::Int32 PackMsg(lua_State* L);
	static GE::Int32 ReceiveMsg(char* bufHead);
	static GE::Int32 DebugPrintMsg(lua_State* L);
	static GE::Int32 DebugReceiveMsg(lua_State* L);
	static GE::Int32 Days(lua_State* L);
	static GE::Int32 Seconds(lua_State* L);
	static GE::Int32 Year(lua_State* L);
	static GE::Int32 Month(lua_State* L);
	static GE::Int32 Day(lua_State* L);
	static GE::Int32 Hour(lua_State* L);
	static GE::Int32 Minute(lua_State* L);
	static GE::Int32 Second(lua_State* L);
	static GE::Int32 SetServerStop(lua_State* L);
	static GE::Int32 LuaObjToString(lua_State* L);

	static GE::Int32 DebugSendMsg(lua_State* L);			// 测试发送一个数据		Param:sessionId, data
	static GE::Int32 RegMsgDistribute(lua_State* L);		// 注册一个消息回调
};


static const luaL_Reg lua_reg_gameserver_func[] = {
		{"CreateNetwork", LuaGameServer::CreateNetwork},
		{"SetProcessName", LuaGameServer::SetProcessName},
		{"SetClientRedirect", LuaGameServer::SetClientRedirect},
		{"LuaPrint", LuaGameServer::LuaPrint},
		{"GetGameServerID", LuaGameServer::GetGameServerID},
		{"Connect", LuaGameServer::Connect},
		{"PackMsg", LuaGameServer::PackMsg},
		{"DebugReceiveMsg", LuaGameServer::DebugReceiveMsg},
		{"Days", LuaGameServer::Days},
		{"Seconds", LuaGameServer::Seconds},
		{"Year", LuaGameServer::Year},
		{"Month", LuaGameServer::Month},
		{"Day", LuaGameServer::Day},
		{"Hour", LuaGameServer::Hour},
		{"Minute", LuaGameServer::Minute},
		{"Second", LuaGameServer::Second},
		{"SetServerStop", LuaGameServer::SetServerStop},
		{"LuaObjToString", LuaGameServer::LuaObjToString},
		{"SetConnectParam", LuaGameServer::SetConnectParam},
		{"DebugSendMsg", LuaGameServer::DebugSendMsg},
		{"DebugPrintMsg", LuaGameServer::DebugPrintMsg},
		{NULL, NULL}
};


LUA_API GE::Int32 (luaopen_luagameserver_libs)(lua_State* L);


static luaL_Reg lua_reg_libs[] = {
		{"cGameServer", luaopen_luagameserver_libs},
		{NULL, NULL}
};

static void RegLuaModule(){
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();

	luabridge::Namespace cGameServer = luabridge::getGlobalNamespace(L)
			.beginNamespace("cGameServer");
	// 使用luabridge库把cGameServer注册为全局的环境
	const luaL_Reg* cGameServerReg = lua_reg_gameserver_func;
	// 注册lua库
	for(;cGameServerReg->func; ++cGameServerReg){
		cGameServer.addCFunction(cGameServerReg->name, cGameServerReg->func);
	}
	cGameServer.endNamespace();

}

#endif //MYSERVER_LUAGAMESERVER_H
