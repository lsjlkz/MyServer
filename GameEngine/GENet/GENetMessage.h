//
// Created by lsjlkz on 2023/3/21.
//

#ifndef MYSERVER_GENETMESSAGE_H
#define MYSERVER_GENETMESSAGE_H

#include "GEDefine.h"

// 网络消息最大长度
#define MSG_MAX_SIZE	MAX_UINT16
// 网络消息的基本长度
#define MSG_BASE_SIZE	4

// 网络消息宏
#define MSG_BEGIN(className, msgType) \
class className:public MsgBase        \
{                                     \
public:                               \
	className():MsgBase(sizeof(className), msgType){}

union tdMsgRedirect{
	tdMsgRedirect(){}

	struct{
		// 发送的次数，可以用来校验
		GE::Uint16 uClientMsgCount;
		// 重定向标记
		// 客户端的消息可能会经过网关转发到其他进程
		GE::Uint16 uClientRedirect;
	};

	// 转发到其他进程的话，要附带一个sessionid
	GE::Uint32 uClientSessionID;

	struct {
		// 重定向信息
		GE::Uint16 uServerRedirectCount1;
		GE::Uint16 uServerRedirectCount2;
	};

};

GE_STATIC_ASSERT(sizeof(tdMsgRedirect)==4);

// C++中的属性在内存中的位置是按照声明的顺序分配的
// 序列化之后只要强制声明为同一个对象对象，就可以获取到原来的属性
// 消息体
class MsgBase{
public:
	MsgBase():m_uSize(sizeof(MsgBase)), m_uType(0){}
	MsgBase(GE::Uint16 uType):m_uSize(sizeof(MsgBase)), m_uType(uType){}
	MsgBase(GE::Uint16 uSize, GE::Uint16 uType):m_uSize(uSize), m_uType(uType){}

	GE::Uint16 				Size() const {return m_uSize;}
	void 					AddSize(GE::Uint16 uSize){m_uSize += uSize;}
	GE::Uint16 				Type() const {return m_uType;}
	void 					Type(GE::Uint16 uType){m_uType=uType;}

	tdMsgRedirect& 			Redirect() {return mRedirect;}
	const tdMsgRedirect& 	Redirect() const{return mRedirect;}
	void*					Body(){return (char*)this + sizeof(MsgBase);}
	GE::Uint16 				BodySize() const {return m_uSize - sizeof(MsgBase);}

	void					Align();					// 4字节对齐
	void 					AddRedirect1_us(GE::Uint32 uSessionId);
	void 					AddRedirect2_us(GE::Uint32 uSessionId);
	void					ClearRedirect1();
	void					ClearRedirect2();

	GE::Uint32 *			GetRedirectPtr();			// 获取重定向信息的指针


protected:
	GE::Uint16 m_uType;						// 消息体的类型
	GE::Uint16 m_uSize;						// 消息体的大小
	tdMsgRedirect mRedirect;				// 重定向的数据
};


class GENetMessage {

};


#endif //MYSERVER_GENETMESSAGE_H
