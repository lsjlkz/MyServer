//
// Created by lsjlkz on 2023/3/6.
//

// 进程
#ifndef MYSERVER_GEPROCESS_H
#define MYSERVER_GEPROCESS_H

#include "Head.h"
#include "SingleTon.h"
#include "GEDefine.h"

class GEProcess: public SingleTon<GEProcess>{
public:
	GEDefine::ConnectParam			DefualConnectParam;
};


#endif //MYSERVER_GEPROCESS_H
