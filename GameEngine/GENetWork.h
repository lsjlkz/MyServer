//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_GENETWORK_H
#define MYSERVER_GENETWORK_H

#include "iostream"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

// 网络层
class GENetWork{
public:
    typedef boost::asio::ip::tcp::acceptor				tdBoostAcceptor;
    typedef boost::asio::io_service                     tdBoostIOServer;
    typedef boost::thread								tdBoostThread;
    typedef std::vector<tdBoostThread*>					tdBoostThreads;
    typedef std::vector<std::string>					tdStringVector;

	GENetWork(int MaxConnect, int Thread);
	int ListenPort(int Port);


	bool isRun();

private:
    tdBoostIOServer     ioServer;
    tdBoostAcceptor*    accpetor;

};


#endif //MYSERVER_GENETWORK_H
