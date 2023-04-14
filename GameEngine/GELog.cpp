//
// Created by lsjlkz on 2023/4/13.
//

#include "GELog.h"
#include "GEDateTime.h"
#include "iostream"

GELog::GELog() {
	// 设置输出控制台的编码
	// SetConsoleCP(CP_UTF8);
	// 先创建一个Log文件夹
	FS::create_directories("../Log/");
	SetLogName("Server");
}

void GELog::SetLogName(const char *name) {
	std::string ss = "../Log/" + std::string(name) + ".log";
	FS::path filePath(ss);
	if(os.is_open()){
		os.close();
	}
	os.open(filePath, std::ios::app);
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
	if (os.is_open()) {
		CoutMutex.lock();
		os << GEDateTime::Instance()->GetDateTimeString() << "\t" << ss.str() << std::endl;
		CoutMutex.unlock();
	}
}

void GELog::WriteStream(const char * s) {
	if (os.is_open()) {
		CoutMutex.lock();
		os << GEDateTime::Instance()->GetDateTimeString() << "\t" << s << std::endl;
		CoutMutex.unlock();
	}
}

void GELog::WriteStream(std::string s) {
	if (os.is_open()) {
		CoutMutex.lock();
		os << GEDateTime::Instance()->GetDateTimeString() << "\t" << s << std::endl;
		CoutMutex.unlock();
	}
}
