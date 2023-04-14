//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_LUAEVENT_H
#define MYSERVER_LUAEVENT_H

#include "GEDefine.h"
#include "GESingleton.h"

class LuaEvent: public GESingleton<LuaEvent>{

public:
	GE::Int32 CCallLuaPerSecond();
	GE::Int32 CCallLuaPerMinute();
	GE::Int32 CCallLuaPerHour();
	GE::Int32 CCallLuaPerDay();
};


#endif //MYSERVER_LUAEVENT_H
