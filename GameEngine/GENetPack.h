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
	int				msgSize;
	char*			curBufHead;			// 当前打包的缓冲区的头
	int 			curBufOffset;		// 当前打包的缓冲区偏移
	int				curBufEmpty;		// 当前打包的缓冲区剩余空间
	int				curStackDeep;		// 栈深度，防止递归自身
public:
	PackMessage();
	~PackMessage();

	void			ClearCache();

	bool 			PackMsgType(int msgType);

	bool			PackType(int t);

	bool 			PackCharInt(int c);
	bool 			PackChar(char c);

	bool			PackInt(int i);
	int&			PackIntRef();
	bool			PackIntObj(int i);

	bool			PackLong(long l);
	bool			PackLongObj(long l);

	bool			PackBool(bool b);
	bool			PackBoolObj(bool b);

	bool 			PackString(const char* s, int size);
	bool 			PackStringObj(const char *s, int size);

	bool 			PackLuaObj(lua_State* L);
	void 			PackLuaHelp(lua_State* L, int index);

	int				PackSize() const{return msgSize;}
	void 			PackByte(const void* pHead, int size);
	BufQueue*		BigMsgQueue() const{return bigMsgQueue;}
	void			NewBuf();			// 新建一个缓冲区
	char*			MsgIter();


};

class UnpackMessage{
public:
	UnpackMessage(void* pHead);
	UnpackMessage(void* pHead, int nSize);
public:
	bool 			UnpackType(int&flag);
	bool 			UnpackMsgType(int& msgType);
	bool			UnpackInt(int& i);
	bool			UnpackLong(long& l);
	bool			UnpackBool(bool& b);
	bool 			UnpackString(char* s, int size);
	bool 			UnpackLuaObj(lua_State* L);
	bool 			UnpackLuaObjHelp(lua_State* L);

	void 			SetSize(int size){msgSize = size;leftSize = msgSize - curBufOffset;}
	int				MsgSize(){return msgSize;}

private:
	int 			curBufOffset;		// 当前打包的缓冲区偏移
	char*			curBufHead;			// 缓冲区的头
	int				msgSize;			// 消息大小
	int				leftSize;			// 剩余大小
	bool 			isOK;				// 是否有报错

};


#endif //MYSERVER_GENETPACK_H
