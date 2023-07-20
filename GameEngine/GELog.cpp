//
// Created by lsjlkz on 2023/4/13.
//

#include "iostream"

#include "GELog.h"
#include "GEDateTime.h"
#include "GEProcess.h"

#define FOUT(fos, s) fos<<GEDateTime::Instance()->GetDateTimeString()<<"\t"<<s<<std::endl
#define SOUT(fos, s) std::cout<<GEDateTime::Instance()->GetDateTimeString()<<"\t"<<s<<std::endl

#define WINDEBUG
#ifdef WINDEBUG
#define GEOUT SOUT
#else
#define GEOUT FOUT
#endif

GELog::GELog():m_uLogDays(0)
{
	// 先创建一个Log文件夹
	FS::create_directories("../Log/");
}


void GELog::WriteOutStream(std::string s) {
	MakeSureLogDays();
	if (m_fileOS.is_open()) {
		m_fileMutex.lock();
		GEOUT(m_fileOS, s);
		m_fileOS.flush();
		m_fileMutex.unlock();
	}
}

void GELog::MakeSureLogDays() {
	// 确保当前的fos是当天的
	if(GEDateTime::Instance()->Days() != m_uLogDays){
		m_uLogDays = GEDateTime::Instance()->Day();
		NewLogFile();
	}
}

void GELog::NewLogFile() {
	std::stringstream ss;
	ss << "../Log/" << GEProcess::Instance()->GetProcessName() << "_" << GEDateTime::Instance()->GetDateString() << ".log";
	FS::path filePath(ss.str());
	m_fileMutex.lock();
	if(m_fileOS.is_open()){
		m_fileOS.close();
	}
	m_fileOS.open(filePath, std::ios::app);
	m_fileMutex.unlock();
}
