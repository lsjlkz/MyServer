//
// Created by lsjlkz on 2022/6/2.
//

#include "LuaGameServer.h"
#include "GEProcess.h"
#include "GELog.h"
#include "GENet/GENetPack.h"
#include "GEDateTime.h"
#include "LuaTick.h"

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

GE::Int32 LuaGameServer::SetClientRedirect(lua_State* L){
	// 客户端重定向
	GE::Uint32 uRedirectType = lua_tointeger(L, 1);	// 重定向类型
	WIN_ASSERT(uRedirectType < CLIENT_REDIRECT_MAX);
	GE::Uint32 uRedirectSessionId = lua_tointeger(L, 2);// 重定向session
	GEProcess::Instance()->SetClientRedirect(uRedirectType, uRedirectSessionId);
	return 1;
}

GE::Int32 LuaGameServer::LuaPrint(lua_State* L){
	luaL_checktype(L, -1, LUA_TSTRING);
	size_t size = 0;
	const char* s = lua_tolstring(L, -1, &size);
	GELog::Instance()->Log(s);
	return 1;
}

GE::Int32 LuaGameServer::Connect(lua_State* L){
	// 连接到某个host和端口
	luaL_checktype(L, 1, LUA_TSTRING);	// host
	luaL_checktype(L, 2, LUA_TNUMBER);	// port
	luaL_checktype(L, 3, LUA_TNUMBER);	// who
	luaL_checktype(L, 4, LUA_TNUMBER);	// sendBlockSize
	luaL_checktype(L, 5, LUA_TNUMBER);	// sendBlockNum
	luaL_checktype(L, 6, LUA_TNUMBER);	// recvBlockSize
	luaL_checktype(L, 7, LUA_TNUMBER);	// recvBlockNum
	luaL_checktype(L, 8, LUA_TNUMBER);	// waitWhoTime
	luaL_checktype(L, 9, LUA_TNUMBER);	// waitRecvTime

	size_t size = 0;
	const char* sIP = lua_tolstring(L, 1, &size);
	GE::Uint32 uPort = lua_tonumber(L, 2);
	GE::Uint32 uWho = lua_tonumber(L, 3);

	GEDefine::ConnectParam* pCP = new GEDefine::ConnectParam();
	pCP->uSendBlockSize = lua_tonumber(L, 4);
	pCP->uSendBlockNum = lua_tonumber(L, 5);
	pCP->uRecvBlockSize = lua_tonumber(L, 6);
	pCP->uRecvBlockNum = lua_tonumber(L, 7);
	pCP->uWaitWhoTime = lua_tonumber(L, 8);
	pCP->uWaitRecvTime = lua_tonumber(L, 9);
	GE::Uint32 uSessionID = GameServer::Instance()->Connect(sIP, uPort, uWho, pCP);
	lua_pushinteger(L, uSessionID);
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

GE::Int32 LuaGameServer::DebugPrintMsg(lua_State* L){
	PackMessage::Instance()->Align();
	GE::Uint32* p = reinterpret_cast<GE::Uint32*>(PackMessage::Instance()->HeadPtr());
	GE::Uint32 len = PackMessage::Instance()->PackSize() / sizeof(GE::Uint32);
	GELog::Instance()->Log("size:", PackMessage::Instance()->PackSize());
	for(GE::Uint32 i = 0; i < len; i++){
		GELog::Instance()->Log("", *(p + i));
	}
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

GE::Int32 LuaGameServer::RegTick(lua_State *L) {
	GE::Uint64 seconds = luabridge::get<GE::Uint64>(L, 1);
	luabridge::LuaRef owner = luabridge::get<luabridge::LuaRef>(L, 2);
	luabridge::LuaRef callback = luabridge::get<luabridge::LuaRef>(L, 3);
	luabridge::LuaRef param = luabridge::get<luabridge::LuaRef>(L, 4);
	GE::Uint64 tickID = LuaTick::Instance()->RegTick(seconds, owner, callback, param);
	luabridge::push(L, tickID);
	return 1;
}


GE::Int32 LuaGameServer::TriggerTick(lua_State *L) {
	return 1;
}


GE::Int32 LuaGameServer::UnregTick(lua_State *L) {
	return 1;
}




GE::Int32 LuaGameServer::DebugSendMsg(lua_State *L) {

	GE::Uint32 sessionId = lua_tointeger(L, -3);
	PackMessage::Instance()->ClearCache();
	PackMessage::Instance()->PackLuaObj(L);

	GameServer::Instance()->SendMsg(sessionId, PackMessage::Instance()->Msg());

	return 1;
}

GE::Int32 LuaGameServer::GC(lua_State *L) {
	lua_gc(L, LUA_GCCOLLECT);
	lua_pushnil(L);
	return 1;
}

LUA_API GE::Int32 luaopen_luagameserver_libs(lua_State *L) {
	luaL_newmetatable(L, "__G__LuaGameServerTable");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, lua_reg_gameserver_func, 0);
	return 1;
}
