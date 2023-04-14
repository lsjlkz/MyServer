// 接收缓冲区
// Created by lsjlkz on 2023/4/13.
//

#ifndef MYSERVER_GENETRECVBUF_H
#define MYSERVER_GENETRECVBUF_H


#include "GENetMessage.h"
#include "GENetBuf.h"

class GENetRecvBuf {

public:
	GENetRecvBuf(GE::Uint16 uBlockSize, GE::Uint16 uBlockNum);
	~GENetRecvBuf(void);


public:
	void				UsePool(GE::Uint16 uSize);							// 消息量很大的时候使用对象池
	bool				WriteBytes(MsgBase* pMsg);							// 数据写入缓冲区
	bool				ReadMsgFromReadBuf(MsgBase** pMsg);					// 从缓冲区中读东西到pMsg中
	bool				MoveToNextReadBuf();								// 读下一个buf
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

	// TODO 为什么Pool用指针而Queue用对象，估计是因为Queue是肯定会被用上的，但是Pool不一定？
	// GPT说
	// 这是因为，缓冲块池和缓冲块队列的功能不同。
	// 缓冲块池是用于存储和管理一组可重复利用的缓冲块，因此需要快速地分配和回收缓冲块，并且需要高效地处理缓冲块之间的引用和指针。
	// 使用指针可以更方便地管理缓冲块之间的关系，因为指针可以直接指向缓冲块的内存地址，而且指针的操作相对来说更加高效。
	// 缓冲块队列则是用于管理一组正在被使用的缓冲块，因此需要支持插入和删除缓冲块，并且需要支持遍历和访问缓冲块。
	// 使用对象可以更好地封装缓冲块的数据和操作，因为对象可以包含缓冲块的状态和行为，并且可以通过对象的方法来管理缓冲块的生命周期和状态。
	tdBufVec*			m_pBufPool;
	tdBufQueue*			m_pBufQueue;

	GE::Uint16 			m_uBlockNum;					// 允许的最大的缓冲块的数量
	GE::Uint16 			m_uNewCnt;
	GE::Uint16 			m_uDelCnt;
};


#endif //MYSERVER_GENETRECVBUF_H
