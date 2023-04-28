//
// Created by lsjlkz on 2023/3/20.
//

#include "GENetSendBuf.h"
#include "GELog.h"

GENetSendBuf::GENetSendBuf(GE::Uint16 uBlockSize, GE::Uint16 uBlockNum) :
		m_uBlockNum(uBlockNum),
		m_bIsHoldBlock(false),
		m_uNewCnt(0),
		m_pBufPool(nullptr){
	this->m_pReadBuf = this->NewNetBuf(uBlockSize);
	this->m_pWriteBuf = this->NewNetBuf(uBlockSize);
	this->m_pBufQueue = new tdBufQueue();
}

GENetSendBuf::~GENetSendBuf(void) {
	if(GE_IS_POINT_NOT_NULL(this->m_pBufPool)){
		while (!this->m_pBufPool->empty()) {
			delete this->m_pBufPool->back();
			this->m_pBufPool->pop_back();
		}
	}
	if(GE_IS_POINT_NOT_NULL(this->m_pBufQueue)){
		while (!this->m_pBufQueue->empty()) {
			delete this->m_pBufQueue->front();
			this->m_pBufQueue->pop();
		}
	}
	GE_SAFE_DELETE_POINT(this->m_pReadBuf);
	GE_SAFE_DELETE_POINT(this->m_pWriteBuf);
	GE_SAFE_DELETE_POINT(this->m_pBufPool)
}

GE::Uint16 GENetSendBuf::GetPoolCnt() {
	if(GE_IS_POINT_NULL(this->m_pBufPool)){
		return 0;
	}
	return static_cast<GE::Uint16>(this->m_pBufPool->size());
}

bool GENetSendBuf::IsEmpty() {
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

GENetBuf *GENetSendBuf::NewNetBuf(GE::Uint16 uSize) {
	++this->m_uNewCnt;
	if(GE_IS_POINT_NULL(this->m_pBufPool)){
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

void GENetSendBuf::DelNetBuf(GENetBuf *pBuff) {
	if(GE_IS_POINT_NULL(this->m_pBufPool)){
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

void GENetSendBuf::UsePool(GE::Uint16 uSize) {
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

bool GENetSendBuf::WriteBytes(const void *pHead, GE::Uint16 uSize) {
	// 这个是写入到缓冲区内
	// GENetConnect会Hold住一块缓冲区，然后直接发送出去
	// 这样子就可以实现缓冲区写入和发送的逻辑分离，可以多线程操作
	while(this->m_pWriteBuf->CanWriteSize() < uSize){
		if(this->m_pBufQueue->size() > this->m_uBlockNum){
			// 已经超了
			return false;
		}
		// 不够长度了
		GE::Uint16 canWriteSize = this->m_pWriteBuf->CanWriteSize();
		// 写入部分
		this->m_pWriteBuf->WriteBytes_us(pHead, canWriteSize);
		this->m_pWriteBuf->MoveWriteFence_us(canWriteSize);
		// 剩下的
		uSize -= canWriteSize;
		// 插入到队列中
		this->m_pBufQueue->emplace(this->m_pWriteBuf);
		// 拿到一个新的
		this->m_pWriteBuf = this->NewNetBuf(this->m_pWriteBuf->MaxSize());
		// 继续写入剩下的
		pHead = (GE::Uint16 *) pHead + canWriteSize;
	}
	// 这里肯定够了
	if(uSize){
		this->m_pWriteBuf->WriteBytes_us(pHead, uSize);
		this->m_pWriteBuf->MoveWriteFence_us(uSize);
	}
	return true;
}

bool GENetSendBuf::HoldBlock(void **pHead, GE::Uint16 &uSize) {
	if (this->m_bIsHoldBlock) {
		return false;
	}
	// 这里的this->m_pReadBuf已经在上一次读过了的
	if (!this->m_pBufQueue->empty()) {
		// 队列不为空，继续读队列
		// 先把当前的读buf回池
		this->DelNetBuf(this->m_pReadBuf);
		this->m_pReadBuf = this->m_pBufQueue->front();
		this->m_pBufQueue->pop();
	} else {
		// 队列为空，读写的buf
		if (!this->m_pWriteBuf->CanReadSize()) {
			return false;
		}
		std::swap(this->m_pWriteBuf, this->m_pReadBuf);
		this->m_pWriteBuf->Reset();
	}
	*pHead = this->m_pReadBuf->HeadPtr();
	uSize = this->m_pReadBuf->CanReadSize();
	this->m_bIsHoldBlock = true;
	return true;
}

void GENetSendBuf::ReleaseBlock() {
	GE_WIN_ASSERT(this->m_bIsHoldBlock == true);
	this->m_pWriteBuf->Reset();
	this->m_bIsHoldBlock = false;
}