//
// Created by lsjlkz on 2023/3/1.
//

#include "GENetConnect.h"
#include "GENetWork.h"
#include "GEDateTime.h"
#include "GENetMessage.h"


GENetConnect::GENetConnect(GENetWork *pNetWork, GEDefine::ConnectParam &CP):
	m_pNetWork(pNetWork),
	m_BoostSocket(pNetWork->IOS()),
	m_RecvBuf(CP.uRecvBlockSize, CP.uRecvBlockNum),
	m_SendBuf(CP.uSendBlockSize, CP.uSendBlockNum),
	m_RecvCache(CP.uRecvBlockSize)
{
}

GENetConnect::~GENetConnect() {
	this->m_pNetWork=NULL;
}

GENetConnect::BoostSocket& GENetConnect::Socket() {
	{return m_BoostSocket;}
}

GE::Int32 GENetConnect::RecvCompletionMsg() {
	return 0;
}

void GENetConnect::SendBytes(const void* pHead, GE::Int16 uSize){
	this->WriteBytes(pHead, uSize);
}

void GENetConnect::WriteBytes(const void * pHead, GE::Int16 uSize) {

}

void GENetConnect::AsyncSendBlock() {
	// TODO 有消息要发送的话，就这里发送
	if (this->IsShutdown()){
		return;
	}
}

void GENetConnect::AsyncRecvHead() {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
}

void GENetConnect::AsyncRecvBody() {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
}

void GENetConnect::HandleReadMsgHead() {
	if (this->IsShutdown()){
		return;
	}

}

void GENetConnect::HandleReadMsgBody() {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
	m_RecvCache.Reset();
	// TODO
//	boost::asio::async_read(m_BoostSocket,
//							boost::asio::buffer(m_RecvCache.HeadPtr(), sizeof(MsgBase))
//							)

}

void GENetConnect::Start() {
	this->m_uConnectSeconds = GEDateTime::Instance()->UnixTime();
	std::cout << this->m_uSessionId << "连接成功:" << this->m_uConnectSeconds<< std::endl;
	this->KeepAlive();
	this->AsyncRecvHead();
}

void GENetConnect::KeepAlive() {
	// 保持长连接
	boost::asio::socket_base::keep_alive option(true);
	this->m_BoostSocket.set_option(option);
}

void GENetConnect::Shutdown(NetConnectState state) {
	// TODO 关闭连接
}


