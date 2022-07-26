//
// Created by lsjlkz on 2022/6/2.
//

#include "LuaGameServer.h"

int LuaGameServer::CreateNetwork(lua_State* L) {
	luaL_checktype(L, 1, LUA_TNUMBER);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	int maxCount = (int) lua_tointeger(L, 1);
	int thread = (int) lua_tointeger(L, 2);
	int port = (int) lua_tointeger(L, 3);
	lua_settop(L, 0);
	int ret = GameServer::Instance()->CreateNetwork(maxCount, thread, port);
	lua_pushnumber(L, ret);
	return 1;
}

int LuaGameServer::GetGameServerID(lua_State* L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GameServer::Instance()->GetGameServerID());
	return 1;
}

//int LuaGameServer::CallLuaFunc(lua_State* L) {
//	luaL_checktype(L, 1, LUA_TNUMBER);
//	int funcID = lua_tointeger(L, 1);
//	const char* p = lua_tostring(L, 2);
//	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/Function.lua");
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
//		std::cout << "lua call err" << std::endl;
//	}
//	return 0;
//}

int LuaGameServer::PackMsg(lua_State* L){
	PackMessage::Instance()->ClearCache();
	return PackMessage::Instance()->PackLuaObj(L);
}

int LuaGameServer::DebugReceiveMsg(lua_State* L) {
	PackMessage::Instance()->ClearCache();
	PackMessage::Instance()->PackLuaObj(L);
	ReceiveMsg(PackMessage::Instance()->curBufHead);
	return 1;
}

int LuaGameServer::ReceiveMsg(char *bufHead) {
	lua_State* L = LuaEngine::Instance()->GetLuaState();
	int ret = LuaEngine::Instance()->LoadFile("../LuaCode/Common/Game/GSMessage.lua");
	if(ret){
		return ret;
	}
	// 获取元表
	lua_getglobal(L, "__G__GSMessageTable");
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
		std::cout << "lua call err" << lua_tostring(L, -1)  << std::endl;
		return ret;
	}
	return 0;
}

int LuaGameServer::Init() {
	RegLuaModule();
	lua_State* L = LuaEngine::Instance()->GetLuaState();
	// luaserver初始化
	int ret = LuaEngine::Instance()->DoFile("../LuaCode/Server/GSInit.lua");
	if(ret){
		return ret;
	}
	// 获取元表
	lua_getglobal(L, "__G__GSInitTable");
	lua_pushstring(L, "main");
	lua_gettable(L, -2);
	ret = lua_pcall(L, 0, 0, 0);
	if(ret){
		std::cout << "lua call err:" << lua_tostring(L, -1) << std::endl;
		return ret;
	}
	return 0;
}

int LuaGameServer::Year(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Year());
	return 1;
}

int LuaGameServer::Month(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Month());
	return 1;
}

int LuaGameServer::Day(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Day());
	return 1;
}

int LuaGameServer::Hour(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Hour());
	return 1;
}

int LuaGameServer::Minute(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Minute());
	return 1;
}

int LuaGameServer::Second(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->Second());
	return 1;
}

int LuaGameServer::Seconds(lua_State *L) {
	lua_settop(L, 0);
	lua_pushinteger(L, GEDateTime::Instance()->UnixTime());
	return 1;
}

int luaopen_luagameserver_libs(lua_State *L) {
	luaL_newmetatable(L, "__G__LuaGameServerTable");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, lua_reg_gameserver_func, 0);
	return 1;
}
