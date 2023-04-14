// Log
// Created by lsjlkz on 2023/4/13.
//

#ifndef MYSERVER_GELOG_H
#define MYSERVER_GELOG_H

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
	typedef std::stringstream 		sstream;



	GELog();

	void SetLogName(const char* name);

	void Log(const char* msg);

	void Log(const char* msg, const char* msg2);

	void Log(const char *msg, GE::Int32 value);

	void Log(const char *msg, GE::Uint32 value);

	void Log(const char *msg, GE::Int64 value);
	void Log(const char *msg, GE::Uint64 value);

	void WriteStream(sstream &ss);
	void WriteStream(const char* s);
	void WriteStream(std::string s);

private:
	Mutex CoutMutex;

	FS::ofstream os;
};


#endif //MYSERVER_GELOG_H
