// Log
// Created by lsjlkz on 2023/4/13.
//

#ifndef MYSERVER_GELOG_H
#define MYSERVER_GELOG_H

#include <Windows.h>
#include <boost/thread/mutex.hpp>
#include "GEDefine.h"
#include "GESingleton.h"
#include "fstream"
#include "iostream"
#include "boost/filesystem.hpp"
namespace FS = boost::filesystem;


class GELog: public GESingleton<GELog>{
public:
	typedef boost::mutex			Mutex;



	GELog(){
		// 设置输出控制台的编码
		// SetConsoleCP(CP_UTF8);
		// 先创建一个Log文件夹
		FS::create_directories("../Log/");
		SetLogName("Server");
	}

	void SetLogName(const char* name){
		std::string ss = "../Log/" + std::string(name) + ".log";
		FS::path filePath(ss);
		os.open(filePath);
	}

	void Log(const char* msg){
		// TODO
		if(os.is_open()) {
			CoutMutex.lock();
			os << msg << std::endl;
			CoutMutex.unlock();
			os.close();
		}
	}

	void Log(const char* msg, const char* msg2){
		// TODO
		if(os.is_open()) {
			CoutMutex.lock();
			os << msg << "\t" << msg2 << std::endl;
			CoutMutex.unlock();
			os.close();
		}
	}

	void Log(const char *msg, GE::Int32 i32) {
		// TODO
		if (os.is_open()) {
			CoutMutex.lock();
			os << msg << "\t" << i32 << std::endl;
			CoutMutex.unlock();
			os.close();
		}
	}

	void Log(const char *msg, GE::Uint32 ui32) {
		// TODO
		if (os.is_open()) {
			CoutMutex.lock();
			os << msg << "\t" << ui32 << std::endl;
			CoutMutex.unlock();
			os.close();
		}
	}

	void Log(const char *msg, GE::Int64 i64) {
		// TODO
		if (os.is_open()) {
			CoutMutex.lock();
			os << msg << "\t" << i64 << std::endl;
			CoutMutex.unlock();
			os.close();
		}
	}

	void Log(const char *msg, GE::Uint64 ui64) {
		// TODO
		if (os.is_open()) {
			CoutMutex.lock();
			os << msg << "\t" << ui64 << std::endl;
			CoutMutex.unlock();
			os.close();
		}
	}

private:
	Mutex CoutMutex;

	FS::ofstream os;
};


#endif //MYSERVER_GELOG_H
