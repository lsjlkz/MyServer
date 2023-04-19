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

	const char* GetProcessName(){return m_sProcessName;}


private:
	char*						m_sProcessName;

};


#endif //MYSERVER_GEPROCESS_H
