//
// Created by lsjlkz on 2023/6/30.
//

#ifndef MYSERVER_LUATICK_H
#define MYSERVER_LUATICK_H

#include <queue>
#include "GEDefine.h"
#include "GESingleton.h"
#include "LuaBridge/LuaBridge.h"

class Tick{
public:
	Tick(GE::Int64 seconds, const luabridge::LuaRef& owner, const luabridge::LuaRef& callback, const luabridge::LuaRef& param):m_uSeconds(seconds), m_rOwner(owner), m_rCallback(callback), m_rParam(param){}
	~Tick();

	void Call();		// 触发一个tick
	void Call(const luabridge::LuaRef& callArgv);	// 带参数的触发

private:
	GE::Int64 m_uSeconds;
	luabridge::LuaRef m_rOwner;
	luabridge::LuaRef m_rCallback;
	luabridge::LuaRef m_rParam;
};

class LuaTick: public GESingleton<LuaTick> {

	typedef std::priority_queue<Tick*> TickList;
public:
	LuaTick();

	GE::Int32 RegTick(GE::Int64 seconds, const luabridge::LuaRef& owner, const luabridge::LuaRef& callback, const luabridge::LuaRef& param);
	// 使用一个优先队列来存储

	TickList n_qTickList;
};


#endif //MYSERVER_LUATICK_H
