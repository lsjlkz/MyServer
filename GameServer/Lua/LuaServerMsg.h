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
#include "LuaFunctionProxy.h"

// 注册Lua层消息回调的

class LuaServerMsg {
public:
	LuaServerMsg(GE::Uint32 _id, const luabridge::LuaRef& _callback):uMsgType(_id), luaFunctionProxy(_callback){
	}
	void Call(GE::Uint32 uSessionID, const luabridge::LuaRef& rParam);

private:
	GE::Uint32 uMsgType;
	LuaFunctionProxy luaFunctionProxy;

};

class LuaClientMsg{
public:
	LuaClientMsg(GE::Uint32 _id, const luabridge::LuaRef& _callback):uMsgType(_id), luaFunctionProxy(_callback){}
	void Call(LuaRole* pRole, const luabridge::LuaRef& rParam);
private:
	GE::Uint32 uMsgType;
	LuaFunctionProxy luaFunctionProxy;

};


class LuaServerMsgMgr:public GESingleton<LuaServerMsgMgr>{
	// 管理服务器进程消息的
	typedef std::unordered_map<GE::Uint16, LuaServerMsg> tdMsgMgr;

public:
	bool		RegServerMsg(GE::Uint16 uMsgType, luabridge::LuaRef callback);
	bool 		CallServerMsg(GE::Uint16 uMsgType, GE::Uint32 uSessionID, const luabridge::LuaRef& rParam);

private:
	tdMsgMgr msgMgr;
};

#endif //MYSERVER_LUASERVERMSG_H
