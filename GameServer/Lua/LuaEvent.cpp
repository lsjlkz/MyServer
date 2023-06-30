//
// Created by lsjlkz on 2022/7/22.
//

#include "LuaEvent.h"
#include "GELog.h"
#include "LuaEngine.h"

LuaEvent::LuaEvent() {
	this->nil = luabridge::LuaRef(LuaEngine::Instance()->GetMainLuaState(), 0);
	for(GE::Uint32 _i = 0; _i < MAX_EVENT_PARAM; _i++){
		zeroParams.emplace_back(nil);
	}
}

GE::Int32 LuaEvent::Init() {
	RegLuaEvent();
	return 0;
}


GE::Int32 LuaEvent::CCallLuaPerSecond() {
	if(this->i_mCallPerSecondIndex == 0){
		return 0;
	}
	return this->TriggerEvent(this->i_mCallPerSecondIndex);
}

GE::Int32 LuaEvent::CCallLuaPerMinute() {
	if(this->i_mCallPerMinuteIndex == 0){
		return 0;
	}
	return this->TriggerEvent(this->i_mCallPerMinuteIndex);
}

GE::Int32 LuaEvent::CCallLuaPerHour() {
	if(this->i_mCallPerHourIndex == 0){
		return 0;
	}
	return this->TriggerEvent(this->i_mCallPerHourIndex);
}

GE::Int32 LuaEvent::CCallLuaPerDay() {
	if(this->i_mCallPerDayIndex == 0){
		return 0;
	}
	return this->TriggerEvent(this->i_mCallPerDayIndex);
}

GE::Int32 LuaEvent::SetCallPerIndex(GE::Int32 perSecondIndex, GE::Int32 perMinuteIndex, GE::Int32 perHourIndex, GE::Int32 perDayIndex) {
	Instance()->i_mCallPerSecondIndex = perSecondIndex;
	Instance()->i_mCallPerMinuteIndex = perMinuteIndex;
	Instance()->i_mCallPerHourIndex = perHourIndex;
	Instance()->i_mCallPerDayIndex = perDayIndex;
	return 1;
}

GE::Int32 LuaEvent::RegEvent(GE::Int32 event, luabridge::LuaRef callback) {
	// 注册一个事件和回调
	EventMap::iterator it = Instance()->m_mEventFunctionList.find(event);
	if(it == this->m_mEventFunctionList.end()){
		std::tie(it, std::ignore) = this->m_mEventFunctionList.insert({event, EventList()});
	}
	EventList& v = it->second;
	v.emplace_back(callback);
	return 1;
}
GE::Int32 LuaEvent::TriggerEvent(GE::Int32 event) {
	return this->TriggerEvent(event, zeroParams);
}

GE::Int32 LuaEvent::TriggerEvent(GE::Int32 event, CallParam params) {
	// 触发一个事件
	EventMap::iterator it = Instance()->m_mEventFunctionList.find(event);
	if(it == this->m_mEventFunctionList.end()){
		return 0;
	}
	GE::Uint32 _i = params.size();
	for(;_i < MAX_EVENT_PARAM;_i++){
		// 补充参数0
		params.emplace_back(nil);
	}
	EventList& v= it->second;
	for(const auto& callback: v){
		GE_WIN_ASSERT(callback.isFunction());
		callback(params[0], params[1], params[2], params[3], params[4]);
	}
	return 0;
}


GE::Int32 LuaEvent::sRegEvent(lua_State *L) {
	GE::Int32 event = luabridge::get<GE::Int32>(L, 1);
	luabridge::LuaRef callback = luabridge::get<luabridge::LuaRef>(L, 2);
	GE_WIN_ASSERT(callback.isFunction());
	Instance()->RegEvent(event, callback);
	lua_pushnil(L);
	return 1;
}
GE::Int32 LuaEvent::sTriggerEvent(lua_State *L) {
	GE::Int32 event = luabridge::get<GE::Int32>(L, 1);
	GE::Uint32 top = lua_gettop(L);
	// 参数必须小于(MAX_EVENT_PARAM + 1)
	GE_WIN_ASSERT(top <= (MAX_EVENT_PARAM + 1));
	// 参数个数
	CallParam params;
	GE::Int32 _i;
	for(_i = 2; _i <= top; _i++){
		params.push_back(luabridge::get<luabridge::LuaRef>(L, _i));
	}
	Instance()->TriggerEvent(event, params);
	lua_pushnil(L);
	return 1;
}
GE::Int32 LuaEvent::sSetCallPerIndex(lua_State *L) {
	GE::Int32 perSecondIndex = luabridge::get<GE::Int32>(L, 1);
	GE::Int32 perMinuteIndex = luabridge::get<GE::Int32>(L, 2);
	GE::Int32 perHourIndex = luabridge::get<GE::Int32>(L, 3);
	GE::Int32 perDayIndex = luabridge::get<GE::Int32>(L, 4);
	Instance()->SetCallPerIndex(perSecondIndex, perMinuteIndex, perHourIndex, perDayIndex);
	lua_pushnil(L);
	return 1;
}

void RegLuaEvent(){
	luabridge::getGlobalNamespace(LuaEngine::Instance()->GetMainLuaState())
	.beginNamespace("GSEvent")
	.addCFunction("RegEvent", &LuaEvent::sRegEvent)
	.addCFunction("TriggerEvent", &LuaEvent::sTriggerEvent)
	.addCFunction("SetCallPerIndex", &LuaEvent::sSetCallPerIndex)
	.endNamespace();
}
