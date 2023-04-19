//
// Created by lsjlkz on 2023/4/13.
//

#include "GELog.h"
#include "GEDateTime.h"
#include "GEProcess.h"


#define FOUT(fos, s) fos<<GEDateTime::Instance()->GetDateTimeString()<<"\t"<<s<<std::endl

GELog::GELog():m_uLogDays(0)
{
	// 先创建一个Log文件夹
	FS::create_directories("../Log/");
}

void GELog::Log(const char *msg) {
	// TODO
	WriteStream(msg);
}

void GELog::Log(const char *msg, const char *msg2) {
	// TODO
	sstream s;
	s << msg << "\t" << msg2;
	WriteStream(s.str());
}

void GELog::Log(const char *msg, GE::Int32 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s.str());
}

void GELog::Log(const char *msg, GE::Uint32 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s.str());
}

void GELog::Log(const char *msg, GE::Int64 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s.str());
}

void GELog::Log(const char *msg, GE::Uint64 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s.str());
}

void GELog::WriteStream(sstream &ss) {
	MakeSureLogDays();
	if (m_fileOS.is_open()) {
		m_fileMutex.lock();
//		m_fileOS << GEDateTime::Instance()->GetDateTimeString() << "\t" << ss.str() << std::endl;
		FOUT(m_fileOS, ss.str());
		m_fileMutex.unlock();
	}
}


void GELog::WriteStream(const char * s) {
	MakeSureLogDays();
	if (m_fileOS.is_open()) {
		m_fileMutex.lock();
		FOUT(m_fileOS, s);
		m_fileMutex.unlock();
	}
}

void GELog::WriteStream(std::string s) {
	MakeSureLogDays();
	if (m_fileOS.is_open()) {
		m_fileMutex.lock();
		FOUT(m_fileOS, s);
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
