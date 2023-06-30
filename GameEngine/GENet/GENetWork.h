// 每一个客户端或者服务端的连接都有一个单独的GENetWork
// 一个logic会有好多好多个GENetWork对象
//
// Created by lsjlkz on 2022/6/2.
//

#pragma once
#ifndef MYSERVER_GENETWORK_H
#define MYSERVER_GENETWORK_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include "GENetConnect.h"
#include "GENetConnectMgr.h"


// 网络层
class GENetWork{
public:
    typedef boost::asio::ip::tcp::acceptor				tdBoostAcceptor;
    typedef boost::asio::ip::address_v4					tdBoostAddV4;
    typedef boost::asio::io_context                     tdBoostIOContext;
    typedef boost::thread								tdBoostThread;
    typedef std::vector<tdBoostThread*>					tdBoostThreads;
    typedef std::vector<std::string>					tdStringVector;
	typedef boost::asio::ip::tcp::endpoint 				tdBoostEndPoint;

	GENetWork(GE::Int32 MaxConnect, GE::Int32 Thread);
	GE::Int32 Listen_MT(GE::Int32 uListenPort);

	void					BoostAsioRun();

	void					AsyncAccept_NT();
	void					HandleAccept_NT(GENetConnect::ConnectSharePtr s_pConnect, const boost::system::error_code& error );

	void					Start_MT();

	void					Stop_MT();

	bool					Connect_MT(const char* sIP, GE::Uint32 uPort, GE::Uint32& uSessionID, GE::Uint16 uWho, void* pBindPoint, GEDefine::ConnectParam* pCP);
	bool					HasConnect(GE::Uint32 uSessionId);
	void					SendBytes_MT(GE::Uint32 uSessionID, void* pHead, GE::Uint32 uSize);		// 不加锁，只能主线程发送，线程不安全
	void					SendBytes(GE::Uint32 uSessionID, void* pHead, GE::Uint32 uSize);		// 加锁，可以多线程发送
	bool 					MoveToNextMsg();
	GENetConnect*			CurConnect(){return this->m_pIterConnect;}	// 返回当前的连接
	MsgBase*				CurMsg(){return this->m_pCurMsg;}	// 返回当前的连接

	tdBoostIOContext&		IOS();


	bool isRun(){return m_bIsRun;}

private:

	GENetConnectMgr		m_ConnectMgr;
	boost::mutex		m_ConnectMutex;

	bool 				m_bIsRun;
	bool				m_bIsStop;
    tdBoostIOContext    m_ioContext;
    tdBoostAcceptor*    m_pAcceptor;
	tdBoostThreads		m_pNetWorkThreads;

	GENetConnect*		m_pIterConnect;			// 迭代当前的连接
	MsgBase*			m_pCurMsg;

};


#endif //MYSERVER_GENETWORK_H
