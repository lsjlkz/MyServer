//
// Created by lsjlkz on 2022/6/2.
//

#include "LuaGameServer.h"
#include "GEProcess.h"
#include "GELog.h"
#include "GENet/GENetPack.h"
#include "GEDateTime.h"

GE::Int32 LuaGameServer::CreateNetwork(lua_State* L) {
	luaL_checktype(L, 1, LUA_TNUMBER);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	GE::Int32 maxCount = (GE::Int32) lua_tointeger(L, 1);
	GE::Int32 thread = (GE::Int32) lua_tointeger(L, 2);
	GE::Int32 port = (GE::Int32) lua_tointeger(L, 3);
	lua_settop(L, 0);
	GE::Int32 ret = GameServer::Instance()->CreateNetwork(maxCount, thread, port);
	lua_pushnumber(L, ret);
	return 1;
}

GE::Int32 LuaGameServer::SetConnectParam(lua_State *L) {
	luaL_checktype(L, 1, LUA_TNUMBER);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	luaL_checktype(L, 6, LUA_TNUMBER);
	GEDefine::ConnectParam CP;
	CP.uSendBlockSize = (GE::Int32) lua_tointeger(L, 1);
	CP.uSendBlockNum = (GE::Int32) lua_tointeger(L, 2);
	CP.uRecvBlockSize = (GE::Int32) lua_tointeger(L, 3);
	CP.uRecvBlockNum = (GE::Int32) lua_tointeger(L, 4);
	CP.uWaitWhoTime = (GE::Int32) lua_tointeger(L, 5);
	CP.uWaitRecvTime = (GE::Int32) lua_tointeger(L, 6);
	GEProcess::Instance()->DefualConnectParam = CP;
	lua_settop(L, 0);
	return 1;
}

GE::Int32 LuaGameServer::SetProcessName(lua_State* L){
	luaL_checktype(L, -1, LUA_TSTRING);
	size_t size = 0;
	const char* s = lua_tolstring(L, -1, &size);
	GEProcess::Instance()->SetProcessName(s, size);
	return 1;
}
GE::Int32 LuaGameServer::LuaPrint(lua_State* L){
	luaL_checktype(L, -1, LUA_TSTRING);
	size_t size = 0;
	const char* s = lua_tolstring(L, -1, &size);
	GELog::Instance()->Log(s);
	return 1;
}

GE::Int32 LuaGameServer::GetGameServerID(lua_State* L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GameServer::Instance()->GetGameServerID());
	return 1;
}

//GE::Int32 LuaGameServer::CallLuaFunc(lua_State* L) {
//	luaL_checktype(L, 1, LUA_TNUMBER);
//	GE::Int32 funcID = lua_tointeger(L, 1);
//	const char* p = lua_tostring(L, 2);
//	GE::Int32 ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/Function.lua");
//	if(ret){
//		return ret;
//	}
//	// 获取元表
//	lua_getglobal(L, "__G__FunctionTable");
//	// 获取函数分发函数
//	lua_pushstring(L, "TriMesFunction");
//	lua_gettable(L, -2);
//	lua_pushinteger(L, funcID);
//	lua_pushstring(L, p);
//	ret = lua_pcall(L, 2, 0, 0);
//	if(ret){
//		GELog::Instance()->Log("lua call err");
//	}
//	return 0;
//}

GE::Int32 LuaGameServer::PackMsg(lua_State* L){
	PackMessage::Instance()->ClearCache();
	return PackMessage::Instance()->PackLuaObj(L);
}

#include "iostream"
GE::Int32 LuaGameServer::DebugPrintMsg(lua_State* L){
	PackMessage::Instance()->Align();
	GE::Uint32* p = reinterpret_cast<GE::Uint32*>(PackMessage::Instance()->HeadPtr());
	GE::Uint32 len = PackMessage::Instance()->PackSize() / sizeof(GE::Uint32);
	std::cout << "size:" <<PackMessage::Instance()->PackSize() << std::endl;
	for(GE::Uint32 i = 0; i < len; i++){
		std::cout << *(p + i) << " ";
	}
	std::cout << std::endl;
	return 1;
}

GE::Int32 LuaGameServer::DebugReceiveMsg(lua_State* L) {
	PackMessage::Instance()->ClearCache();
	PackMessage::Instance()->PackLuaObj(L);
	return 1;
}

GE::Int32 LuaGameServer::ReceiveMsg(char *bufHead) {
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	GE::Int32 ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/GSMessage.lua");
	if(ret){
		return ret;
	}
	// 获取元表
	lua_getglobal(L, "__G__GSMessageTable");
	// 获取函数分发函数
	lua_pushstring(L, "TriggerServerDistribute");
	lua_gettable(L, -2);
	UnpackMessage um(PackMessage::Instance()->HeadPtr());
	GE::Uint16 msg_type = 0;
	um.UnpackMsgType(msg_type);
	GE::Uint16 msg_size = 0;
	um.UnpackU16(msg_size);
	um.SetSize(msg_size);
	lua_pushinteger(L, msg_type);
	um.UnpackLuaObj(L);
	ret = lua_pcall(L, 2, 0, 0);
	if(ret){
		GELog::Instance()->Log("ReceiveMsg lua call err");
		return ret;
	}
	return 0;
}

GE::Int32 LuaGameServer::Init() {
	RegLuaModule();
	LuaGameServer::DoInitGSInit();
	return 1;
}

GE::Int32 LuaGameServer::DoInitGSInit() {
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	// luaserver初始化
	GE::Int32 ret = LuaEngine::Instance()->DoFile("../LuaCode/Server/GSInit.lua");
	if(ret){
		return ret;
	}
	// 获取元表
	lua_getglobal(L, "__G__GSInitTable");
	// 把函数名压栈
	lua_pushstring(L, "main");
	// 这里是-2，因为-1是刚刚压栈的字符串main
	lua_gettable(L, -2);
	ret = lua_pcall(L, 0, 0, 0);
	if(ret){
		GELog::Instance()->Log("Init lua call err");
		return ret;
	}
	return 0;
}

GE::Int32 LuaGameServer::Days(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Days());
	return 1;
}

GE::Int32 LuaGameServer::Seconds(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Seconds());
	return 1;
}

GE::Int32 LuaGameServer::Year(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Year());
	return 1;
}

GE::Int32 LuaGameServer::Month(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Month());
	return 1;
}

GE::Int32 LuaGameServer::Day(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Day());
	return 1;
}

GE::Int32 LuaGameServer::Hour(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Hour());
	return 1;
}

GE::Int32 LuaGameServer::Minute(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Minute());
	return 1;
}

GE::Int32 LuaGameServer::Second(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Second());
	return 1;
}

GE::Int32 LuaGameServer::SetServerStop(lua_State *L) {
	GameServer::Instance()->SetStop();
	return 1;
}

GE::Int32 LuaGameServer::LuaObjToString(lua_State *L) {
	PackMessage::Instance()->ClearCache();
	return PackMessage::Instance()->PackLuaObj(L);
}

GE::Int32 LuaGameServer::DebugSendMsg(lua_State *L) {

	GE::Uint32 sessionId = lua_tointeger(L, -3);
	PackMessage::Instance()->ClearCache();
	PackMessage::Instance()->PackLuaObj(L);

	GameServer::Instance()->SendMsg(sessionId, PackMessage::Instance()->Msg());

	return 1;
}


LUA_API GE::Int32 luaopen_luagameserver_libs(lua_State *L) {
	luaL_newmetatable(L, "__G__LuaGameServerTable");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, lua_reg_gameserver_func, 0);
	return 1;
}
