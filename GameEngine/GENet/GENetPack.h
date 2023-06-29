//
// Created by lsjlkz on 2022/6/15.
//

#ifndef MYSERVER_GENETPACK_H
#define MYSERVER_GENETPACK_H

#include <vector>
#include <queue>
#include "GENetMessage.h"
#include "GESingleton.h"
#include "../../GameServer/Lua/LuaInclude.h"

#define IntFlag					-98
#define LongFlag				-99
#define NoneFlag				-100
#define TrueFlag				-101
#define FalseFlag				-102
#define TableFlag				-103
#define StringFlag				-106


#define MAX_STACK_DEEP			30			// 最大递归层数


#define ASSERT_LUA_TOP(L, top, i)	\
	if (top + i != lua_gettop(L)) { std::abort(); };



typedef std::vector<char*>			BufPool;
typedef std::queue<char*>			BufQueue;


// 这个模块即将会很庞大，貌似
class PackMessage:public GESingleton<PackMessage>{
	GE_DISALLOW_COPY_AND_ASSIGN(PackMessage);

public:
	PackMessage();
	~PackMessage();

	void			ClearCache();

	bool 			PackMsgType(GE::Uint16 msgType);

	bool			PackType(GE::Int8 t);

	GE::Uint8* 		PackU8Ref();
	GE::Int8*		PackI8Ref();
	GE::Uint16* 	PackU16Ref();
	GE::Uint32*		PackU32Ref();
	GE::Int32*		PackI32Ref();
	bool 			PackU8(GE::Uint8 i);
	bool 			PackI8(GE::Int8 i);
	bool 			PackU16(GE::Uint16 i);
	bool			PackI32(GE::Int32 i);
	bool			PackI32Obj(GE::Int32 i);

	bool			PackI64(GE::Int64 l);
	bool			PackI64Obj(GE::Int64 l);

	bool			PackBool(bool b);
	bool			PackBoolObj(bool b);

	bool 			PackString(const char* s, GE::Uint32 size);
	bool 			PackStringObj(const char *s, GE::Uint16 size);

	bool			PackMsg(MsgBase* pMsg);								// 打包一个msg

	bool 			PackLuaObj(lua_State* L);							// 打包一个lua的对象
	void 			PackLuaHelp(lua_State* L, GE::Int32 index);

	void			Align();											// 4字节对齐

	GE::Uint32		PackSize() const{return m_uSize;}
	void 			PackByte(const void* pHead, GE::Int32 size);
	BufQueue*		BigMsgQueue() const{return m_pBigMsgQueue;}
	void			NewBuf();			// 新建一个缓冲区
	void*			HeadPtr(){return this->m_pCurBufHead;}

	MsgBase*		Msg(){return reinterpret_cast<MsgBase*>(this->HeadPtr());}

	GE::Uint32 		PackFence(){return this->m_pCurPackFence;}


private:
	bool					m_bIsOk;				// 是否有错误
	BufPool*				m_pBigMsgPool;
	BufQueue*				m_pBigMsgQueue;			// 打包的流队列
	GE::Uint32				m_uSize;				// 当前打包所有缓冲区空间
	char*					m_pCurBufHead;			// 当前打包的缓冲区的头
	GE::Uint32 				m_pCurPackFence;		// 当前打包的缓冲区偏移
	GE::Uint32				m_uCurBufEmpty;			// 当前打包的缓冲区剩余空间
	GE::Uint32				m_uCurStackDeep;		// 栈深度，防止递归自身
};

class UnpackMessage{
	GE_DISALLOW_COPY_AND_ASSIGN(UnpackMessage);
public:
	UnpackMessage(void* pHead);
	UnpackMessage(void* pHead, GE::Int32 nSize);
public:
	bool 			UnpackType(GE::Int8&flag);
	bool 			UnpackMsgType(GE::Uint16& msgType);
	bool 			UnpackU8(GE::Uint8& u8);
	bool 			UnpackI8(GE::Int8& i8);
	bool 			UnpackU16(GE::Uint16& u16);
	bool 			UnpackI16(GE::Int16& i16);
	bool 			UnpackU32(GE::Uint32& u32);
	bool			UnpackI32(GE::Int32& i32);
	bool 			UnpackU64(GE::Uint64& u64);
	bool			UnpackI64(GE::Int64& i64);
	bool			UnpackBool(bool& b);
	bool 			UnpackString(char* s, GE::Uint32 size);
	bool 			UnpackLuaObj(lua_State* L);
	bool 			UnpackLuaObjHelp(lua_State* L);

	void 			SetSize(GE::Uint32 uSize){ m_uSize = uSize;m_nSurplusSize = m_uSize - curBufOffset;}
	GE::Int32		MsgSize(){return m_uSize;}

private:
	GE::Int32 		curBufOffset;		// 当前打包的缓冲区偏移
	char*			curBufHead;			// 缓冲区的头
	GE::Int32		m_uSize;			// 消息大小
	GE::Int32		m_nSurplusSize;			// 剩余大小
	bool 			m_bIsOK;				// 是否有报错

};


#endif //MYSERVER_GENETPACK_H
