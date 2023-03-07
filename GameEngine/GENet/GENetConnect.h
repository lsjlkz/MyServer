//
// Created by lsjlkz on 2023/3/1.
//

#pragma once
#ifndef MYSERVER_GENETCONNECT_H
#define MYSERVER_GENETCONNECT_H

#include "GEProcess.h"
#include "Head.h"

class GENetWork;

class GENetConnect:public boost::enable_shared_from_this<GENetConnect> {
public:
	typedef boost::asio::ip::tcp::socket 				BoostSocket;
	typedef boost::shared_ptr<GENetConnect>				ConnectSharePtr;

	GENetConnect(GENetWork* pNetWork, GEDefine::ConnectParam& CP);
	~GENetConnect();


	GE::Int32 			RecvCompletionMsg();


	void 				SendBytes(const void* pHead, GE::Int16 uSize);
	void 				WriteBytes(const void* pHead, GE::Int16 uSize);
	void				AsyncSendBlock();

	BoostSocket&		Socket();

private:
	GENetWork*			m_pNetWork;

	BoostSocket			m_BoostSocket;
};


#endif //MYSERVER_GENETCONNECT_HF
