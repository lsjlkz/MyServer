//
// Created by lsjlkz on 2023/4/13.
//

#include "GENetRecvBuf.h"
#include "GELog.h"

GENetRecvBuf::GENetRecvBuf(GE::Uint16 uBlockSize, GE::Uint16 uBlockNum) :
		m_uBlockNum(uBlockNum),
		m_uNewCnt(0)  {
	this->m_pWriteBuf = new GENetBuf(uBlockSize);
	this->m_pReadBuf = new GENetBuf(uBlockSize);
	this->m_pBufPool = nullptr;
	this->m_pBufQueue = new tdBufQueue();
}

GENetRecvBuf::~GENetRecvBuf(void) {
	if(GE_IS_POINT_NOT_NULL(this->m_pBufPool)){
		while(!this->m_pBufPool->empty()){
			GENetBuf *pHead = this->m_pBufPool->back();
			this->m_pBufPool->pop_back();
			GE_SAFE_DELETE_POINT(pHead);
		}
	}
	if(GE_IS_POINT_NOT_NULL(this->m_pBufQueue)){
		while(!this->m_pBufQueue->empty()){
			GENetBuf *pHead = this->m_pBufQueue->front();
			this->m_pBufQueue->pop();
			GE_SAFE_DELETE_POINT(pHead);
		}
	}
	GE_SAFE_DELETE_POINT(this->m_pReadBuf);
	GE_SAFE_DELETE_POINT(this->m_pWriteBuf);
	GE_SAFE_DELETE_POINT(this->m_pBufPool)

}

GE::Uint16 GENetRecvBuf::GetPoolCnt() {
	if (GE_IS_POINT_NULL(this->m_pBufPool)) {
		return 0;
	}
	return static_cast<GE::Uint16>(this->m_pBufPool->size());
}

bool GENetRecvBuf::IsEmpty() {
	if (!this->m_pBufQueue->empty()) {
		return false;
	}
	if (!this->m_pReadBuf->CanReadSize()) {
		return false;
	}
	if (!this->m_pWriteBuf->CanReadSize()) {
		return false;
	}
	return true;
}

GENetBuf *GENetRecvBuf::NewNetBuf(GE::Uint16 uSize) {
	++this->m_uNewCnt;
	if (GE_IS_POINT_NULL(this->m_pBufPool)) {
		return new GENetBuf(uSize);
	}
	if (this->m_pBufPool->empty()) {
		return new GENetBuf(uSize);
	}
	GENetBuf *pHead = this->m_pBufPool->back();
	pHead->Reset();
	this->m_pBufPool->pop_back();
	return pHead;
}

void GENetRecvBuf::DelNetBuf(GENetBuf *pBuff) {
	if (GE_IS_POINT_NULL(this->m_pBufPool)) {
		// 池是空指针，那就不回池了
		GE_SAFE_DELETE_POINT(pBuff)
		return;
	}
	if (this->m_pBufPool->size() == this->m_pBufPool->capacity()) {
		// 池满了
		GE_SAFE_DELETE_POINT(pBuff)
		return;
	}
	// 这里可以不Reset，因为取的时候会Reset
	this->m_pBufPool->emplace_back(pBuff);
}

void GENetRecvBuf::UsePool(GE::Uint16 uSize) {
	if (uSize == 0) {
		return;
	}
	if(GE_IS_POINT_NOT_NULL(this->m_pBufPool)){
		GELog::Instance()->Log("use pool repeat");
		return;
	}
	this->m_pBufPool = new tdBufVec();
	this->m_pBufPool->reserve(uSize);
}

bool GENetRecvBuf::WriteBytes(MsgBase* pMsg) {
	// pMsg是asio设置的缓冲区头，也就是刚从客户端收到的消息，没经过处理，只是简单的强制转换为MsgBase指针
	// 这里需要把这个缓冲区的数据写进本缓冲区中
	// 写进缓冲区之后呢
	// GENetWork会循环调用MoveNextMsg，获取到当前缓冲区中的消息（如果有的话）
	// 这样子就实现了缓冲区写入和读取的分离，可以多线程处理

	// 保证是4的倍数
	GE_WIN_ASSERT(pMsg->Size() % MSG_BASE_SIZE == 0)
	if(this->m_pWriteBuf->MaxSize() < pMsg->Size()){
		// 太大了
		GELog::Instance()->Log("recv buf write to long");
		return false;
	}
	if(this->m_pWriteBuf->CanWriteSize() < pMsg->Size()){
		if(this->m_pBufQueue->size() > this->m_uBlockNum){
			// 已经超过容量了
			return false;
		}
		// 剩下的部分就不要了，直接放进去队列里面
		this->m_pBufQueue->push(this->m_pWriteBuf);
		// 这里用了旧的MaxSize
		this->m_pWriteBuf = this->NewNetBuf(this->m_pWriteBuf->MaxSize());
		// 到这里就创建了一个新的缓冲区了
		// 又因为上面判断了MaxSize 肯定大于Msg的Size
		// 所以下面肯定能够写入
	}
	this->m_pWriteBuf->WriteBytes_us(pMsg, pMsg->Size());
	this->m_pWriteBuf->MoveWriteFence_us(pMsg->Size());
	return true;
}

bool GENetRecvBuf::ReadMsgFromReadBuf(MsgBase **pMsg) {
	// 这个方法将缓冲区内的buf写入到pMsg中

	// 没有可以读的了
	if(this->m_pReadBuf->CanReadSize() == 0){
		return false;
	}
	// 直接转换就好了
	*pMsg = static_cast<MsgBase*>(this->m_pReadBuf->ReadFence_us());
	this->m_pReadBuf->MoveReadFence_us((*pMsg)->Size());
	return true;
}

bool GENetRecvBuf::MoveToNextReadBuf() {
	if(this->m_pBufQueue->empty()){
		// 队列空了，拿不出来了
		if (!this->m_pWriteBuf->CanReadSize()) {
			return true;
		}
		// 直接交换读写缓冲区
		std::swap(this->m_pWriteBuf, this->m_pReadBuf);
		this->m_pWriteBuf->Reset();
		return true;
	}
	// 先把旧的回池了先
	this->DelNetBuf(this->m_pReadBuf);
	this->m_pReadBuf = this->m_pBufQueue->front();
	this->m_pBufQueue->pop();
	return true;
}
