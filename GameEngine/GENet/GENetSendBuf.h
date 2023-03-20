// 发送的头
// Created by lsjlkz on 2023/3/20.
//
#ifndef MYSERVER_GENETSENDBUF_H
#define MYSERVER_GENETSENDBUF_H

#include "queue"
#include "GENetBuf.h"

class GENetSendBuf {
	typedef std::queue<GENetBuf*> tdBufQueue;
	typedef std::vector<GENetBuf*> tdBufVec;

public:
	GENetSendBuf(GE::Uint16 uBlockSize, GE::Uint16 uBlockNum);
	~GENetSendBuf(void);

public:
	void				UsePool(GE::Uint16 uSize);							// 消息量很大的时候使用对象池
	bool				WriteBytes(const void* pHead, GE::Uint16 uSize);	// 数据写入缓冲区
	bool				HoldBlock(void** pHead, GE::Uint16& uSize);			// Hold住一块，然后可以发送
	bool				ReleaseBlock();										// 上次Hold住的发送完毕，释放掉，返回是否还有数据
	bool				IsEmpty();
	GE::Uint16 			GetNewCnt(){return m_uNewCnt;}
	GE::Uint16 			GetDelCnt(){return m_uDelCnt;}
	GE::Uint16 			GetPoolCnt();

private:
	GENetBuf*			NewNetBuf(GE::Uint16 uSize);						// 获取一个新的Buf，new或者池弹出
	void 				DelNetBuf(GENetBuf* pBuff);							// 回池


private:
	// 这个SendBuf需要写缓存和读缓存
	// 其中写缓存，当有需要发送的数据时，直接写入到写缓存中
	// 写缓存在满了之后会push进queue待发送
	// 读缓存是由写缓存交换过来的
	// 当读缓存空了的时候，队列也没什么可以发送了，就直接把写缓存拿过来继续发送
	// 然后原来的读缓存变成写缓存，可以继续写入数据
	// 这样子就可以避免读写操作的竞争
	GENetBuf*			m_pReadBuf;
	GENetBuf*			m_pWriteBuf;

	tdBufVec*			m_pBufPool;
	tdBufQueue			m_pBufQueue;

	bool 				m_bIsHoldBlock;
	GE::Uint16 			m_uBlockNum;
	GE::Uint16 			m_uNewCnt;
	GE::Uint16 			m_uDelCnt;

};


#endif //MYSERVER_GENETSENDBUF_H
