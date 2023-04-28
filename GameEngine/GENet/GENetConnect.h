//
// Created by lsjlkz on 2023/3/1.
//

#pragma once
#ifndef MYSERVER_GENETCONNECT_H
#define MYSERVER_GENETCONNECT_H

#include <boost/asio/ip/tcp.hpp>
#include "boost/shared_ptr.hpp"
#include <boost/enable_shared_from_this.hpp>
#include "boost/thread/mutex.hpp"
#include "GENetSendBuf.h"
#include "GENetRecvBuf.h"

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
	void				AsyncRecvHead();						// 异步接受消息头
	void				AsyncRecvBody();						// 异步接受消息体
	void				HandleReadMsgHead(const boost::system::error_code &ec, size_t uTransferredBytes);					// 接收消息头的句柄
	void 				HandleReadMsgBody(const boost::system::error_code &ec, size_t uTransferredBytes);					// 接收消息体的句柄
	void 				HandleWriteMsg(const boost::system::error_code &ec, size_t uTransferredBytes);
	void				KeepAlive();							// 保持长连接

	void				Start();
	void 				Shutdown(NetConnectState state);

	bool				IsShutdown(){return m_State != enNetConnect_Work;}// 是否关闭了连接
	GE::Uint8 			GetSocketState(){return m_State;}

	BoostSocket&		Socket();
	void				SessionID(GE::Uint32 uId){this->m_uSessionId = uId;}
	GE::Uint32			SessionID(){return this->m_uSessionId;}

private:
	GENetWork*			m_pNetWork;

	BoostSocket			m_BoostSocket;

	GE::Uint32			m_uSessionId;
	GE::Uint32			m_uConnectSeconds;						// 连接时间
	NetConnectState		m_State;

	GENetBuf			m_RecvCache;							// 当前的接收
	GENetSendBuf		m_SendBuf;
	GENetRecvBuf		m_RecvBuf;

	// 读写的锁
	boost::mutex		m_SendBufMutex;
	boost::mutex		m_RecvBufMutex;

};


#endif //MYSERVER_GENETCONNECT_HF
