// Log
// Created by lsjlkz on 2023/4/13.
//

#ifndef MYSERVER_GELOG_H
#define MYSERVER_GELOG_H

#include "boost/filesystem.hpp"
#include "boost/thread/mutex.hpp"

#include "GEDefine.h"
#include "GESingleton.h"


namespace FS = boost::filesystem;


class GELog: public GESingleton<GELog>{
public:
	typedef std::stringstream 		sstream;

public:
	GELog();

	void Log(const char* msg);

	void Log(const char* msg, const char* msg2);

	void Log(const char *msg, GE::Int32 value);

	void Log(const char *msg, GE::Uint32 value);

	void Log(const char *msg, GE::Int64 value);
	void Log(const char *msg, GE::Uint64 value);

	void MakeSureLogDays();

	void NewLogFile();

	void WriteStream(sstream &ss);
	void WriteStream(const char* s);
	void WriteStream(std::string s);

private:
	boost::mutex m_fileMutex;

	FS::ofstream m_fileOS;

	GE::Uint32 m_uLogDays;
};


#endif //MYSERVER_GELOG_H
