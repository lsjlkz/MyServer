//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_LUAEVENT_H
#define MYSERVER_LUAEVENT_H

#include "unordered_map"

#include "GEDefine.h"
#include "GESingleton.h"
#include "LuaEngine.h"
#include "LuaBridge/LuaBridge.h"

// 最大参数个数
#define MAX_EVENT_PARAM 5

// TODO 待定，可能需要拷贝构造LuaRef

class LuaEvent: public GESingleton<LuaEvent>{

	typedef std::vector<luabridge::LuaRef> EventList;
	typedef std::unordered_map<GE::Int32 , EventList> EventMap;
	typedef std::vector<luabridge::LuaRef> CallParam;

	public:
		LuaEvent();
		GE::Int32 Init();
		GE::Int32 CCallLuaPerSecond();
		GE::Int32 CCallLuaPerMinute();
		GE::Int32 CCallLuaPerHour();
		GE::Int32 CCallLuaPerDay();


		static GE::Int32 SetCallPerIndex(GE::Int32 perSecondIndex, GE::Int32 perMinuteIndex, GE::Int32 perHourIndex, GE::Int32 perDayIndex);

		GE::Int32 RegEvent(GE::Int32 event, luabridge::LuaRef);
		GE::Int32 TriggerEvent(GE::Int32 event);
		GE::Int32 TriggerEvent(GE::Int32 event, CallParam params);
		static GE::Int32 sRegEvent(lua_State* L);
		static GE::Int32 sTriggerEvent(lua_State* L);
		static GE::Int32 sSetCallPerIndex(lua_State* L);


private:
		EventMap m_mEventFunctionList;

		GE::Int32 	i_mCallPerSecondIndex = 0;
		GE::Int32 	i_mCallPerMinuteIndex = 0;
		GE::Int32 	i_mCallPerHourIndex = 0;
		GE::Int32 	i_mCallPerDayIndex = 0;

		luabridge::LuaRef nil = nullptr;
		CallParam zeroParams;
};

void RegLuaEvent();

#endif //MYSERVER_LUAEVENT_H
