//
// Created by lsjlkz on 2023/3/1.
//

#ifndef MYSERVER_GENETCONNECTMGR_H
#define MYSERVER_GENETCONNECTMGR_H

#include "GENetConnect.h"

class GENetConnectMgr {


	typedef GENetConnect*			ConnectPtr;
	typedef std::unordered_map<GE::Uint32, GENetConnect::ConnectSharePtr>			HolderMap;
	typedef std::map<GE::Uint32, GE::Uint32>		IndexMap;

public:
	GENetConnectMgr(GE::Uint32 uMaxConnect);
	~GENetConnectMgr();


	bool			AddConnect(GENetConnect::ConnectSharePtr& spConnect, GE::Uint32& uId);
	bool			DelConnect(GE::Uint32 uId);
	bool			HasConnect(GE::Uint32 uId);
	GENetConnect*	FindConnect(GE::Uint32 uId);

	GE::Uint32 		UID2Index(GE::Uint32 uId){
		return uId % m_uMaxSize;
	}

	GE::Uint32 		ConnectCnt(){return m_uConnectCnt;};
	GE::Uint32 		FreeCnt(){return m_uMaxSize - m_uConnectCnt;};

private:
	HolderMap 		m_pHolder;
	ConnectPtr*		m_pConnectArr;
	IndexMap 		m_indexMap;

	GE::Uint32		m_uMaxSize;
	GE::Uint32 		m_uConnectCnt;
};


#endif //MYSERVER_GENETCONNECTMGR_H
