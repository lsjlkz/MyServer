//
// Created by lsjlkz on 2022/6/2.
//

#include "LuaGameServer.h"

int LuaGameServer::CreateNetwork(int MaxConnect, int Thread, int Port) {
	return GameServer::Instance()->CreateNetwork(MaxConnect, Thread, Port);
}

int LuaGameServer::GetGameServerID() {
	return GameServer::Instance()->GetGameServerID();
}

int LuaGameServer::CallLuaFunc(int funcID, const char* p) {
	lua_State *L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/Function.lua");
	if(ret){
		return ret;
	}
	// 获取元表
	lua_getglobal(L, "__G__FunctionTable");
	// 获取函数分发函数
	lua_pushstring(L, "TriMesFunction");
	lua_gettable(L, -2);
	lua_pushinteger(L, funcID);
	lua_pushstring(L, p);
	ret = lua_pcall(L, 2, 0, 0);
	if(ret){
		std::cout << "lua call err" << std::endl;
	}
	return 0;
}

int LuaGameServer::CallLuaFunc(lua_State *L) {
	luaL_checktype(L, 1, LUA_TNUMBER);
	int funcID = lua_tointeger(L, 1);
	const char* p = lua_tostring(L, 2);
	return CallLuaFunc(funcID, p);
}

int LuaGameServer::PackMsg(lua_State* L){
	PackMessage::Instance()->ClearCache();
	return PackMessage::Instance()->PackLuaObj(L);
}

int LuaGameServer::DebugReceiveMsg(lua_State* L) {
	PackMessage::Instance()->ClearCache();
	PackMessage::Instance()->PackLuaObj(L);
	ReceiveMsg(PackMessage::Instance()->curBufHead);
//	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/Message.lua");
//	if(ret){
//		return ret;
//	}
//	// 获取元表
//	lua_getglobal(L, "__G__MessageTable");
//	// 获取函数分发函数
//	lua_pushstring(L, "TriggerServerDistribute");
//	lua_gettable(L, -2);
//	UnpackMessage um = UnpackMessage(PackMessage::Instance()->curBufHead, PackMessage::Instance()->msgSize);
//	int msg_size = 0;
//	um.UnpackInt(msg_size);
//	int msg_type = 0;
//	um.UnpackMsgType(msg_type);
//	lua_pushinteger(L, msg_type);
//	um.UnpackLuaObj(L);
//	ret = lua_pcall(L, 2, 0, 0);
//	if(ret){
//		std::cout << "lua call err" << std::endl;
//	}
	return 1;
}

int LuaGameServer::ReceiveMsg(char *bufHead) {
	lua_State* L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/Message.lua");
	if(ret){
		return ret;
	}
	// 获取元表
	lua_getglobal(L, "__G__MessageTable");
	// 获取函数分发函数
	lua_pushstring(L, "TriggerServerDistribute");
	lua_gettable(L, -2);
	int msg_size = 0;
	UnpackMessage um = UnpackMessage(PackMessage::Instance()->curBufHead);
	um.UnpackInt(msg_size);
	um.SetSize(msg_size);
	int msg_type = 0;
	um.UnpackMsgType(msg_type);
	lua_pushinteger(L, msg_type);
	um.UnpackLuaObj(L);
	ret = lua_pcall(L, 2, 0, 0);
	if(ret){
		std::cout << "lua call err" << std::endl;
	}
	return 0;
}

void LuaGameServer::Init() {
	RegLuaModule();
	// luaserver初始化
	LuaEngine::Instance()->DoFile("../LuaCode/Server/GSInit.lua");
}

int reg_lua_func(lua_State* L){
	return 0;
}

int call_lua_func(lua_State *L){
	return LuaGameServer::CallLuaFunc(L);
}

int pack_msg(lua_State* L){
	return LuaGameServer::PackMsg(L);
}

int debug_receive_msg(lua_State* L){
	return LuaGameServer::DebugReceiveMsg(L);
}

int get_gameserver_id(lua_State* L){
	lua_settop(L, 0);
	lua_pushinteger(L, LuaGameServer::GetGameServerID());
	return 1;
}

int create_network(lua_State* L){
	luaL_checktype(L, 1, LUA_TNUMBER);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	int maxCount = (int) lua_tointeger(L, 1);
	int thread = (int) lua_tointeger(L, 2);
	int port = (int) lua_tointeger(L, 3);
	lua_settop(L, 0);
	int ret = LuaGameServer::CreateNetwork(maxCount, thread, port);
	lua_pushnumber(L, ret);
	return 1;
}

int luaopen_luagameserver_libs(lua_State *L) {
	luaL_newmetatable(L, "LuaGameServer");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, lua_reg_gameserver_func, 0);
	// 创建新表，压函数
//	luaL_newlib(L, lua_reg_gameserver_create_funcs);
	return 1;
}
