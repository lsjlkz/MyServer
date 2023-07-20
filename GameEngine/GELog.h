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

	template <typename T>
	void WriteToStream(sstream &s, T&& t){
		s << t;
	}

	// 这个不能删，不定长参数需要用
	template <typename T, typename ...Args>
	void WriteToStream(sstream& s, T&& t, Args &&... arguments){
		s << t;
		WriteToStream(s, std::forward<Args>(arguments)...);
	}

	template <typename ...Args>
	void Log(Args &&... arguments){
		sstream s;
		WriteToStream(s, std::forward<Args>(arguments)...);
		WriteOutStream(s.str());
	}

	void MakeSureLogDays();

	void NewLogFile();

	void WriteOutStream(std::string s);

private:
	boost::mutex m_fileMutex;

	FS::ofstream m_fileOS;

	GE::Uint32 m_uLogDays;
};


#endif //MYSERVER_GELOG_H
