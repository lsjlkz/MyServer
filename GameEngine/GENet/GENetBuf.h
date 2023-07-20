//
// Created by lsjlkz on 2023/3/20.
//

#ifndef MYSERVER_GENETBUF_H
#define MYSERVER_GENETBUF_H

#include "queue"
#include "GEDefine.h"

class GENetBuf;

typedef std::queue<GENetBuf*> tdBufQueue;
typedef std::vector<GENetBuf*> tdBufVec;

class GENetBuf {
public:
	GENetBuf(GE::Uint16 uMaxSize);
	~GENetBuf();

public:
	void 			Reset(GE::Uint16 uSize=0);	// 初始化
	void*			HeadPtr(GE::Uint16 uSize=0){return m_pHead + uSize;}						// buf头指针
	GE::Uint16 		MaxSize(){return m_uMaxSize;}
	GE::Uint16 		CanWriteSize(){return m_uMaxSize - m_uWriteSize;}
	GE::Uint16 		CanReadSize(){return m_uWriteSize - m_uReadSize;}

	void*			WriteFence_us(){return m_pHead + m_uWriteSize;}
	void			WriteBytes_us(const void* phead, GE::Uint16 uSize);						// 一定长度的拷到buf中
	void 			MoveWriteFence_us(GE::Uint16 uSize){m_uWriteSize += uSize;}				// 指针移动
	void*			ReadFence_us(){return m_pHead + m_uReadSize;}
	void 			MoveReadFence_us(GE::Uint16 uSize){m_uReadSize += uSize;}


public:
	char*			m_pHead;		// buff头指针
	GE::Uint16 		m_uReadSize;	// 已经读的长度
	GE::Uint16 		m_uWriteSize;	// 已经写的长度
	GE::Uint16 		m_uMaxSize;		// 最大长度
};


#endif //MYSERVER_GENETBUF_H
