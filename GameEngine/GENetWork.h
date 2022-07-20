//
// Created by lsjlkz on 2022/6/2.
//

#ifndef MYSERVER_GENETWORK_H
#define MYSERVER_GENETWORK_H

#include "iostream"

// 网络层
class GENetWork{
public:
	GENetWork(int MaxConnect, int Thread);
	int ListenPort(int Port);

	bool isRun();
};


#endif //MYSERVER_GENETWORK_H
