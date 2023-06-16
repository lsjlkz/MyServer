//
// Created by lsjlkz on 2022/6/8.
//

#ifndef MYSERVER_GSDEFINE_H
#define MYSERVER_GSDEFINE_H


#include "GENet/GENetMessage.h"

enum CMsgType{
	CMsg_None = 0,					//占位
	CMsg_Ping,						//心跳包
	CMsg_Pong,						//回复包
	CMsg_Who,						//表明身份
};


MSG_BEGIN(MSG_Who, CMsg_Who)
	GE::Uint32 uWho;
};

GE_STATIC_ASSERT(sizeof(MSG_Who) == 12);

#endif //MYSERVER_GSDEFINE_H
