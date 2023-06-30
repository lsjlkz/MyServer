
#include <GEDateTime.h>
#include "GameServer/Lua/LuaGameServer.h"


int main(int argc, char* argv[]) {

	GEDateTime::New();
	GameServer::Instance()->Init(argv);// 初始化C++服务器
	LuaEngine::New();// 初始化lua引擎
	LuaEngine::Instance()->Init();

	LuaGameServer::Init();// 初始化LuaGameServer
	LuaEvent::Instance()->Init();

	LuaGameServer::DoInitGSInit();

	GameServer::Instance()->Loop();

	return 0;
}