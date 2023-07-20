//
// Created by lsjlkz on 2022/6/2.
//

#include <iostream>
#include "GameServer.h"
#include "GEProcess.h"
#include "GEDateTime.h"
#include "GELog.h"
#include "GSDefine.h"
#include "Lua/LuaTick.h"
#include "Lua/LuaConst.h"
#include "Lua/LuaServerMsg.h"


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

GE::Uint32 GameServer::Connect(const char* sIP, GE::Uint32 uPort, GE::Uint16 uWho, GEDefine::ConnectParam* pCP){
	GE::Uint32 uSessionID(MAX_UINT32);
	if(this->m_pNetWork->Connect_MT(sIP, uPort, uSessionID, uWho, nullptr, pCP)){
		// 连接成功
		// 要告知对方自己是什么身份
		GE::B4 b4;
		// 潜规则，被动=主动+1
		b4.U16_0() = uWho + 1;
		// TODO 这里还有2个字节可以使用
//		b4.U16_1() = GEProcess::Instance()->
		MSG_Who who;
		who.uWho = b4.UI32();
		this->SendMsg(uSessionID, &who);
		return uSessionID;
	}
	GELog::Instance()->Log("Connect", uSessionID);
	return uSessionID;
}

void GameServer::SetGameServerID(GE::Int32 id) {
	this->GameServerID = id;
}

GE::Int32 GameServer::Init(char* argv[]) {
	GE::Int32 id;
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
	LuaTick::Instance()->AfterCallPerSecond();

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
	// TODO 接收处理消息
	bool hasNext = this->m_pNetWork->MoveToNextMsg();
	if(hasNext){
		// 有消息
		this->OnMsg();
	}
	// 没有消息就休眠1ms
	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
}

void GameServer::SetStop() {

	this->m_pNetWork->Stop_MT();
}

void GameServer::SendMsg(GE::Uint32 uSessionId, MsgBase *pMsg) {
	this->m_pNetWork->SendBytes_MT(uSessionId, pMsg, pMsg->Size());
}


bool GameServer::OnMsg(){
	// 处理消息
	if(this->m_pNetWork->CurConnect()->IsWhoNone()){
		// 没有身份，那应该是要表明身份
		return this->OnDeclareIdentity();
	}
	// 不是表明身份的
	if(this->m_pNetWork->CurConnect()->IsWhoGateway()){
		// 网关发过来的
		this->OnMsgEx();
	}else{
		// TODO 其他的转发的
		this->OnMsgEx();
	}

	return true;
}
bool GameServer::OnMsgEx(){
	GE::Uint16 uMsgType = this->m_pNetWork->CurMsg()->Type();
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	lua_push_buf_to_stack(L, this->m_pNetWork->CurMsg()->Body(), this->m_pNetWork->CurMsg()->BodySize());

	luabridge::LuaRef rParam = luabridge::LuaRef::fromStack(L, -1);
	LuaServerMsgMgr::Instance()->CallServerMsg(uMsgType, this->m_pNetWork->CurConnect()->SessionID(), rParam);
	return true;
}

bool GameServer::OnDeclareIdentity(){
	if(!this->OnSetWho()){
		this->DisConnect(this->m_pNetWork->CurConnect()->SessionID());
	}
	return true;
}

bool GameServer::OnSetWho(){
	MsgBase* pMsg = this->m_pNetWork->CurMsg();
	GE::Uint16 uSize = pMsg->Size();
	GE::Uint16 uMsgType = pMsg->Type();
	if(uMsgType != CMsg_Who){
		// 居然不是表明身份
		return false;
	}
	if(uSize != sizeof(MSG_Who)){
		return false;
	}
	MSG_Who* pMsgWho = (MSG_Who*)pMsg;
	GE::B4 b4= pMsgWho->uWho;
	GE::Uint16 uWhoType = b4.U16_0();
	if(uWhoType == EndPoindType::EndPoint_None){
		GELog::Instance()->Log("error set who", this->m_pNetWork->CurConnect()->SessionID());
		return false;
	}
	this->m_pNetWork->CurConnect()->Who(uWhoType);
	std::string sIP;
	GE::Uint16 uPort;
	this->m_pNetWork->CurConnect()->RemoteEndPoint(sIP, uPort);
	LuaConst::Instance()->ConnectedLuaFunction->Call(this->m_pNetWork->CurConnect()->SessionID(), sIP, uPort);
	return true;
}

void GameServer::DisConnect(GE::Uint32 uSessionID) {
	this->m_pNetWork->DisConnect_MT(uSessionID);
}
