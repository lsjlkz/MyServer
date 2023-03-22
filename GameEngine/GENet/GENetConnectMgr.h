//
// Created by lsjlkz on 2023/3/1.
//

#ifndef MYSERVER_GENETCONNECTMGR_H
#define MYSERVER_GENETCONNECTMGR_H

#include <queue>
#include "GENetConnect.h"

class GENetConnectMgr {


	typedef GENetConnect			Connect;
	typedef Connect*				ConnectPtr;
	typedef boost::mutex			Mutex;
	typedef Mutex*					MutexPtr;
	typedef std::unordered_map<GE::Uint32, GENetConnect::ConnectSharePtr>			HolderMap;
	typedef std::queue<GE::Uint32>	UIDQueue;

public:
	GENetConnectMgr(GE::Uint32 uMaxConnect);
	~GENetConnectMgr();


	bool			AddConnect(GENetConnect::ConnectSharePtr& spConnect, GE::Uint32& uid);
	bool			DelConnect(GE::Uint32 uid);
	bool			HasConnect(GE::Uint32 uid);
	GENetConnect*	FindConnect(GE::Uint32 uid);

	void			ForceShutdownIllegalConnect_us();			// 处理一下非法连接

	GE::Uint32 		ConnectCnt(){return m_uConnectCnt;};
	GE::Uint32 		FreeCnt(){return m_uMaxSize - m_uConnectCnt;};

private:
	HolderMap 		m_pHolder;
	ConnectPtr*		m_pConnectArr;
	MutexPtr*		m_pConnectMutexArr;

	UIDQueue		m_UIDQueue;

	GE::Uint32		m_uMaxSize;
	GE::Uint32 		m_uConnectCnt;
};


#endif //MYSERVER_GENETCONNECTMGR_H
