//
// Created by lsjlkz on 2023/3/1.
//

#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "GENetConnect.h"
#include "GENetWork.h"
#include "GEDateTime.h"
#include "GELog.h"


GENetConnect::GENetConnect(GENetWork *pNetWork, GEDefine::ConnectParam &CP):
	m_pNetWork(pNetWork),
	m_BoostSocket(pNetWork->IOS()),
	m_RecvBuf(CP.uRecvBlockSize, CP.uRecvBlockNum),
	m_SendBuf(CP.uSendBlockSize, CP.uSendBlockNum),
	m_RecvCache(CP.uRecvBlockSize)
{
}

GENetConnect::~GENetConnect() {
	GE_SAFE_DELETE_POINT(this->m_pNetWork);
}

GENetConnect::BoostSocket& GENetConnect::Socket() {
	{return m_BoostSocket;}
}

GE::Int32 GENetConnect::RecvCompletionMsg() {
	return 0;
}

void GENetConnect::SendBytes(const void* pHead, GE::Int16 uSize){
	// 发送消息，写入到缓冲区中
	this->WriteBytes(pHead, uSize);
}

void GENetConnect::WriteBytes(const void * pHead, GE::Int16 uSize) {
	if(this->IsShutdown()){
		return;
	}
	// 先锁住先
	this->m_SendBufMutex.lock();
	bool ret = this->m_SendBuf.WriteBytes(pHead, uSize);
	// 解锁
	this->m_SendBufMutex.unlock();
	if(!ret){
		// 没有写入成功
		GELog::Instance()->Log("write send buf error", this->SessionID());
		this->Shutdown(enNetConnect_SendBufFull);
		return;
	}
	// 异步发送
	this->AsyncSendBlock();
}

void GENetConnect::AsyncSendBlock() {
	// 有消息要发送的话，就这里发送
	if (this->IsShutdown()){
		return;
	}
	void* pHead = nullptr;
	GE::Uint16 uSize=0;
	// 先锁住先
	this->m_SendBufMutex.lock();
	bool ret = this->m_SendBuf.HoldBlock(&pHead, uSize);
	this->m_SendBufMutex.unlock();
	if(!ret){
		// 没有消息
		return;
	}
	boost::asio::async_write(this->m_BoostSocket,
							 boost::asio::buffer(pHead, uSize),
							 boost::bind(&GENetConnect::HandleWriteMsg,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void GENetConnect::AsyncRecvHead() {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
	GELog::Instance()->Log("AsyncRecvHead", this->m_uSessionId);
	m_RecvCache.Reset();
	boost::asio::async_read(m_BoostSocket,
							boost::asio::buffer(m_RecvCache.HeadPtr(), sizeof(MsgBase)),
							// 第一个是回调，第二个是回调的对象，也就是this
							// 第三个是ec，第四个是传输字节的长度
							boost::bind(&GENetConnect::HandleReadMsgHead, shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
}

void GENetConnect::AsyncRecvBody() {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
	GELog::Instance()->Log("AsyncRecvBody", this->m_uSessionId);
	MsgBase* pMsg = static_cast<MsgBase*>(m_RecvCache.HeadPtr());
	boost::asio::async_read(m_BoostSocket,
							boost::asio::buffer(m_RecvCache.WriteFence_us(), pMsg->Size() - sizeof(MsgBase)),
							// 第一个是回调，第二个是回调的对象，也就是this
							// 第三个是ec，第四个是传输字节的长度
							boost::bind(&GENetConnect::HandleReadMsgBody, shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void GENetConnect::HandleReadMsgHead(const boost::system::error_code &ec, size_t uTransferredBytes) {
	if (this->IsShutdown()){
		return;
	}
	GELog::Instance()->Log("HandleReadMsgHead", this->m_uSessionId);
	this->AsyncRecvBody();
}

void GENetConnect::HandleReadMsgBody(const boost::system::error_code &ec, size_t uTransferredBytes) {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
	GELog::Instance()->Log("HandleReadMsgBody", this->m_uSessionId);
	this->AsyncRecvHead();

}

void GENetConnect::HandleWriteMsg(const boost::system::error_code &ec, size_t uTransferredBytes) {
	// 发送之后的回调

	this->m_SendBufMutex.lock();
	// 释放一下
	this->m_SendBuf.ReleaseBlock();

	this->m_SendBufMutex.unlock();

	if (ec)
	{
		this->Shutdown(enNetConnect_RemoteClose);
		return;
	}
	// 继续异步发送
	this->AsyncSendBlock();
}

void GENetConnect::Start() {
	this->m_uConnectSeconds = GEDateTime::Instance()->UnixTime();
	GELog::Instance()->Log("connect successfully", this->m_uSessionId);
	this->KeepAlive();
	this->AsyncRecvHead();
}

void GENetConnect::KeepAlive() {
	// 保持长连接
	boost::asio::socket_base::keep_alive option(true);
	this->m_BoostSocket.set_option(option);
}

void GENetConnect::Shutdown(NetConnectState state) {
	// TODO 关闭一条数据的连接，并不是关闭socket，只是通知对方这条消息被关闭了
}


