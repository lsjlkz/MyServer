//
// Created by lsjlkz on 2023/7/7.
//

#ifndef MYSERVER_LUASERVERMSG_H
#define MYSERVER_LUASERVERMSG_H

#include <unordered_map>

#include "GESingleton.h"
#include "LuaInclude.h"
#include "LuaBridge/LuaBridge.h"
#include "GEDefine.h"
#include "LuaRole.h"

// 注册Lua层消息回调的

class LuaServerMsg {
public:
	LuaServerMsg(GE::Int32 _id, const luabridge::LuaRef& _callback):id(_id), callback(_callback){}
	void Call(GE::Uint32 uSessionID, const luabridge::LuaRef& rParam);
private:
	GE::Int32 id;
	luabridge::LuaRef callback;

};

class LuaClientMsg{
public:
	LuaClientMsg(GE::Int32 _id, const luabridge::LuaRef& _callback):id(_id), callback(_callback){}
	void Call(LuaRole* pRole, const luabridge::LuaRef& rParam);
private:
	GE::Int32 id;
	luabridge::LuaRef callback;

};


class LuaServerMsgMgr:public GESingleton<LuaServerMsgMgr>{
	typedef std::unordered_map<GE::Uint16, LuaServerMsg> tdMsgMgr;

public:
	bool		RegServerMsg(GE::Uint16 uMsgType, luabridge::LuaRef callback);
	bool 		CallServerMsg(GE::Uint16 uMsgType, GE::Uint32 uSessionID, const luabridge::LuaRef& rParam);

private:

	tdMsgMgr msgMgr;
};

#endif //MYSERVER_LUASERVERMSG_H
