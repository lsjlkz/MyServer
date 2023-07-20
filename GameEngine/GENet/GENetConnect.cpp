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
#include "GEProcess.h"
#include "GSDefine.h"


GENetConnect::GENetConnect(GENetWork *pNetWork, GEDefine::ConnectParam &CP):
	m_pNetWork(pNetWork),
	m_BoostSocket(pNetWork->IOS()),
	m_RecvBuf(CP.uRecvBlockSize, CP.uRecvBlockNum),
	m_SendBuf(CP.uSendBlockSize, CP.uSendBlockNum),
	m_RecvCache(CP.uRecvBlockSize),
	m_State(NetConnectState::enNetConnect_Work),
	m_uWaitSeconds(DefaultWaitTime),
	m_uWho(EndPoindType::EndPoint_None){
}

GENetConnect::~GENetConnect() {
	this->m_pNetWork = nullptr;
	this->m_uSessionId = MAX_UINT32;
	this->m_uWho = 0;
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
		this->Shutdown(NetConnectState::enNetConnect_SendBufFull);
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
	if (ec)
	{
		this->Shutdown(NetConnectState::enNetConnect_RemoteClose);
		return;
	}
	// 这里只是移动写指针，并没有移动HeadPtr
	this->m_RecvCache.MoveWriteFence_us(sizeof(MsgBase));
	this->m_uLastestRecvSeconds = GEDateTime::Instance()->Seconds();
	this->AsyncRecvBody();
}

void GENetConnect::HandleReadMsgBody(const boost::system::error_code &ec, size_t uTransferredBytes) {
	// 尝试接受客户端的消息
	if (this->IsShutdown()){
		return;
	}
	if (ec)
	{
		this->Shutdown(NetConnectState::enNetConnect_RemoteClose);
		return;
	}
	MsgBase* pMsg = static_cast<MsgBase*>(m_RecvCache.HeadPtr());
	// 这里只是移动写指针，并没有移动HeadPtr
	this->m_RecvCache.MoveWriteFence_us(pMsg->Size() - sizeof(MsgBase));
	// 客户端已经处理好4字节对齐了的，这里保证一下
	pMsg->Align();
	this->RecvMsgCompletely();
	this->m_uLastestRecvSeconds = GEDateTime::Instance()->Seconds();
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
		this->Shutdown(NetConnectState::enNetConnect_RemoteClose);
		return;
	}
	// 继续异步发送
	this->AsyncSendBlock();
}

void GENetConnect::RemoteEndPoint(std::string &sIP, GE::Uint16 &uPort) {
	GE_WIN_ASSERT(this->Socket().is_open());
	sIP = this->Socket().remote_endpoint().address().to_string();
	uPort = this->Socket().remote_endpoint().port();
}


int GENetConnect::RecvMsgCompletely(){
	GELog::Instance()->Log("RecvMsgCompletely");
	// TODO
	// 项目中主线程会迭代所有的链接，知道找到一个有效且有消息的连接，这里是否可以把session写入到一个vector中来迭代
	MsgBase* pMsg = static_cast<MsgBase*>(m_RecvCache.HeadPtr());
	tdMsgRedirect rd = pMsg->Redirect();
	GELog::Instance()->Log("Who", this->Who());
	if(this->IsWhoNone()){

	}
	else if(this->IsWhoClient()){
		// 客户端转发的消息
		GELog::Instance()->Log("uClientRedirect", rd.uClientRedirect);
		if(rd.uClientRedirect != CLIENT_REDIRECT_NONE){
			// 需要重定向到其他进程
			GE::Uint32 uRedirectSessionID = GEProcess::Instance()->GetClientRedirect(rd.uClientRedirect);
			if(uRedirectSessionID == MAX_UINT32){
				// 错误了
				return 2;
			}
			// 可以覆盖重定向信息了，补充为客户端的sessionId，因为之后可能会用到
			rd.uClientSessionID = this->m_uSessionId;
			GELog::Instance()->Log("redirect to ", uRedirectSessionID);
			// 这里转发到
			this->m_pNetWork->SendBytes(uRedirectSessionID, pMsg, pMsg->Size());
		}
		return 0;
	}
	else if(this->IsWhoGateway()){
		// 网关的话，那就不用重定向
	}
	else{
		// TODO
	}
	// 为什么这里不直接处理消息，因为消息接收是多线程的，这里写入到缓冲区中，在主线程中遍历处理。
	this->m_RecvBufMutex.lock();
	bool ret = this->m_RecvBuf.WriteBytes(pMsg);
	this->m_RecvBufMutex.unlock();
	if(ret){
		return 0;
	}
	return 1;
}

void GENetConnect::Start() {
	this->m_uConnectSeconds = GEDateTime::Instance()->UnixTime();
	this->m_uLastestRecvSeconds = GEDateTime::Instance()->UnixTime();
	GELog::Instance()->Log("connect successfully", this->m_uSessionId);
	this->KeepAlive();
	this->AsyncRecvHead();
}

void GENetConnect::KeepAlive() {
	// 保持长连接
	boost::asio::socket_base::keep_alive option(true);
	this->m_BoostSocket.set_option(option);
}

bool GENetConnect::IsLongTimeNoRecv(){
	return (this->m_uLastestRecvSeconds + this->m_uWaitSeconds) < GEDateTime::Instance()->Seconds();
}

bool GENetConnect::ReadMsg(MsgBase** pMsg){
	// 读一条消息
	if(this->m_RecvBuf.ReadMsgFromReadBuf(pMsg)){
		// 有消息
		return true;
	}
	else{
		// 没有消息
		//移到下一个
		this->m_RecvBufMutex.lock();
		this->m_RecvBuf.MoveToNextReadBuf();
		this->m_RecvBufMutex.unlock();
		return false;
	}
}

void GENetConnect::WritePing(){
	MsgBase msg;
	msg.Type(CMsgType::CMsg_Ping);
	this->WriteBytes(&msg, sizeof(MsgBase));
}

void GENetConnect::WritePong(){
	MsgBase msg;
	msg.Type(CMsgType::CMsg_Pong);
	this->WriteBytes(&msg, sizeof(MsgBase));
}

void GENetConnect::Shutdown(NetConnectState state) {
	// 关闭socket
	if(this->IsShutdown()){
		// 重复关闭
		return;
	}
	try{
		this->Socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
#ifdef WIN
		this->Socket().close();
#elif LINUX
		this->Socket().cancel();
#endif
		this->m_State = state;
	}
	catch(std::exception& e){
		GELog::Instance()->Log(e.what());
	}
}

void GENetConnect::Who(GE::Uint16 uWho){
	this->m_uWho = uWho;
}

bool GENetConnect::IsWhoNone() const {
	return this->m_uWho == EndPoindType::EndPoint_None;
}

bool GENetConnect::IsWhoClient() const {
	// 是否是客户端连接
	return this->m_uWho == EndPoindType::EndPoint_ClientGateway_;
}

bool GENetConnect::IsWhoGateway() const{
	// 是否是网关
	return this->m_uWho == EndPoindType::EndPoint_GatewayLogic_ || this->m_uWho == EndPoindType::EndPoint_GatewayWorld_;
}


