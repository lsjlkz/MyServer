//
// Created by lsjlkz on 2022/6/2.
//

#include "GameServer.h"

int GameServer::CreateNetwork(int MaxConnect, int Thread, int Port) {
	this->geNetWork = new GENetWork(MaxConnect, Thread);
	this->geNetWork->ListenPort(Port);
	return 0;
}

void GameServer::SetGameServerID(int id) {
	this->GameServerID = id;
}

int GameServer::Init(char* argv[]) {
	int id;
	sscanf_s(argv[1], "%d", &id);
	SetGameServerID(id);
	return 0;
}

void GameServer::Loop() {
	GEDateTime::Instance()->UpdateDateTime();
	this->lastUpdateSecond = GEDateTime::Instance()->Second();
	this->lastUpdateMinute = GEDateTime::Instance()->Minute();
	this->lastUpdateHour = GEDateTime::Instance()->Hour();
	this->lastUpdateDay = GEDateTime::Instance()->Day();
	while(this->isRun && this->geNetWork->isRun()){
		this->Time();
		this->Cycle();
	}
}

void GameServer::Time() {
	GEDateTime::Instance()->UpdateDateTime();
	if(this->lastUpdateSecond == GEDateTime::Instance()->Second())
		return;
	this->lastUpdateSecond = GEDateTime::Instance()->Second();
	LuaEvent::Instance()->CCallLuaPerSecond();

	if(this->lastUpdateMinute == GEDateTime::Instance()->Minute())
		return;
	this->lastUpdateMinute = GEDateTime::Instance()->Minute();
	LuaEvent::Instance()->CCallLuaPerMinute();

	if(this->lastUpdateHour == GEDateTime::Instance()->Hour())
		return;
	this->lastUpdateHour = GEDateTime::Instance()->Hour();
	LuaEvent::Instance()->CCallLuaPerHour();

	if(this->lastUpdateDay == GEDateTime::Instance()->Day())
		return;
	this->lastUpdateDay = GEDateTime::Instance()->Day();
	LuaEvent::Instance()->CCallLuaPerDay();
}

void GameServer::Cycle() {
	// 消息接受等
	// 没有消息就休眠1ms
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void GameServer::SetStop() {
	this->isRun = false;
}
