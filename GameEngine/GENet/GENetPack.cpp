//
// Created by lsjlkz on 2022/6/15.
//

#include "GENetPack.h"


PackMessage::PackMessage() {
	m_pBigMsgPool = new BufPool();
	m_pBigMsgQueue = new BufQueue();
	this->m_uSize = 0;
	this->NewBuf();
}

PackMessage::~PackMessage() {
	// 清空池和队列
	while(!m_pBigMsgPool->empty()){
		char* head = m_pBigMsgPool->back();
		m_pBigMsgPool->pop_back();
		delete head;
	}
	delete m_pBigMsgPool;
	while(!m_pBigMsgQueue->empty()){
		char* head = m_pBigMsgQueue->front();
		m_pBigMsgQueue->pop();
		delete head;
	}
	delete m_pBigMsgQueue;
}

void PackMessage::NewBuf() {
	// 新建一个缓冲区
	if(m_pBigMsgPool->empty()){
		m_pCurBufHead = new char[MSG_MAX_SIZE];
	}else{
		// 如果池有的话，就直接拿池的
		this->m_pCurBufHead = m_pBigMsgPool->back();
		m_pBigMsgPool->pop_back();
	}
	// 放到队列中
	this->m_pBigMsgQueue->push(this->m_pCurBufHead);
	this->m_uCurBufOffset = 0;
	this->m_uCurBufEmpty = MSG_MAX_SIZE;
}

bool PackMessage::PackType(GE::Int32 t) {
	this->PackCharInt(t);
	return false;
}

bool PackMessage::PackCharInt(GE::Int32 c) {
	char cc = static_cast<char>(c);
	this->PackChar(cc);
	return true;
}

bool PackMessage::PackChar(char c) {
	this->PackByte(&c, sizeof(char));
	return true;
}

bool PackMessage::PackInt(GE::Int32 i) {
	this->PackByte(&i, sizeof(GE::Int32));
	return true;
}

bool PackMessage::PackIntObj(GE::Int32 i) {
	this->PackType(IntFlag);
	this->PackInt(i);
	return true;
}

bool PackMessage::PackLong(GE::Int64 l) {
	this->PackByte(&l, sizeof(GE::Int64));
	return true;
}

bool PackMessage::PackLongObj(GE::Int64 l) {
	this->PackType(LongFlag);
	this->PackLong(l);
	return true;
}

bool PackMessage::PackString(const char *s, GE::Uint32 size) {
	this->PackByte(s, size);
	return true;
}

bool PackMessage::PackStringObj(const char *s, GE::Uint32 size) {
	this->PackType(StringFlag);
	this->PackInt(size);
	this->PackString(s, size);
	return true;
}


void PackMessage::PackByte(const void *pHead, GE::Int32 size) {
	if(this->m_uCurBufEmpty == 0){
		// 无空间了，重新申请一个
		this->NewBuf();
	}
	if(size <= this->m_uCurBufEmpty){
		// 还够用，不用重新申请
		memcpy(this->m_pCurBufHead + this->m_uCurBufOffset, pHead, size);
		this->m_uCurBufOffset += size;
		this->m_uSize += size;
		this->m_uCurBufEmpty -= size;
		return;
	}
	char* curP = (char *) pHead;
	// 不够用了，重新申请一个
	while(size > MSG_MAX_SIZE){
		// 放入空余的位置
		memcpy(this->m_pCurBufHead, curP, this->m_uCurBufEmpty);
		curP += this->m_uCurBufEmpty;
		this->m_uSize += this->m_uCurBufEmpty;
		size -= this->m_uCurBufEmpty;
		this->NewBuf();
	}
	memcpy(this->m_pCurBufHead, pHead, size);
	this->m_uCurBufOffset += size;
	this->m_uSize += size;
	this->m_uCurBufEmpty -= size;
}

bool PackMessage::PackLuaObj(lua_State *L) {
	if(GE_IS_POINT_NULL(this->m_pBigMsgQueue)){
		return false;
	}
	GE::Int32 top = lua_gettop(L);
	if(top <= 0){
		GELog::Instance()->Log("lua object empty");
		return false;
	}
	GE::Uint32& size = this->PackIntRef();
	this->m_uCurStackDeep = 0;
	GE::Int32 msg_type = (GE::Int32) lua_tointeger(L, -2);
	this->PackMsgType(msg_type);
	this->PackLuaHelp(L, -1);
	// size是引用，这里改变size
	size = this->PackSize();
	return true;
}

void PackMessage::PackLuaHelp(lua_State *L, GE::Int32 index) {
	this->m_uCurStackDeep++;		// 增加递归深度
	GE::Int32 top = lua_gettop(L);
	GE::Int32 lt = lua_type(L, index);
	if(lt == LUA_TNIL){
		// nil
		this->PackType(NoneFlag);
	}else if(lt == LUA_TBOOLEAN){
		// bool
		GE::Int32 b = lua_toboolean(L, index);
		if(b){
			this->PackType(TrueFlag);
		}else{
			this->PackType(FalseFlag);
		}
	}else if(lt == LUA_TNUMBER){
		GE::Int32 i = lua_tointeger(L, index);
		this->PackIntObj(i);
	}else if(lt == LUA_TSTRING){
		size_t size = 0;
		const char* s = lua_tolstring(L, index, &size);
		// TODO 越界
		this->PackStringObj(s, static_cast<GE::Uint32>(size));
	}else if(lt == LUA_TTABLE){
		this->PackType(TableFlag);
		GE::Uint32& tableSize = this->PackIntRef();
		GE::Uint32 size = 0;
		// 也就是把-1这些索引转为正值的索引，如长度为n，索引-2应为倒数第二个，也就是转为n-1
		GE::Int32 table_index = lua_absindex(L, index);
		// lua_checkstack检测栈上是否有足够的空间，如下是检测top上是否还有2个位置，不够的话会自动扩容
		lua_checkstack(L, top + 2);
		lua_pushnil(L);
		while(lua_next(L, table_index)){
			size++;
			// key
			this->PackLuaHelp(L, -2);
			// value
			this->PackLuaHelp(L, -1);

			lua_pop(L, 1);
		}

		tableSize = size;
	}else{

		GELog::Instance()->Log("not support lua object");
	}
	ASSERT_LUA_TOP(L, top, 0);
	--(this->m_uCurStackDeep);
}

GE::Uint32 &PackMessage::PackIntRef() {
	GE::Uint32*r = (GE::Uint32*)(this->m_pCurBufHead + this->m_uCurBufOffset);
	// 把长度打包进去，最后再改
	this->PackByte(r, sizeof(GE::Uint32));
	return *r;
}

void PackMessage::ClearCache() {
	// 清理旧的，然后回池
	while(!this->m_pBigMsgQueue->empty()){
		m_pBigMsgPool->push_back(this->m_pBigMsgQueue->back());
		this->m_pBigMsgQueue->pop();
	}
	// 拿个新的
	this->NewBuf();
}

void PackMessage::Align() {
	if(!(this->m_uSize % MSG_BASE_SIZE)){
		return;
	}
	this->m_uSize += (MSG_BASE_SIZE - this->m_uSize % MSG_BASE_SIZE);
}


bool PackMessage::PackMsgType(GE::Int32 msgType) {
	return this->PackInt(msgType);
}

bool PackMessage::PackMsg(MsgBase *pMsg) {
	return false;
}


bool UnpackMessage::UnpackMsgType(GE::Int32 &msgType) {
	return this->UnpackInt(msgType);
}

bool UnpackMessage::UnpackType(GE::Int32 &flag) {
	if(m_nSurplusSize < sizeof(char)){
		this->m_bIsOK = false;
		return false;
	}
	flag = static_cast<char>(*(curBufHead));
	curBufHead += sizeof(char);
	m_nSurplusSize -= sizeof(char);
	return true;
}

bool UnpackMessage::UnpackInt(GE::Int32& i) {
	if(m_uSize != 0 && m_nSurplusSize < sizeof(GE::Int32)){
		this->m_bIsOK = false;
		return false;
	}
	i = static_cast<GE::Int32>(*((GE::Int32*)curBufHead));
	curBufHead += sizeof(GE::Int32);
	m_nSurplusSize -= sizeof(GE::Int32);
	return true;
}

bool UnpackMessage::UnpackBool(bool &b) {
	if(m_nSurplusSize < sizeof(GE::Int32)){
		this->m_bIsOK = false;
		return false;
	}
	GE::Int32 bFlag = static_cast<GE::Int32>(*((GE::Int32*)curBufHead));
	if(bFlag == TrueFlag){
		b = true;
	}else{
		b = false;
	}
	return true;
}


bool UnpackMessage::UnpackLong(GE::Int64 &l) {
	if(m_nSurplusSize < sizeof(GE::Int64)){
		this->m_bIsOK = false;
		return false;
	}
	l = static_cast<GE::Int64>(*((GE::Int64*)curBufHead));
	curBufHead += sizeof(GE::Int64);
	m_nSurplusSize -= sizeof(GE::Int64);
	return true;
}

bool UnpackMessage::UnpackString(char *s, GE::Uint32 size) {
	if(m_nSurplusSize < size){
		this->m_bIsOK = false;
		return false;
	}
	memcpy(s, curBufHead, size);
	curBufHead += size;
	return true;
}

UnpackMessage::UnpackMessage(void *pHead){
	m_uSize = 0;
	m_nSurplusSize = 0;
	curBufHead = static_cast<char*>(pHead);
	curBufOffset = 0;
}

UnpackMessage::UnpackMessage(void *pHead, GE::Int32 nSize) {
	m_uSize = nSize;
	m_nSurplusSize = m_uSize;
	curBufHead = static_cast<char*>(pHead);
	curBufOffset = 0;
}

bool UnpackMessage::UnpackLuaObj(lua_State *L) {
	GE::Int32 top = lua_gettop(L);
	this->UnpackLuaObjHelp(L);

	ASSERT_LUA_TOP(L, top, 1);
	return this->m_bIsOK;
}

bool UnpackMessage::UnpackLuaObjHelp(lua_State *L) {
	if(!this->m_bIsOK){
		lua_pushnil(L);
		return false;
	}

	GE::Int32 top = lua_gettop(L);
	lua_checkstack(L, top + 5);
	GE::Int32 flag = 0;
	this->UnpackType(flag);
	switch(flag){
		case IntFlag:
		{
			GE::Int32 i = 0;
			if(this->UnpackInt(i)){ lua_pushinteger(L, i); }
			else{ lua_pushinteger(L, 0); }
			break;
		}
		case LongFlag:
		{

			GE::Int64 l = 0;
			if(this->UnpackLong(l)){ lua_pushinteger(L, l); }
			else{ lua_pushinteger(L, 0); }
			break;
		}
		case StringFlag:
		{
			GE::Int32 size = 0;
			this->UnpackInt(size);
			char* str = new char(size);
			if(this->UnpackString(str, size)) { lua_pushlstring(L, str, size); }
			else { lua_pushnil(L); }
			break;
		}
		case NoneFlag:
		{
			lua_pushnil(L);
			break;
		}
		case TrueFlag:
		{
			lua_pushboolean(L, 1);
			break;
		}
		case FalseFlag:
		{
			lua_pushboolean(L, 0);
			break;
		}
		case TableFlag:
		{
			GE::Int32 size = 0;
			// table的长度
			this->UnpackInt(size);
//			lua_createtable(L, size, 0);
			lua_newtable(L);
			ASSERT_LUA_TOP(L, top, 1);
			GE::Int32 table_index = lua_gettop(L);
			for(GE::Int32 idx = 0; idx < size; idx++){
//				lua_pushinteger(L, idx + 1);
				// key
				this->UnpackLuaObjHelp(L);
				// value
				this->UnpackLuaObjHelp(L);
				lua_settable(L, table_index);
			}
			break;
		}
		default:
		{
			// 压个nil
			lua_pushnil(L);
			break;
		}
	}
	ASSERT_LUA_TOP(L, top, 1);
	return true;
}