//
// Created by lsjlkz on 2023/9/27.
//

#ifndef MYSERVER_LUASUBTHREAD_H
#define MYSERVER_LUASUBTHREAD_H

#include "boost/thread.hpp"
#include "LuaTask.h"

class LuaSubThread{
	typedef TaskTarget TT;
	typedef boost::thread			tdBoostThread;

public:
	LuaSubThread(){
		this->L = luaL_newstate();
	}

	~LuaSubThread(){
		lua_close(this->L);
		L = nullptr;
	}

	void LoadLuaMainFunction(){
		// 加载lua层的main
	}

	void Excute(TT tt){
		switch(tt.uTargetType){
			// 初始化的任务
			case TargetTypeEnum::TargetTypeInit:

		}
	}

private:
	lua_State* L = nullptr;
};

#endif //MYSERVER_LUASUBTHREAD_H
