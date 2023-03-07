//
// Created by lsjlkz on 2022/6/2.
//

#include "GENetWork.h"

GENetWork::GENetWork(GE::Int32 MaxConnect, GE::Int32 Thread) {
	// TODO 网络层研究一下
	for(GE::Int32 i = 0; i < Thread; ++i){
		this->m_pNetWorkThreads.push_back(NULL);
	}
	std::cout << "GENetWork" << MaxConnect << ";" << Thread << std::endl;
}

GE::Int32 GENetWork::Listen_MT(GE::Int32 uListenPort) {
	// 监听端口
    if(this->m_pAcceptor == NULL){
        try{
#ifdef WIN
			// 这里开启一个
            this->m_pAcceptor = new tdBoostAcceptor(m_ioServer, tdBoostEndPoint(tdBoostAddV4::from_string("0.0.0.0"), uListenPort),
													false);
#elif LINUX
			this->m_pAcceptor = new tdBoostAcceptor(m_ioServer, tdBoostEndPoint(tdBoostAddV4::from_string("0.0.0.0"), uListenPort),
													true);
#endif
			this->AsyncAccept_NT();
        }
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "listen port error " << uListenPort << std::endl;
			return 0;
		}
		return 1;
    }
	return 0;
}
void GENetWork::Start_MT(){
	// 这里开了多个线程接收io
	for(GE::Int32 i = 0; i < this->m_pNetWorkThreads.size(); ++i){
		this->m_pNetWorkThreads.at(i) = new tdBoostThread (boost::bind(&GENetWork::BoostAsioRun, this));
	}
	this->m_bIsRun = true;
}

void GENetWork::Stop_MT(){
	this->m_bIsRun = false;
	this->m_bIsStop = true;



}

GENetWork::tdBoostIOServer& GENetWork::IOS() {
	return this->m_ioServer;
}

void GENetWork::BoostAsioRun(){
	std::cout << "BoostAsioRun" << std::endl;
	this->m_ioServer.run();
}

void GENetWork::AsyncAccept_NT(){
	if(NULL == this->m_pAcceptor){
		return;
	}
	GENetConnect::ConnectSharePtr spConnect = GENetConnect::ConnectSharePtr(new GENetConnect(this, GEProcess::Instance()->DefualConnectParam));
	this->m_pAcceptor->async_accept(spConnect->Socket(),
									boost::bind(&GENetWork::HandleAccept_NT, this, spConnect, boost::asio::placeholders::error));
}

void write_handle(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
	std::cout << "write_handle" << std::endl;
}

void GENetWork::HandleAccept_NT(GENetConnect::ConnectSharePtr s_pConnect, const boost::system::error_code &error) {
	std::cout << "HandleAccept_NT" << std::endl;
	std::string data = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
	boost::asio::async_write(s_pConnect->Socket(), boost::asio::buffer(data), write_handle);

}

