//
// Created by lsjlkz on 2022/6/2.
//

#include "GameServer.h"
#include "GEDateTime.h"


GameServer::GameServer():m_pNetWork(nullptr),
						 GameServerID(0),
						 lastUpdateSecond(0),
						 lastUpdateMinute(0),
						 lastUpdateHour(0),
						 lastUpdateDay(0)
{

}

GE::Int32 GameServer::CreateNetwork(GE::Int32 MaxConnect, GE::Int32 Thread, GE::Int32 Port) {
	this->m_pNetWork = new GENetWork(MaxConnect, Thread);
	this->m_pNetWork->Listen_MT(Port);
	return 0;
}

void GameServer::SetGameServerID(GE::Int32 id) {
	this->GameServerID = id;
}

GE::Int32 GameServer::Init(char* argv[]) {
	GE::Int32 id;
	sscanf_s(argv[1], "%d", &id);
	SetGameServerID(2);
	return 0;
}

void GameServer::Loop() {
	GEDateTime::Instance()->UpdateDateTime();
	this->lastUpdateSecond = GEDateTime::Instance()->Second();
	this->lastUpdateMinute = GEDateTime::Instance()->Minute();
	this->lastUpdateHour = GEDateTime::Instance()->Hour();
	this->lastUpdateDay = GEDateTime::Instance()->Day();
	this->m_pNetWork->Start_MT();
	while(this->m_pNetWork->isRun()){
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

	this->m_pNetWork->Stop_MT();
}

void GameServer::SendMsg(GE::Uint32 uSessionId, MsgBase *pMsg) {
	this->m_pNetWork->SendBytes_MT(uSessionId, pMsg, pMsg->Size());
}
