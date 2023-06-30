//
// Created by lsjlkz on 2023/6/30.
//

#include "LuaTick.h"
#include "GEDateTime.h"

Tick::~Tick() {
	// 析构
	this->m_rOwner.pop();
	this->m_rCallback.pop();
	this->m_rParam.pop();
}


void Tick::Call() {
	this->m_rCallback(this->m_rOwner, 0, this->m_rParam);
}

void Tick::Call(const luabridge::LuaRef& callArgv) {
	this->m_rCallback(this->m_rOwner, callArgv, this->m_rParam);
}

LuaTick::LuaTick() {

}

GE::Int32 LuaTick::RegTick(GE::Int64 seconds, const luabridge::LuaRef& owner, const luabridge::LuaRef& callback, const luabridge::LuaRef& param) {
	GE::Int64 targetSeconds = GEDateTime::Instance()->Seconds() + seconds;
	// 这里需要使用拷贝构造，不然就会被GC
	Tick* tick = new Tick(seconds, luabridge::LuaRef(owner), luabridge::LuaRef(callback), luabridge::LuaRef(param));
	n_qTickList.emplace(tick);
	return 0;
}
