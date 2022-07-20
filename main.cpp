#include <iostream>
#include "LuaGameServer.h"

int main(int argc, char* argv[]) {

	// 初始化C++服务器

	GameServer::Instance()->Init(argv);
	// 初始化lua引擎
	LuaEngine::New();
	LuaEngine::Instance()->Init();
	// 初始化LuaGameServer
	LuaGameServer::Init();

	GameServer::Instance()->Loop();
	return 0;
}