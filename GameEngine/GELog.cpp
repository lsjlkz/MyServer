//
// Created by lsjlkz on 2023/4/13.
//

#include "GELog.h"
#include "GEDateTime.h"

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
	os.open(filePath, std::ios::app);
}

void GELog::Log(const char *msg) {
	// TODO
	sstream s;
	s << msg;
	WriteStream(s);
}

void GELog::Log(const char *msg, const char *msg2) {
	// TODO
	sstream s;
	s << msg << "\t" << msg2;
	WriteStream(s);
}

void GELog::Log(const char *msg, GE::Int32 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s);
}

void GELog::Log(const char *msg, GE::Uint32 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s);
}

void GELog::Log(const char *msg, GE::Int64 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s);
}

void GELog::Log(const char *msg, GE::Uint64 value) {
	// TODO
	sstream s;
	s << msg << "\t" << value;
	WriteStream(s);
}

void GELog::WriteStream(sstream &ss) {
	if (os.is_open()) {
		CoutMutex.lock();
		// TODO 时间
		os << GEDateTime::Instance()->GetDateTimeString() << "\t" << ss.str() << std::endl;
		CoutMutex.unlock();
		os.close();
	}
}
