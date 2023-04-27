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
    typedef boost::asio::io_service                     tdBoostIOServer;
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

	bool					HasConnect(GE::Uint32 uSessionId);
	void					SendBytes_MT(GE::Uint32 uSessionID, void* pHead, GE::Uint32 uSize);		// 不加锁，只能主线程发送，线程不安全
	void					SendBytes(GE::Uint32 uSessionID, void* pHead, GE::Uint32 uSize);		// 加锁，可以多线程发送


	tdBoostIOServer&		IOS();


	bool isRun(){return m_bIsRun;}

private:

	GENetConnectMgr		m_ConnectMgr;
	boost::mutex		m_ConnectMutex;

	bool 				m_bIsRun;
	bool				m_bIsStop;
    tdBoostIOServer     m_ioServer;
    tdBoostAcceptor*    m_pAcceptor;
	tdBoostThreads		m_pNetWorkThreads;

};


#endif //MYSERVER_GENETWORK_H
