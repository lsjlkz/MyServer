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
	static void Init();

	static int GetGameServerID();
	static int CreateNetwork(int MaxConnect, int Thread, int Port);
	static int CallLuaFunc(int funcID, const char* p);		// 这个是C调用lua的，而不是lua调用C调用lua的，所以这里不需要参数
	static int CallLuaFunc(lua_State* L);		// 这个是C调用lua的，而不是lua调用C调用lua的，所以这里不需要参数
	static int PackMsg(lua_State* L);
	static int ReceiveMsg(char* bufHead);
	static int DebugReceiveMsg(lua_State* L);
};



int create_network(lua_State* L);
int get_gameserver_id(lua_State* L);
int reg_lua_func(lua_State* L);
int call_lua_func(lua_State* L);
int pack_msg(lua_State* L);
int debug_receive_msg(lua_State* L);

static const luaL_Reg lua_reg_gameserver_func[] = {
		{"CreateNetwork", create_network},
		{"GetGameServerID", get_gameserver_id},
		{"CallLuaFunc", call_lua_func},
		{"PackMsg", pack_msg},
		{"DebugReceiveMsg", debug_receive_msg},
		{NULL, NULL}
};

static const luaL_Reg lua_reg_gameserver_create_funcs[] = {
		{NULL, NULL}
};

int luaopen_luagameserver_libs(lua_State* L);


static luaL_Reg lua_reg_libs[] = {
		{"base", luaopen_base},
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
