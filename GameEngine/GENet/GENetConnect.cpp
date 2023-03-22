//
// Created by lsjlkz on 2023/3/1.
//

#include "GENetConnect.h"
#include "GENetWork.h"
#include "GEDateTime.h"


GENetConnect::GENetConnect(GENetWork *pNetWork, GEDefine::ConnectParam &CP):m_pNetWork(pNetWork),m_BoostSocket(pNetWork->IOS()) {
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
}

void GENetConnect::Start() {
	this->m_uConnectSeconds = GEDateTime::Instance()->UnixTime();
	std::cout << this->m_uSessionId << "连接成功:" << this->m_uConnectSeconds<< std::endl;
	this->KeepAlive();
	this->AsyncSendBlock();
}

void GENetConnect::KeepAlive() {
	// 保持长连接
	boost::asio::socket_base::keep_alive option(true);
	this->m_BoostSocket.set_option(option);
}

void GENetConnect::Shutdown(NetConnectState state) {
	// TODO 关闭连接
}


