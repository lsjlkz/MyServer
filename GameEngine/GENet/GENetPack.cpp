//
// Created by lsjlkz on 2022/6/15.
//

#include "GENetPack.h"
#include "GELog.h"

#define MAX_BUF_SIZE 65535
typedef struct {
	size_t write_size;
	size_t read_size;
	char buf[MAX_BUF_SIZE];
} LuaPackBufRef;


#define UnpackTypeMsg(type, name) \
if(m_uSize != 0 && m_nSurplusSize < sizeof(type)){\
this->m_bIsOK = false;\
return false;\
}\
name = static_cast<type>(*((type*)curBufHead));\
curBufHead += sizeof(type);\
m_nSurplusSize -= sizeof(type);\
return true;


PackMessage::PackMessage() {
	m_pBigMsgPool = new BufPool();
	m_pBigMsgQueue = new BufQueue();
	this->NewBuf();
}

PackMessage::~PackMessage() {
	// 清空池和队列
	while (!m_pBigMsgPool->empty()) {
		char *head = m_pBigMsgPool->back();
		m_pBigMsgPool->pop_back();
		delete head;
	}
	delete m_pBigMsgPool;
	while (!m_pBigMsgQueue->empty()) {
		char *head = m_pBigMsgQueue->front();
		m_pBigMsgQueue->pop();
		delete head;
	}
	delete m_pBigMsgQueue;
}

void PackMessage::NewBuf() {
	// 新建一个缓冲区
	if (m_pBigMsgPool->empty()) {
		m_pCurBufHead = new char[MSG_MAX_SIZE];
	} else {
		// 如果池有的话，就直接拿池的
		this->m_pCurBufHead = m_pBigMsgPool->back();
		m_pBigMsgPool->pop_back();
	}
	// 放到队列中
	this->m_pBigMsgQueue->push(this->m_pCurBufHead);
	this->m_pCurPackFence = 0;
	this->m_uSize = 0;
	this->m_uCurBufEmpty = MSG_MAX_SIZE;
}

bool PackMessage::PackType(GE::Int8 t) {
	this->PackI8(t);
	return false;
}

bool PackMessage::PackU8(GE::Uint8 c) {
	this->PackByte(&c, sizeof(GE::Uint8));
	return true;
}

bool PackMessage::PackI8(GE::Int8 c) {
	this->PackByte(&c, sizeof(GE::Int8));
	return true;
}


bool PackMessage::PackU16(GE::Uint16 i) {
	this->PackType(IntFlag);
	this->PackByte(&i, sizeof(GE::Uint16));
	return true;
}

bool PackMessage::PackI32(GE::Int32 i) {
	this->PackByte(&i, sizeof(GE::Int32));
	return true;
}

bool PackMessage::PackI32Obj(GE::Int32 i) {
	this->PackType(IntFlag);
	this->PackI32(i);
	return true;
}

bool PackMessage::PackI64(GE::Int64 l) {
	this->PackByte(&l, sizeof(GE::Int64));
	return true;
}

bool PackMessage::PackI64Obj(GE::Int64 l) {
	this->PackType(LongFlag);
	this->PackI64(l);
	return true;
}

bool PackMessage::PackString(const char *s, GE::Uint32 size) {
	this->PackByte(s, size);
	return true;
}

bool PackMessage::PackStringObj(const char *s, GE::Uint16 size) {
	this->PackType(StringFlag);
	this->PackU16(size);
	this->PackString(s, size);
	return true;
}


void PackMessage::PackByte(const void *pHead, GE::Int32 size) {
	if (this->m_uCurBufEmpty == 0) {
		// 无空间了，重新申请一个
		this->NewBuf();
	}
	if (size <= this->m_uCurBufEmpty) {
		// 还够用，不用重新申请
		memcpy(this->m_pCurBufHead + this->m_pCurPackFence, pHead, size);
		this->m_pCurPackFence += size;
		this->m_uSize += size;
		this->m_uCurBufEmpty -= size;
		return;
	}
	// 不够用了，重新申请一个
	while (size > MSG_MAX_SIZE) {
		// 放入空余的位置
		memcpy(this->m_pCurBufHead, pHead, this->m_uCurBufEmpty);
		pHead = (char *) pHead + this->m_uCurBufEmpty;
		this->m_uSize += this->m_uCurBufEmpty;
		size -= this->m_uCurBufEmpty;
		this->NewBuf();
	}
	memcpy(this->m_pCurBufHead, pHead, size);
	this->m_pCurPackFence += size;
	this->m_uSize += size;
	this->m_uCurBufEmpty -= size;
}

bool PackMessage::PackLuaObj(lua_State *L) {
	if (GE_IS_POINT_NULL(this->m_pBigMsgQueue)) {
		return false;
	}
	GE::Int32 top = lua_gettop(L);
	if (top < 2) {
		GELog::Instance()->Log("lua object empty");
		return false;
	}

	// 消息类型
	GE::Int16 msg_type = (GE::Int16) lua_tointeger(L, -2);
	this->PackMsgType(msg_type);

	// 消息长度
	GE::Uint16 *size = this->PackU16Ref();
	void **pHead = new void *();

	// 把消息打包到buf中
	GE::Int32 ret = lua_pack_top_to_buf(L, pHead);
	if (!ret) {
		GELog::Instance()->Log("error lua_pack_top_to_buf");
		return false;
	}

	// 重定向占位
	GE::Uint32 *redirect = this->PackU32Ref();
	*redirect = 0;

	// 把buf拷贝到缓冲区中
	LuaPackBufRef *io = reinterpret_cast<LuaPackBufRef *>(*pHead);
	this->PackByte((void *) io->buf, io->write_size);

	// size是引用，这里改变size
	*size = io->write_size + sizeof(MsgBase);
	return true;
}

//void PackMessage::PackLuaHelp(lua_State *L, GE::Int32 index) {
//	this->m_uCurStackDeep++;        // 增加递归深度
//	GE::Int32 top = lua_gettop(L);
//	GE::Int32 lt = lua_type(L, index);
//	if (lt == LUA_TNIL) {
//		// nil
//		this->PackType(NoneFlag);
//	} else if (lt == LUA_TBOOLEAN) {
//		// bool
//		GE::Int32 b = lua_toboolean(L, index);
//		if (b) {
//			this->PackType(TrueFlag);
//		} else {
//			this->PackType(FalseFlag);
//		}
//	} else if (lt == LUA_TNUMBER) {
//		GE::Int64 i = lua_tointeger(L, index);
//		this->PackI64Obj(i);
//	} else if (lt == LUA_TSTRING) {
//		size_t size = 0;
//		const char *s = lua_tolstring(L, index, &size);
//		// TODO 越界
//		this->PackStringObj(s, static_cast<GE::Uint16>(size));
//	} else if (lt == LUA_TTABLE) {
//		this->PackType(TableFlag);
//		GE::Uint8 *tableSize = this->PackU8Ref();
//		GE::Uint8 size = 0;
//		// 也就是把-1这些索引转为正值的索引，如长度为n，索引-2应为倒数第二个，也就是转为n-1
//		GE::Int32 table_index = lua_absindex(L, index);
//		// lua_checkstack检测栈上是否有足够的空间，如下是检测top上是否还有2个位置，不够的话会自动扩容
//		lua_checkstack(L, top + 2);
//		lua_pushnil(L);
//		while (lua_next(L, table_index)) {
//			size++;
//			// key
//			this->PackLuaHelp(L, -2);
//			// value
//			this->PackLuaHelp(L, -1);
//
//			lua_pop(L, 1);
//		}
//		*tableSize = size;
//	} else {
//
//		GELog::Instance()->Log("not support lua object");
//	}
//	ASSERT_LUA_TOP(L, top, 0);
//	--(this->m_uCurStackDeep);
//}

GE::Uint8 *PackMessage::PackU8Ref() {
	GE::Uint8 *r = (GE::Uint8 *) (this->m_pCurBufHead + this->m_pCurPackFence);
	// 把长度打包进去，最后再改
	this->PackByte(r, sizeof(GE::Uint8));
	return r;
}

GE::Int8 *PackMessage::PackI8Ref() {
	GE::Int8 *r = (GE::Int8 *) (this->m_pCurBufHead + this->m_pCurPackFence);
	// 把长度打包进去，最后再改
	this->PackByte(r, sizeof(GE::Int8));
	return r;
}

GE::Uint16 *PackMessage::PackU16Ref() {
	GE::Uint16 *r = (GE::Uint16 *) (this->m_pCurBufHead + this->m_pCurPackFence);
	// 把长度打包进去，最后再改
	this->PackByte(r, sizeof(GE::Uint16));
	return r;
}

GE::Uint32 *PackMessage::PackU32Ref() {
	GE::Uint32 *r = (GE::Uint32 *) (this->m_pCurBufHead + this->m_pCurPackFence);
	// 把长度打包进去，最后再改
	this->PackByte(r, sizeof(GE::Uint32));
	return r;
}

GE::Int32 *PackMessage::PackI32Ref() {
	GE::Int32 *r = (GE::Int32 *) (this->m_pCurBufHead + this->m_pCurPackFence);
	// 把长度打包进去，最后再改
	this->PackByte(r, sizeof(GE::Int32));
	return r;
}

void PackMessage::ClearCache() {
	// 清理旧的，然后回池
	while (!this->m_pBigMsgQueue->empty()) {
		m_pBigMsgPool->push_back(this->m_pBigMsgQueue->back());
		this->m_pBigMsgQueue->pop();
	}
	// 拿个新的
	this->NewBuf();
}

void PackMessage::Align() {
	if (!(this->m_uSize % MSG_BASE_SIZE)) {
		return;
	}
	this->m_uSize += (MSG_BASE_SIZE - this->m_uSize % MSG_BASE_SIZE);
}


bool PackMessage::PackMsgType(GE::Uint16 msgType) {
	this->PackByte(&msgType, sizeof(GE::Uint16));
	return true;
}

bool PackMessage::PackMsg(MsgBase *pMsg) {
	return false;
}


bool UnpackMessage::UnpackMsgType(GE::Uint16 &msgType) {
	return this->UnpackU16(msgType);
}

bool UnpackMessage::UnpackType(GE::Int8 &flag) {
	UnpackTypeMsg(GE::Int8, flag);
}

bool UnpackMessage::UnpackU8(GE::Uint8 &u8) {
	UnpackTypeMsg(GE::Uint8, u8);
}

bool UnpackMessage::UnpackI8(GE::Int8 &i8) {
	UnpackTypeMsg(GE::Int8, i8);
}

bool UnpackMessage::UnpackU16(GE::Uint16 &u16) {
	UnpackTypeMsg(GE::Uint16, u16);
}

bool UnpackMessage::UnpackI16(GE::Int16 &i16) {
	UnpackTypeMsg(GE::Int16, i16);
}

bool UnpackMessage::UnpackU32(GE::Uint32 &i32) {
	UnpackTypeMsg(GE::Uint32, i32);
}

bool UnpackMessage::UnpackI32(GE::Int32 &i32) {
	UnpackTypeMsg(GE::Int32, i32);
}


bool UnpackMessage::UnpackU64(GE::Uint64 &u64) {
	UnpackTypeMsg(GE::Uint64, u64);
}

bool UnpackMessage::UnpackI64(GE::Int64 &i64) {
	UnpackTypeMsg(GE::Int64, i64);
}

bool UnpackMessage::UnpackBool(bool &b) {
	if (m_nSurplusSize < sizeof(GE::Int32)) {
		this->m_bIsOK = false;
		return false;
	}
	GE::Int32 bFlag = static_cast<GE::Int32>(*((GE::Int32 *) curBufHead));
	if (bFlag == TrueFlag) {
		b = true;
	} else {
		b = false;
	}
	return true;
}

bool UnpackMessage::UnpackString(char *s, GE::Uint32 size) {
	if (m_nSurplusSize < size) {
		this->m_bIsOK = false;
		return false;
	}
	memcpy(s, curBufHead, size);
	curBufHead += size;
	return true;
}

UnpackMessage::UnpackMessage(void *pHead) {
	this->m_bIsOK = false;
	m_uSize = 0;
	m_nSurplusSize = 0;
	curBufHead = static_cast<char *>(pHead);
	curBufOffset = 0;
}

UnpackMessage::UnpackMessage(void *pHead, GE::Int32 nSize) {
	this->m_bIsOK = false;
	m_uSize = nSize;
	m_nSurplusSize = m_uSize;
	curBufHead = static_cast<char *>(pHead);
	curBufOffset = 0;
}

bool UnpackMessage::UnpackLuaObj(lua_State *L) {
	GE::Int32 top = lua_gettop(L);
	this->UnpackLuaObjHelp(L);

	ASSERT_LUA_TOP(L, top, 1);
	return this->m_bIsOK;
}

bool UnpackMessage::UnpackLuaObjHelp(lua_State *L) {
	if (!this->m_bIsOK) {
		lua_pushnil(L);
		return false;
	}

	GE::Int32 top = lua_gettop(L);
	lua_checkstack(L, top + 5);
	GE::Int8 flag = 0;
	this->UnpackType(flag);
	switch (flag) {
		case IntFlag: {
			GE::Int32 i = 0;
			if (this->UnpackI32(i)) { lua_pushinteger(L, i); }
			else { lua_pushinteger(L, 0); }
			break;
		}
		case LongFlag: {

			GE::Int64 l = 0;
			if (this->UnpackI64(l)) { lua_pushinteger(L, l); }
			else { lua_pushinteger(L, 0); }
			break;
		}
		case StringFlag: {
			GE::Int32 size = 0;
			this->UnpackI32(size);
			char *str = new char[size];
			if (this->UnpackString(str, size)) { lua_pushlstring(L, str, size); }
			else { lua_pushnil(L); }
			break;
		}
		case NoneFlag: {
			lua_pushnil(L);
			break;
		}
		case TrueFlag: {
			lua_pushboolean(L, 1);
			break;
		}
		case FalseFlag: {
			lua_pushboolean(L, 0);
			break;
		}
		case TableFlag: {
			GE::Uint8 size = 0;
			// table的长度
			this->UnpackU8(size);
//			lua_createtable(L, size, 0);
			lua_newtable(L);
			ASSERT_LUA_TOP(L, top, 1);
			GE::Int32 table_index = lua_gettop(L);
			for (GE::Uint8 idx = 0; idx < size; idx++) {
//				lua_pushinteger(L, idx + 1);
				// key
				this->UnpackLuaObjHelp(L);
				// value
				this->UnpackLuaObjHelp(L);
				lua_settable(L, table_index);
			}
			break;
		}
		default: {
			// 压个nil
			lua_pushnil(L);
			break;
		}
	}
	ASSERT_LUA_TOP(L, top, 1);
	return true;
}