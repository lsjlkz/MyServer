//
// Created by lsjlkz on 2022/6/8.
//

#ifndef MYSERVER_GSDEFINE_H
#define MYSERVER_GSDEFINE_H


#include "GENet/GENetMessage.h"


enum EndPoindType {
	// 身份
	EndPoint_None = 0,				//占位
	EndPoint_GM,					//GM工具（主动）
	EndPoint_GM_,					//GM工具（被动）
	EndPoint_IO,					//IO工具（主动）
	EndPoint_IO_,					//IO工具（被动）
	EndPoint_ClientGateway,			//客户端（主动）
	EndPoint_ClientGateway_,		//客户端（被动）
	EndPoint_GatewayWorld,			//网关连世界（主动）
	EndPoint_GatewayWorld_,			//网关连世界（被动）
	EndPoint_GatewayLogic,			//网关连逻辑（主动）
	EndPoint_GatewayLogic_,			//网关连逻辑（被动）
	EndPoint_GatewayZone,			//网关连大陆（主动）
	EndPoint_GatewayZone_,			//网关连大陆（被动）
	EndPoint_LogicWorld,			//逻辑连世界（主动）
	EndPoint_LogicWorld_,			//逻辑连世界（被动）
	EndPoint_LogicZone,				//逻辑连大陆（主动）
	EndPoint_LogicZone_,			//逻辑连大陆（被动）
	EndPoint_LogicServerDB,			//逻辑连逻辑服数据（主动）
	EndPoint_LogicServerDB_,		//逻辑连逻辑服数据（被动）
	EndPoint_LogicPlayerDB,			//逻辑连角色数据（主动）
	EndPoint_LogicPlayerDB_,		//逻辑连角色数据（被动）
	EndPoint_ZoneWorld,				//大陆连世界（主动）
	EndPoint_ZoneWorld_,			//大陆连世界（被动）
};

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
