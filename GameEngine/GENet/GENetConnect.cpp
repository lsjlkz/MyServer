//
// Created by lsjlkz on 2023/3/1.
//

#include "GENetConnect.h"
#include "GENetWork.h"


GENetConnect::GENetConnect(GENetWork *pNetWork, GEDefine::ConnectParam &CP):m_pNetWork(pNetWork),m_BoostSocket(pNetWork->IOS()) {
}

GENetConnect::~GENetConnect() {
	this->m_pNetWork=NULL;
}

GENetConnect::BoostSocket& GENetConnect::Socket() {
	{return m_BoostSocket;}
}

GE::Int32 GENetConnect::RecvCompletionMsg() {
	return 0;
}

void GENetConnect::SendBytes(const void* pHead, GE::Int16 uSize){
	this->WriteBytes(pHead, uSize);
}

void GENetConnect::WriteBytes(const void * pHead, GE::Int16 uSize) {

}

void GENetConnect::AsyncSendBlock() {

}


