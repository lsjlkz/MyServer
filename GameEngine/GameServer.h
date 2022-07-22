//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_GAMESERVER_H
#define MYSERVER_GAMESERVER_H

#include "SingleTon.h"
#include "GEDateTime.h"
#include "GENetWork.h"
#include "LuaEvent.h"

class GameServer: public SingleTon<GameServer>{
public:
	int CreateNetwork(int MaxConnect, int Thread, int Port);
	void SetGameServerID(int id);
	int GetGameServerID(){return GameServerID;};
	int Init(char* argv[]);


	void Loop();
	void Time();
	void Cycle();

private:
	GENetWork* geNetWork;
	int GameServerID = 0;

	int lastUpdateSecond = 0;
	int lastUpdateMinute = 0;
	int lastUpdateHour = 0;
	int lastUpdateDay = 0;

};


#endif //MYSERVER_GAMESERVER_H
