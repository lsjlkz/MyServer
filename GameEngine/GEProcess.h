//
// Created by lsjlkz on 2023/3/6.
//

// 进程
#ifndef MYSERVER_GEPROCESS_H
#define MYSERVER_GEPROCESS_H

#include "GESingleton.h"
#include "GEDefine.h"



class GEProcess: public GESingleton<GEProcess>{
public:
	GEProcess();

	GEDefine::ConnectParam			DefualConnectParam;

	void SetProcessName(const char *processName, GE::Uint16 size);
	void SetWhoClient(GE::Uint8 ui8);
	void SetWhoGateway(GE::Uint8 ui8);

	void SetClientRedirect(GE::Uint32 RedirectType, GE::Uint32 SessionID);


	const char* GetProcessName(){return m_sProcessName;}
	const GE::Uint32 GetClientRedirect(GE::Uint32 RedirectType);


private:
	char*						m_sProcessName;


public:
	GE::Uint8 					uWhoClient;
	GE::Uint8 					uWhoGateway;

	GE::Uint32 					ClientRedirectArray[CLIENT_REDIRECT_MAX];
};


#endif //MYSERVER_GEPROCESS_H
