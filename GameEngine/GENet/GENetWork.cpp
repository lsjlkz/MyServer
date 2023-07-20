//
// Created by lsjlkz on 2022/6/2.
//

#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/connect.hpp>
#include "GENetWork.h"
#include "GELog.h"
#include "GEProcess.h"

GENetWork::GENetWork(GE::Int32 uMaxConnect, GE::Int32 Thread):m_ConnectMgr(uMaxConnect),m_pAcceptor(nullptr) {
	// 多个线程
	for(GE::Int32 i = 0; i < Thread; ++i){
		this->m_pNetWorkThreads.push_back(NULL);
	}
}

GE::Int32 GENetWork::Listen_MT(GE::Int32 uListenPort) {
	// 监听端口
	if(GE_IS_POINT_NULL(this->m_pAcceptor)){
        try{
#ifdef WIN
			// 这里开启一个
			this->m_pAcceptor = new tdBoostAcceptor(m_ioContext, tdBoostEndPoint(tdBoostAddV4::from_string("0.0.0.0"), uListenPort),
													false);
#elif LINUX
			this->m_pAcceptor = new tdBoostAcceptor(m_ioContext, tdBoostEndPoint(tdBoostAddV4::from_string("0.0.0.0"), uListenPort),
													true);
#endif
			this->AsyncAccept_NT();
        }
		catch (std::exception &e)
		{
			GELog::Instance()->Log(e.what());
			GELog::Instance()->Log("listen port error", uListenPort);
			return 0;
		}
		return 1;
    }
	return 0;
}
void GENetWork::Start_MT(){
	// 这里开了多个线程接收io
	for(GE::Int32 i = 0; i < this->m_pNetWorkThreads.size(); ++i){
		this->m_pNetWorkThreads.at(i) = new tdBoostThread (boost::bind(&GENetWork::BoostAsioRun, this));
	}
	this->m_bIsRun = true;
}

void GENetWork::Stop_MT(){
	this->m_bIsRun = false;
	this->m_bIsStop = true;

}

GENetWork::tdBoostIOContext& GENetWork::IOS() {
	return this->m_ioContext;
}

void GENetWork::BoostAsioRun(){
	this->m_ioContext.run();
}

void GENetWork::AsyncAccept_NT(){
	// 这里会new一个等待连接
	if(GE_IS_POINT_NULL(this->m_pAcceptor)){
		return;
	}

	GENetConnect::ConnectSharePtr spConnect = GENetConnect::ConnectSharePtr(new GENetConnect(this, GEProcess::Instance()->DefualConnectParam));
	this->m_pAcceptor->async_accept(spConnect->Socket(),
									boost::bind(&GENetWork::HandleAccept_NT, this, spConnect, boost::asio::placeholders::error));

}

void GENetWork::HandleAccept_NT(GENetConnect::ConnectSharePtr s_pConnect, const boost::system::error_code &error) {
	// 客户端请求连接
	this->m_ConnectMutex.lock();
	if(error){
		s_pConnect->Shutdown(NetConnectState::enNetConnect_ConnectFull);
	}else{
		if(this->m_ConnectMgr.FreeCnt() >= 1){
			// 可以连接
			GE::Uint32 uSessionID;
			if(this->m_ConnectMgr.AddConnect(s_pConnect, uSessionID)){
				s_pConnect->Start();
			}else{
				// 先关闭连接
				s_pConnect->Shutdown(NetConnectState::enNetConnect_ConnectFull);
				// 然后再尝试清理
				this->m_ConnectMgr.ForceShutdownIllegalConnect_us();
			}
		}else{
			// 满了
			s_pConnect->Shutdown(NetConnectState::enNetConnect_ConnectFull);
			this->m_ConnectMgr.ForceShutdownIllegalConnect_us();
		}
	}
	this->m_ConnectMutex.unlock();
	this->AsyncAccept_NT();

}

bool GENetWork::Connect_MT(const char* sIP, GE::Uint32 uPort, GE::Uint32& uSessionID, GE::Uint16 uWho, void* pBindPoint, GEDefine::ConnectParam* pCP){
	// 主线程连接
	// 参数分别是ip，端口，连接类型，
	// TODO 要判断是否是主线程
	if(pCP == nullptr){
		// 没有给的话，那就默认的参数
		pCP = &GEProcess::Instance()->DefualConnectParam;
	}
	// 要解析域名
	boost::asio::ip::tcp::resolver resolver(this->m_ioContext);
	boost::asio::ip::tcp::resolver::query query(sIP, std::to_string(uPort));
	auto endPoints = resolver.resolve(query);
	GENetConnect* pConnect = new GENetConnect(this, *pCP);
	boost::shared_ptr<GENetConnect> spConnect = boost::shared_ptr<GENetConnect>(pConnect);
	try {
		boost::asio::connect(spConnect->Socket(), endPoints);
	}catch(std::exception& e) {
		GELog::Instance()->Log("err Connect_MT", e.what());
		return false;
	}
	this->m_ConnectMutex.lock();
	bool ret = this->m_ConnectMgr.AddConnect(spConnect, uSessionID);
	if(ret){
		// 设置连接类型
		pConnect->Who(uWho);
		// TODO 可能还需要设置连接信息，比如session_id
		// 留着备用
		// 开始连接
		pConnect->Start();
	}
	this->m_ConnectMutex.unlock();
	return true;
}

bool GENetWork::HasConnect(GE::Uint32 uSessionId) {
	return this->m_ConnectMgr.HasConnect(uSessionId);
}

void GENetWork::SendBytes_MT(GE::Uint32 uSessionID, void *pHead, GE::Uint32 uSize) {
	// 主线程发送数据
	// 参数分别是sessionId，数据地址和数据大小
	if(!this->HasConnect(uSessionID)){
		// 没有这个连接
		GELog::Instance()->Log("no session", uSessionID);
		return;
	}
	GENetConnect* pConnect = this->m_ConnectMgr.FindConnect(uSessionID);

	GE_WIN_ASSERT(GE_IS_POINT_NOT_NULL(pConnect));

	pConnect->SendBytes(pHead, uSize);

}

void GENetWork::SendBytes(GE::Uint32 uSessionID, void *pHead, GE::Uint32 uSize) {
	// 非主线程发送数据
	// 这是多线程的，需要加锁控制
	if(!this->HasConnect(uSessionID)){
		// 没有这个连接
		return;
	}
	GENetConnect* pConnect = this->m_ConnectMgr.FindConnect(uSessionID);
	GE_WIN_ASSERT(GE_IS_POINT_NOT_NULL(pConnect));

	this->m_ConnectMutex.lock();
	pConnect->SendBytes(pHead, uSize);
	this->m_ConnectMutex.unlock();
}

bool GENetWork::MoveToNextMsg(){
	GENetConnect* pConnect = this->m_ConnectMgr.IterNextConnect(&this->m_pCurMsg);
	if(pConnect == nullptr){
		// 无效连接
		return false;
	}
	this->m_pIterConnect = pConnect;
	return true;
}

void GENetWork::DisConnect_MT(GE::Uint32 uSessionID) {
	if(!this->m_ConnectMgr.HasConnect(uSessionID)){
		return;
	}
	this->m_ConnectMgr.DelConnect(uSessionID);
}
