//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_LUAEVENT_H
#define MYSERVER_LUAEVENT_H

#include "LuaEngine.h"
#include "SingleTon.h"

class LuaEvent: public SingleTon<LuaEvent>{

public:
	GE::Int32 CCallLuaPerSecond();
	GE::Int32 CCallLuaPerMinute();
	GE::Int32 CCallLuaPerHour();
	GE::Int32 CCallLuaPerDay();
};


#endif //MYSERVER_LUAEVENT_H
