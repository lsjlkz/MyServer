//
// Created by lsjlkz on 2023/3/1.
//

#include "GENetConnectMgr.h"

GENetConnect *GENetConnectMgr::FindConnect(GE::Uint32 uId) {
	if(!this->HasConnect(uId)){
		return NULL;
	}
	return m_pConnectArr[this->UID2Index(uId)];
}

bool GENetConnectMgr::HasConnect(GE::Uint32 uId) {
	GE::Uint32 index = this->UID2Index(uId);
	if(this->m_pHolder.find(index) == this->m_pHolder.end()){
		return false;
	}
	return true;
}

