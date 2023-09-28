//
// Created by lsjlkz on 2023/9/27.
//

#ifndef MYSERVER_GEREADWRITE_H
#define MYSERVER_GEREADWRITE_H

#include "GELog.h"
#include "vector"
#include "boost/thread/mutex.hpp"

template <typename T>
class GEReadWrite{
	typedef std::vector<T> tdVector;

public:
	GEReadWrite(){
		tdReadVector = new tdVector();
		tdWriteVector = new tdVector();
	}
	~GEReadWrite(){
		if(!tdReadVector->empty()){
			GELog::Instance()->E("readVector not empty");
		}
		if(!tdWriteVector->empty()){
			GELog::Instance()->E("writeVector not empty");
		}
		delete tdReadVector;
		delete tdWriteVector;
	}

	void WriteOne(T t){
		this->tdWriteVector->emplace_back(t);
	}

	void WriteMany(tdVector vector){
		this->tdWriteVector->insert(this->tdWriteVector->end(), vector.begin(), vector.end());
	}

	tdVector* ReadAll(){
		// 这里是需要读writeVector，需要清空readVector，然后替换，接着写入
		if(!this->tdReadVector->empty()){
			this->tdReadVector->clear();
		}
		tdVector* temp = this->tdWriteVector;
		this->tdWriteVector = tdReadVector;
		this->tdReadVector = temp;
		return temp;
	}

private:
	tdVector* tdReadVector;
	tdVector* tdWriteVector;

};

#endif //MYSERVER_GEREADWRITE_H
