//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_GESYSTEMTIME_H
#define MYSERVER_GESYSTEMTIME_H

#include "GESingleton.h"
#include "GEDefine.h"

class GESystemTime:public GESingleton<GESystemTime> {
public:
	static GE::Uint64 GetSystemMSeconds();
};


#endif //MYSERVER_GESYSTEMTIME_H
