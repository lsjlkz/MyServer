//
// Created by lsjlkz on 2023/3/20.
//

#include "GENetSendBuf.h"

GE::Uint16 GENetSendBuf::GetPoolCnt() {
	if(this->m_pBufPool == NULL){
		return 0;
	}
	return static_cast<GE::Uint16>(this->m_pBufPool->size());
}

bool GENetSendBuf::IsEmpty() {
	if(!this->m_pBufQueue.empty()){
		return false;
	}
	if(this->m_pReadBuf->CanReadSize() != 0){
		return false;
	}
	if(this->m_pWriteBuf->CanReadSize() != 0){
		return false;
	}
	return true;
}

GENetBuf *GENetSendBuf::NewNetBuf(GE::Uint16 uSize) {
	++ this->m_uNewCnt;
	if(this->m_pBufPool == nullptr){
		return new GENetBuf(uSize);
	}
	if(this->m_pBufPool->empty()){
		return new GENetBuf(uSize);
	}
	GENetBuf* pHead = this->m_pBufPool->back();
	pHead->Reset();
	this->m_pBufPool->pop_back();
	return pHead;
}

void GENetSendBuf::DelNetBuf(GENetBuf *pBuff) {
	if(this->m_pBufPool == nullptr){
		// 池是空指针，那就不回池了
		GE_SAFE_DELETE_POINT(pBuff)
		return;
	}
	if(this->m_pBufPool->size() == this->m_pBufPool->capacity()){
		// 池满了
		GE_SAFE_DELETE_POINT(pBuff)
		return;
	}
	// 这里可以不Reset，因为取的时候会Reset
	this->m_pBufPool->emplace_back(pBuff);
}

void GENetSendBuf::UsePool(GE::Uint16 uSize) {
	if(uSize == 0){
		return;
	}
	if(this->m_pBufPool != nullptr){
		std::cout << "use pool repeat" << std::endl;
		return;
	}
	this->m_pBufPool = new tdBufVec();
	this->m_pBufPool->resize(uSize);
}

bool GENetSendBuf::WriteBytes(const void *pHead, GE::Uint16 uSize) {
	if(this->m_pWriteBuf->CanWriteSize() < uSize){
		// 不够长度了
		GE::Uint16 canWriteSize = this->m_pWriteBuf->CanWriteSize();
		// 写入部分
		this->m_pWriteBuf->WriteBytes_us(pHead, canWriteSize);
		// 剩下的
		uSize -= canWriteSize;
		// 插入到队列中
		this->m_pBufQueue.emplace(this->m_pWriteBuf);
		// 拿到一个新的
		this->m_pWriteBuf = this->NewNetBuf(this->m_pWriteBuf->MaxSize());
		// 继续写入剩下的
		pHead = (const char*)pHead + canWriteSize;
		// 再次递归写入
		return this->WriteBytes(pHead, uSize);
	}else{
		// 够长度
		this->m_pWriteBuf->WriteBytes_us(pHead, uSize);
		this->m_pWriteBuf->MoveReadFence_us(uSize);
		return true;
	}
}
