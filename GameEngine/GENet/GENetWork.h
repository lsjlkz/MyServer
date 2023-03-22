//
// Created by lsjlkz on 2022/6/2.
//

#pragma once
#ifndef MYSERVER_GENETWORK_H
#define MYSERVER_GENETWORK_H

#include "Head.h"
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

	void BoostAsioRun();

	void AsyncAccept_NT();
	void HandleAccept_NT(GENetConnect::ConnectSharePtr s_pConnect, const boost::system::error_code& error );

	void Start_MT();

	void Stop_MT();

	tdBoostIOServer&		IOS();


	bool isRun(){return m_bIsRun;}

private:

	GENetConnectMgr		m_ConnectMgr;
	boost::mutex		m_ConnectMutex;				// 防止多线程同时修改Mgr

	bool 				m_bIsRun;
	bool				m_bIsStop;
    tdBoostIOServer     m_ioServer;
    tdBoostAcceptor*    m_pAcceptor;
	tdBoostThreads		m_pNetWorkThreads;

};


#endif //MYSERVER_GENETWORK_H
