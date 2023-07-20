//
// Created by lsjlkz on 2023/6/30.
//

#include "LuaTick.h"
#include "LuaTickUID.h"
#include "GEDateTime.h"
#include "GELog.h"

Tick::~Tick() {
	// 析构
	// 是否需要手动析构？
	// 应该不需要手动析构
	// 因为这个不是new的，而是拷贝构造的，会自动析构
}


GE::Uint64 Tick::AutoTriggerSeconds() const {
	return (this->m_uTickID >> GEUID_TARGET_SECONDS_OFFSET) & GEUID_TARGET_SECONDS_BIT;
}

bool Tick::IsOverTime() {
	return this->AutoTriggerSeconds() < (GEDateTime::Instance()->Seconds() & GEUID_TARGET_SECONDS_BIT);
}

void Tick::Call() {
	this->m_rLuaFunctionProxy.Call(this->m_rOwner, 0, this->m_rParam);
}

void Tick::Call(const luabridge::LuaRef& callArgv) {
	this->m_rLuaFunctionProxy.Call(this->m_rOwner, callArgv, this->m_rParam);
}

bool Tick::operator<(Tick &other) const {
	// 为什么这里是大于，因为这个优先队列是最大堆
	return this->m_uTickID > other.m_uTickID;
}


LuaTick::LuaTick() = default;

GE::Uint64 LuaTick::RegTick(GE::Uint64 seconds, const luabridge::LuaRef& owner, const luabridge::LuaRef& callback, const luabridge::LuaRef& param) {
	//
	GE::Uint64 uTargetSeconds = GEDateTime::Instance()->Seconds() + seconds;
	GE::Uint64 uTickID = LuaTickUID::Instance()->AllotUID(uTargetSeconds);
	Tick* tick = new Tick(uTickID, owner, callback, param);
	m_pTickMap.insert(std::make_pair(uTickID, tick));
	return uTickID;
}

bool LuaTick::TriggerTick(GE::Uint64 m_uTickID, const luabridge::LuaRef& owner, const luabridge::LuaRef& callArgv) {
	auto it = m_pTickMap.begin();
	for(; it != m_pTickMap.end(); it++){
		if(it->first != m_uTickID){
			continue;
		}
		auto tick = it->second;
		if(tick->IsBelongToMe(owner)){
			GELog::Instance()->Log("error trigger tick wrong owner", m_uTickID);
			return false;
		}
		tick->Call(callArgv);
		break;
	}
	if(it == m_pTickMap.end()){
		// 没有找到
		return false;
	}
	m_pTickMap.erase(it);
	delete it->second;
	return true;
}

bool LuaTick::UnregTick(GE::Uint64 m_uTickID, const luabridge::LuaRef &owner) {
	auto it = m_pTickMap.begin();
	for(; it != m_pTickMap.end(); it++){
		if(it->first != m_uTickID){
			continue;
		}
		auto tick = it->second;
		if(tick->IsBelongToMe(owner)){
			GELog::Instance()->Log("error trigger tick wrong owner", m_uTickID);
			return false;
		}
		// 因为不需要有其他处理，所以可以直接删掉
		m_pTickMap.erase(it);
		delete it->second;
		return true;
	}
	return true;
}

void LuaTick::AfterCallPerSecond() {
	// 每秒调用执行
	auto it = m_pTickMap.begin();
	for(; it != m_pTickMap.end();){
		auto tick = it->second;
		// 超时了
		if(tick->IsOverTime()){
			tick->Call();
			m_pTickMap.erase(it++);
			delete tick;
		}else{
			// 后面都没超时
			it++;
//			return;
		}
	}
}
