//
// Created by lsjlkz on 2023/7/19.
//

#pragma once

#ifndef MYSERVER_LUACONST_H
#define MYSERVER_LUACONST_H

#include "LuaFunction.h"
#include "GESingleton.h"


class LuaConst: public GESingleton<LuaConst>{
public:
	LuaFunction* ConnectedLuaFunction = nullptr;
};


#endif //MYSERVER_LUACONST_H
