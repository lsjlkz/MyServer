//
// Created by lsjlkz on 2023/3/21.
//

#include "GENetMessage.h"

void MsgBase::Align() {
	if(!this->m_uSize % MSG_BASE_SIZE){
		return;
	}
	this->m_uSize += (MSG_BASE_SIZE - this->m_uSize % MSG_BASE_SIZE);
}

void MsgBase::AddRedirect1_us(GE::Uint32 uSessionId) {
	// 这里应该是已经4字节对齐了的
	// 因为要先用2的，那么2应该在栈顶，所以这里保证2是空的
	GE_WIN_ASSERT(this->mRedirect.uServerRedirectCount2 == 0)
	GE_WIN_ASSERT(this->m_uSize % MSG_BASE_SIZE == 0)
	GE_WIN_ASSERT(this->m_uSize + sizeof(GE::Uint32) <= MSG_MAX_SIZE)

	GE::Uint32* head = reinterpret_cast<GE::Uint32*>((char*) this + this->m_uSize);
	*head = uSessionId;
	this->m_uSize += sizeof(GE::Uint32);
	this->mRedirect.uServerRedirectCount1 += 1;
}

void MsgBase::AddRedirect2_us(GE::Uint32 uSessionId) {
	// 这里应该是已经4字节对齐了的
	GE_WIN_ASSERT(this->m_uSize % MSG_BASE_SIZE == 0)
	GE_WIN_ASSERT(this->m_uSize + sizeof(GE::Uint32) <= MSG_MAX_SIZE)
	GE::Uint32* head = reinterpret_cast<GE::Uint32*>((char*) this + this->m_uSize);
	*head = uSessionId;
	this->m_uSize += sizeof(GE::Uint32);
	this->mRedirect.uServerRedirectCount2 += 1;
}

void MsgBase::ClearRedirect1() {
	GE_WIN_ASSERT(this->m_uSize % MSG_BASE_SIZE == 0);
	// 因为先用2再用1，那么2肯定已经没了
	GE_WIN_ASSERT(this->Redirect().uServerRedirectCount2 == 0);
	if(this->mRedirect.uServerRedirectCount1 == 0){
		return;
	}
	this->m_uSize -= this->mRedirect.uServerRedirectCount1 * sizeof(GE::Uint32);
	if(this->m_uSize < sizeof(MsgBase)){
		// TODO 会有这种可能吗？
		this->m_uSize = sizeof(MsgBase);
	}
	this->mRedirect.uServerRedirectCount1 = 0;
}

void MsgBase::ClearRedirect2() {
	GE_WIN_ASSERT(this->m_uSize % MSG_BASE_SIZE == 0);
	// 保证重定向1还有，才能清理2的，因为先用2再用1
	GE_WIN_ASSERT(this->Redirect().uServerRedirectCount1 > 0);
	if(this->mRedirect.uServerRedirectCount2 == 0){
		return;
	}
	this->m_uSize -= this->mRedirect.uServerRedirectCount2 * sizeof(GE::Uint32);
	if(this->m_uSize < sizeof(MsgBase)){
		// TODO 会有这种可能吗？
		this->m_uSize = sizeof(MsgBase);
	}
	this->mRedirect.uServerRedirectCount2 = 0;
}

GE::Uint32 *MsgBase::GetRedirectPtr() {
	// 这里直接获取头的指针，在外面会先get到重定向的数量，外面会先清理m_uSize
	return reinterpret_cast<GE::Uint32*>((char*)this + this->m_uSize);
}
