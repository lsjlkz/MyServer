//
// Created by lsjlkz on 2023/4/13.
//

#include "GENetRecvBuf.h"

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
		std::cout << "use pool repeat" << std::endl;
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


	return false;
}

bool GENetRecvBuf::ReadMsgFromReadBuf(MsgBase **pMsg) {
	return false;
}

bool GENetRecvBuf::MoveToNextReadBuf() {
	return false;
}
