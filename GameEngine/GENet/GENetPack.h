//
// Created by lsjlkz on 2022/6/15.
//

#ifndef MYSERVER_GENETPACK_H
#define MYSERVER_GENETPACK_H

#include <vector>
#include <list>
#include "GEDefine.h"
#include "Head.h"

#define IntFlag					-98
#define LongFlag				-99
#define NoneFlag				-100
#define TrueFlag				-101
#define FalseFlag				-102
#define TableFlag				-103
#define StringFlag				-106

#define MSG_MAX_SIZE			256		// 最大char长度
#define MAX_STACK_DEEP			30			// 最大递归层数

// 网络消息的基本长度
#define MSG_BASE_SIZE	4

#define ASSERT_LUA_TOP(L, top, i)	\
	if (top + i != lua_gettop(L)) { std::abort(); };

typedef std::list<char*>			BufPool;
typedef std::list<char*>			BufQueue;



// 这个模块即将会很庞大，貌似
class PackMessage:public SingleTon<PackMessage>{

public:
	BufPool*		bigMsgPool;
	BufQueue*		bigMsgQueue;		// 打包的流队列
	GE::Int32				msgSize;
	char*			curBufHead;			// 当前打包的缓冲区的头
	GE::Int32 			curBufOffset;		// 当前打包的缓冲区偏移
	GE::Int32				curBufEmpty;		// 当前打包的缓冲区剩余空间
	GE::Int32				curStackDeep;		// 栈深度，防止递归自身
public:
	PackMessage();
	~PackMessage();

	void			ClearCache();

	bool 			PackMsgType(GE::Int32 msgType);

	bool			PackType(GE::Int32 t);

	bool 			PackCharInt(GE::Int32 c);
	bool 			PackChar(char c);

	bool			PackInt(GE::Int32 i);
	GE::Int32&		PackIntRef();
	bool			PackIntObj(GE::Int32 i);

	bool			PackLong(long l);
	bool			PackLongObj(long l);

	bool			PackBool(bool b);
	bool			PackBoolObj(bool b);

	bool 			PackString(const char* s, GE::Int32 size);
	bool 			PackStringObj(const char *s, GE::Int32 size);

	bool 			PackLuaObj(lua_State* L);
	void 			PackLuaHelp(lua_State* L, GE::Int32 index);

	GE::Int32		PackSize() const{return msgSize;}
	void 			PackByte(const void* pHead, GE::Int32 size);
	BufQueue*		BigMsgQueue() const{return bigMsgQueue;}
	void			NewBuf();			// 新建一个缓冲区
	char*			MsgIter();


};

class UnpackMessage{
public:
	UnpackMessage(void* pHead);
	UnpackMessage(void* pHead, GE::Int32 nSize);
public:
	bool 			UnpackType(GE::Int32&flag);
	bool 			UnpackMsgType(GE::Int32& msgType);
	bool			UnpackInt(GE::Int32& i);
	bool			UnpackLong(long& l);
	bool			UnpackBool(bool& b);
	bool 			UnpackString(char* s, GE::Int32 size);
	bool 			UnpackLuaObj(lua_State* L);
	bool 			UnpackLuaObjHelp(lua_State* L);

	void 			SetSize(GE::Int32 size){msgSize = size;leftSize = msgSize - curBufOffset;}
	GE::Int32		MsgSize(){return msgSize;}

private:
	GE::Int32 		curBufOffset;		// 当前打包的缓冲区偏移
	char*			curBufHead;			// 缓冲区的头
	GE::Int32		msgSize;			// 消息大小
	GE::Int32		leftSize;			// 剩余大小
	bool 			isOK;				// 是否有报错

};


#endif //MYSERVER_GENETPACK_H
