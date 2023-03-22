//
// Created by lsjlkz on 2023/3/1.
//

#include "GENetConnectMgr.h"

GENetConnectMgr::GENetConnectMgr(GE::Uint32 uMaxConnect) {
	this->m_uMaxSize = uMaxConnect;
	this->m_uConnectCnt = 0;
	// new一个数组，作为连接管理器使用
	this->m_pConnectArr = new ConnectPtr[uMaxConnect];
	this->m_pConnectMutexArr = new MutexPtr[uMaxConnect];
	for(GE::Uint32 uid = 0; uid < uMaxConnect; ++uid){
		this->m_pConnectArr[uid] = nullptr;
		// 锁
		this->m_pConnectMutexArr[uid] = new Mutex();
		this->m_UIDQueue.push(uid);
	}
}

GENetConnectMgr::~GENetConnectMgr() {
	for(GE::Uint32 i = 0; i < this->m_uMaxSize; ++i){
		GE_SAFE_DELETE_POINT(this->m_pConnectArr[i]);
		GE_SAFE_DELETE_POINT(this->m_pConnectMutexArr[i]);
	}
	GE_SAFE_DELETE_POINT(this->m_pConnectArr);
	GE_SAFE_DELETE_POINT(this->m_pConnectMutexArr);
	while(!this->m_UIDQueue.empty()){
		this->m_UIDQueue.pop();
	}
}

GENetConnect *GENetConnectMgr::FindConnect(GE::Uint32 uid) {
	if(!this->HasConnect(uid)){
		return nullptr;
	}
	return m_pConnectArr[uid];
}

bool GENetConnectMgr::HasConnect(GE::Uint32 uid) {
	if(this->m_pHolder.find(uid) == this->m_pHolder.end()){
		return false;
	}
	return true;
}

bool GENetConnectMgr::AddConnect(GENetConnect::ConnectSharePtr &spConnect, GE::Uint32 &uid) {
	// 添加一个连接
	if(this->m_UIDQueue.empty()){
		// 空了
		return false;
	}
	uid = this->m_UIDQueue.front();
	this->m_UIDQueue.pop();
	GE_ASSERT(this->m_pConnectArr[uid] == nullptr);
	this->m_pHolder.insert(std::make_pair(uid, spConnect));
	this->m_pConnectArr[uid] = spConnect.get();
	spConnect->SessionID(uid);
	return true;
}

bool GENetConnectMgr::DelConnect(GE::Uint32 uid) {
	// 删除一个连接
	GE_ASSERT(this->m_pConnectArr[uid] != nullptr);
	this->m_pConnectMutexArr[uid]->lock();
	this->m_pConnectArr[uid]->Shutdown(enNetConnect_LocalClose);
	this->m_pConnectArr[uid] = nullptr;
	this->m_pHolder.erase(uid);
	this->m_pConnectMutexArr[uid]->unlock();
	// uid回池
	this->m_UIDQueue.push(uid);
	return true;
}

void GENetConnectMgr::ForceShutdownIllegalConnect_us() {
	// TODO 处理非法连接
}
