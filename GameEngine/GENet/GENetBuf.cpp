//
// Created by lsjlkz on 2023/3/20.
//

#include "GENetBuf.h"
#include "GEDefine.h"

GENetBuf::GENetBuf(GE::Uint16 uMaxSize) {
	this->m_uMaxSize = uMaxSize;
	this->m_uWriteSize = 0;
	this->m_uReadSize = 0;
	this->m_pHead = new char[m_uMaxSize];
}

GENetBuf::~GENetBuf() {
	GE_SAFE_DELETE_POINT(this->m_pHead);
}

void GENetBuf::WriteBytes_us(const void* phead, GE::Uint16 uSize){
	std::memcpy((this->m_pHead + this->m_uWriteSize), phead, uSize);
}