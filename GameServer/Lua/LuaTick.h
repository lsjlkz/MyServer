//
// Created by lsjlkz on 2023/6/30.
//

#ifndef MYSERVER_LUATICK_H
#define MYSERVER_LUATICK_H

#include <queue>
#include "LuaEngine.h"
#include "GEDefine.h"
#include "GESingleton.h"
#include "LuaBridge/LuaBridge.h"

class Tick{
public:
	Tick(GE::Uint64 tickID, const luabridge::LuaRef& owner, const luabridge::LuaRef& callback, const luabridge::LuaRef& param) noexcept :m_uTickID(tickID), m_rOwner(owner), m_rCallback(callback), m_rParam(param){}
	~Tick();

	GE::Uint64 AutoTriggerSeconds() const;
	bool IsOverTime();
	bool IsBelongToMe(const luabridge::LuaRef& owner){return owner.rawequal(this->m_rOwner);}	//	是否属于我 TODO 不确定是否用这个比较
	void Call();		// 主动触发回调
	void Call(const luabridge::LuaRef& callArgv);	// 带参数的触发

	bool operator<(Tick& other) const;	// 比较


private:
	GE::Uint64 m_uTickID;
	luabridge::LuaRef m_rOwner;	// 任务所有者
	luabridge::LuaRef m_rCallback;	// 任务回调
	luabridge::LuaRef m_rParam;	// 任务参数
};

class LuaTick: public GESingleton<LuaTick> {

	typedef std::map<GE::Uint64, Tick*> TickMap;
public:
	LuaTick();

	GE::Uint64 RegTick(GE::Uint64 seconds, const luabridge::LuaRef& owner, const luabridge::LuaRef& callback, const luabridge::LuaRef& param);	// 注册一个任务
	bool TriggerTick(GE::Uint64 m_uTickID, const luabridge::LuaRef& owner, const luabridge::LuaRef& callArgv);// 主动触发一个任务
	bool UnregTick(GE::Uint64 m_uTickID, const luabridge::LuaRef& owner);

	void AfterCallPerSecond();			// 每秒触发过期的任务

	// 使用一个优先队列来存储
	TickMap m_pTickMap;
};


#endif //MYSERVER_LUATICK_H
