//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_GAMESERVER_H
#define MYSERVER_GAMESERVER_H

#include <chrono>
#include <thread>
#include "GESingleton.h"
#include "GENet/GENetWork.h"
#include "Lua/LuaEvent.h"

class GameServer: public GESingleton<GameServer>{
public:
	GameServer();
	GE::Int32				CreateNetwork(GE::Int32 MaxConnect, GE::Int32 Thread, GE::Int32 Port);
	GE::Uint32 				Connect(const char* sIP, GE::Uint32 uPort, GE::Uint16 uWho, GEDefine::ConnectParam* pCP);
	void					SetGameServerID(GE::Int32 id);
	GE::Int32				GetGameServerID(){return GameServerID;};
	GE::Int32				Init(char* argv[]);
	void DisConnect(GE::Uint32 uSessionID);
	bool					OnMsg();
	bool 					OnMsgEx();
	bool					OnDeclareIdentity();		// 表明身份
	bool					OnSetWho();					// 设置身份

//    TODO
//    GE::Int32 Connect(char* ip, GE::Int32 port, )

	void					SendMsg(GE::Uint32 uSessionId, MsgBase* pMsg);


	void Loop();
	void Time();
	void Cycle();
	void SetStop();

private:
	GENetWork* m_pNetWork;
	GE::Int32 GameServerID;

	GE::Int32 lastUpdateSecond;
	GE::Int32 lastUpdateMinute;
	GE::Int32 lastUpdateHour;
	GE::Int32 lastUpdateDay;


};


#endif //MYSERVER_GAMESERVER_H
