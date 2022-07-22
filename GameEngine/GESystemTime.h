//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_GESYSTEMTIME_H
#define MYSERVER_GESYSTEMTIME_H

#include "SingleTon.h"

class GESystemTime:public SingleTon<GESystemTime> {
public:
	static unsigned long long GetSystemMSeconds();
};


#endif //MYSERVER_GESYSTEMTIME_H
