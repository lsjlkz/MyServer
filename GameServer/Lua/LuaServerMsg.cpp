//
// Created by lsjlkz on 2023/7/7.
//

#include "LuaServerMsg.h"
#include "GELog.h"

void LuaServerMsg::Call(GE::Uint32 uSessionID, const luabridge::LuaRef& rParam) {
	this->callback(uSessionID, rParam);
}

void LuaClientMsg::Call(LuaRole *pRole, const luabridge::LuaRef& rParam) {
	this->callback(*pRole, rParam);
}

bool LuaServerMsgMgr::RegServerMsg(GE::Uint16 uMsgType, luabridge::LuaRef callback) {
	GE_WIN_ASSERT(this->msgMgr.find(uMsgType) == this->msgMgr.end())
	this->msgMgr.insert(std::make_pair(uMsgType, callback));
	return true;
}

bool LuaServerMsgMgr::CallServerMsg(GE::Uint16 uMsgType, GE::Uint32 uSessionID, const luabridge::LuaRef &rParam) {
	tdMsgMgr::iterator it = this->msgMgr.find(uMsgType);
	if(it == this->msgMgr.end()){
		GELog::Instance()->Log("error server msg", uMsgType);
		return false;
	}
	LuaServerMsg luaServerMsg = it->second;
	luaServerMsg.Call(uSessionID, rParam);
	return true;
}
