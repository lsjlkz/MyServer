//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_GAMESERVER_H
#define MYSERVER_GAMESERVER_H

#include <chrono>
#include <thread>
#include "SingleTon.h"
#include "GEDateTime.h"
#include "GENet/GENetWork.h"
#include "LuaEvent.h"

class GameServer: public SingleTon<GameServer>{
public:
	GE::Int32 CreateNetwork(GE::Int32 MaxConnect, GE::Int32 Thread, GE::Int32 Port);
	void SetGameServerID(GE::Int32 id);
	GE::Int32 GetGameServerID(){return GameServerID;};
	GE::Int32 Init(char* argv[]);

//    TODO
//    GE::Int32 Connect(char* ip, GE::Int32 port, )


	void Loop();
	void Time();
	void Cycle();
	void SetStop();

private:
	GENetWork* m_pNetWork;
	GE::Int32 GameServerID = 0;

	GE::Int32 lastUpdateSecond = 0;
	GE::Int32 lastUpdateMinute = 0;
	GE::Int32 lastUpdateHour = 0;
	GE::Int32 lastUpdateDay = 0;


};


#endif //MYSERVER_GAMESERVER_H
