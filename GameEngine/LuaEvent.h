//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_LUAEVENT_H
#define MYSERVER_LUAEVENT_H

#include "LuaEngine.h"
#include "SingleTon.h"

class LuaEvent: public SingleTon<LuaEvent>{

public:
	int CCallLuaPerSecond();
	int CCallLuaPerMinute();
	int CCallLuaPerHour();
	int CCallLuaPerDay();
};


#endif //MYSERVER_LUAEVENT_H
