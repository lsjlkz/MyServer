//
// Created by lsjlkz on 2022/7/19.
//

#ifndef MYSERVER_HEAD_H
#define MYSERVER_HEAD_H

// 需要加上这一行才行
#pragma comment(lib, "Lua.lib")
#pragma comment(lib, "libmysql.lib")

extern "C"{
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}

#include "boost/thread.hpp"
#include <boost/asio.hpp>
#include "GESingleton.h"
#include "GEDefine.h"


#endif //MYSERVER_HEAD_H
