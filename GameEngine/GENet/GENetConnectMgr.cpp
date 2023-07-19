//
// Created by lsjlkz on 2023/3/1.
//

#include "GENetConnectMgr.h"
#include "GELog.h"

GENetConnectMgr::GENetConnectMgr(GE::Uint32 uMaxConnect):
		m_uMaxSize(uMaxConnect),
		m_uConnectCnt(0)
{
	// new一个数组，作为连接管理器使用
	this->m_pConnectArr = new ConnectPtr[uMaxConnect];
	this->m_pConnectMutexArr = new MutexPtr[uMaxConnect];
	for(GE::Uint32 uid = 0; uid < uMaxConnect; ++uid){
		this->m_pConnectArr[uid] = nullptr;
		// 锁
		this->m_pConnectMutexArr[uid] = new Mutex();
		this->m_freeUIDQueue.push(uid);
	}
}

GENetConnectMgr::~GENetConnectMgr() {
	for(GE::Uint32 i = 0; i < this->m_uMaxSize; ++i){
		GE_SAFE_DELETE_POINT(this->m_pConnectArr[i]);
		GE_SAFE_DELETE_POINT(this->m_pConnectMutexArr[i]);
	}
	GE_SAFE_DELETE_POINT(this->m_pConnectArr);
	GE_SAFE_DELETE_POINT(this->m_pConnectMutexArr);
	while(!this->m_freeUIDQueue.empty()){
		this->m_freeUIDQueue.pop();
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
	if(0 == this->FreeCnt()){
		// 空了
		return false;
	}
	uid = this->m_freeUIDQueue.front();
	this->m_freeUIDQueue.pop();
	GE_ASSERT(GE_IS_POINT_NULL(this->m_pConnectArr[uid]));
	this->m_pHolder.insert(std::make_pair(uid, spConnect));
	this->m_pConnectArr[uid] = spConnect.get();
	spConnect->SessionID(uid);
	this->m_uConnectCnt += 1;
	return true;
}

bool GENetConnectMgr::DelConnect(GE::Uint32 uid) {
	// 删除一个连接
	GE_ASSERT(GE_IS_POINT_NOT_NULL(this->m_pConnectArr[uid]));
	this->m_pConnectMutexArr[uid]->lock();
	this->m_pConnectArr[uid]->Shutdown(enNetConnect_LocalClose);
	this->m_pConnectArr[uid] = nullptr;
	this->m_pHolder.erase(uid);
	this->m_pConnectMutexArr[uid]->unlock();
	// uid回池
	this->m_freeUIDQueue.push(uid);
	this->m_uConnectCnt -= 1;
	return true;
}

void GENetConnectMgr::ForceShutdownIllegalConnect_us() {
	// TODO 处理非法连接
//	GELog::Instance()->Log("ForceShutdownIllegalConnect_us");
//	for (auto& [uid, sharePtr] : this->m_pHolder) {
//		GENetConnect *pConnect = sharePtr.get();
//		GELog::Instance()->Log("IsLongTimeNoRecv", pConnect->IsLongTimeNoRecv());
//		if(pConnect->IsLongTimeNoRecv()){
//			DelConnect(uid);
//		}
//		GELog::Instance()->Log("ForceShutdownIllegalConnect_us", (GE::Uint32)uid);
//		GELog::Instance()->Log("ForceShutdownIllegalConnect_us", pConnect->SessionID());
//	}

}


GENetConnectMgr::ConnectPtr GENetConnectMgr::IterNextConnect(MsgBase** pMsg){
	// 迭代下一个连接
	GE::Uint32 maxIterCnt = this->m_uMaxSize;

	for(; maxIterCnt > 0; maxIterCnt -- ){
		this->m_uIterConnectIndex ++;
		if(this->m_uIterConnectIndex >= this->m_uMaxSize){
			this->m_uIterConnectIndex = 0;
		}
		GENetConnect* geNetConnect = this->m_pConnectArr[this->m_uIterConnectIndex];
		if(geNetConnect == nullptr){
			continue;
		}
		// TODO 这里可以用一些方法来检测，是否断线了，或者需要处理
		if(geNetConnect->IsLongTimeNoRecv()){
			// 太久没有接收消息了
			// TODO
			this->DelConnect(geNetConnect->SessionID());
			continue;
		}
		if(!geNetConnect->ReadMsg(pMsg)){
			continue;
		}
		// 有效链接看看有没有消息
		return geNetConnect;
	}
	return nullptr;
}