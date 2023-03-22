//
// Created by lsjlkz on 2023/3/1.
//

#pragma once
#ifndef MYSERVER_GENETCONNECT_H
#define MYSERVER_GENETCONNECT_H

#include "GEProcess.h"
#include "Head.h"

class GENetWork;

// 一个连接的状态
enum NetConnectState
{
	enNetConnect_Work,
	enNetConnect_RemoteClose,
	enNetConnect_MsgError,
	enNetConnect_SendBufFull,
	enNetConnect_RecvBufFull,
	enNetConnect_ConnectFull,
	enNetConnect_LocalClose,
	enNetConnect_ForceClose,
};

class GENetConnect:public boost::enable_shared_from_this<GENetConnect> {
public:
	typedef boost::asio::ip::tcp::socket 				BoostSocket;
	typedef boost::shared_ptr<GENetConnect>				ConnectSharePtr;

	GENetConnect(GENetWork* pNetWork, GEDefine::ConnectParam& CP);
	~GENetConnect();


	GE::Int32 			RecvCompletionMsg();


	void 				SendBytes(const void* pHead, GE::Int16 uSize);
	void 				WriteBytes(const void* pHead, GE::Int16 uSize);
	void				AsyncSendBlock();						// 异步发送消息
	void				KeepAlive();							// 保持长连接

	void				Start();
	void 				Shutdown(NetConnectState state);

	BoostSocket&		Socket();
	void				SessionID(GE::Uint32 uId){this->m_uSessionId = uId;}

private:
	GENetWork*			m_pNetWork;

	BoostSocket			m_BoostSocket;

	GE::Uint32			m_uSessionId;
	GE::Uint32			m_uConnectSeconds;						// 连接时间
};


#endif //MYSERVER_GENETCONNECT_HF
