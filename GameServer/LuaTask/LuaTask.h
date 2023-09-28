//
// Created by lsjlkz on 2023/9/27.
//

#ifndef MYSERVER_LUATASK_H
#define MYSERVER_LUATASK_H

#include "../Lua/LuaInclude.h"
#include "GEDefine.h"


// 任务
union TaskTarget{
	// 任务的参数
	struct {
		union {
			struct {
				GE::Uint8 uTargetType;
				GE::Uint8 uTargetData;
				GE::Uint16 uTargetParam;
			};
			GE::Uint32 uTargetID;
		};
		// 基础信息
		union {
			struct {
				GE::Uint16 uParam1;
				GE::Uint16 uParam2;
			};
			GE::Uint32 uParam;
		};
	};
	GE::Uint64 uTaskTarget;
};


enum TargetTypeEnum{
	TargetTypeInit
};

class LuaTask{
	public:
		LuaTask(lua_State*L, TaskTarget tt):taskTarget(tt){
			this->taskProcessLuaState = L;
		};

		void Excute(){
			// 先清空栈
			lua_settop(this->taskProcessLuaState, 0);
			// TODO
		}




	private:
		TaskTarget taskTarget;
		lua_State* taskProcessLuaState;
};

#endif //MYSERVER_LUATASK_H
