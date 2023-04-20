cmake_minimum_required(VERSION 3.20)
project(MyServer)

set(CMAKE_CXX_STANDARD 11)
set(LINK_DIR ${PROJECT_SOURCE_DIR}/Third/CLib)
set(INC_DIR ${PROJECT_SOURCE_DIR}/Third/Include)
set(BOOST_INC_DIR C:/boost_1_73_0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT")


link_directories(${LINK_DIR})
link_directories(GameEngine CommonBase)

include_directories(${INC_DIR})
include_directories(${BOOST_INC_DIR})
include_directories(GameEngine CommonBase)

add_compile_definitions(WIN)            # 宏定义WIN

add_compile_options(/wd4819)            # 关闭编译器对于代码中“非ANSI字符”的警告
#add_compile_options(-O2)                # release编译优化

add_definitions(-D_WIN32_WINNT=0x0601)  # 最低操作系统
add_definitions(-DJTHREADS=4)           # 多线程编译


add_executable(MyServer main.cpp
        GameEngine/GESingleton.h
        GameEngine/LuaEngine.cpp
        GameEngine/LuaEngine.h
        GameEngine/LuaGameServer.cpp
        GameEngine/LuaGameServer.h
        GameEngine/GameServer.cpp
        GameEngine/GameServer.h
        GameEngine/GENet/GENetWork.cpp
        GameEngine/GENet/GENetWork.h
        GameEngine/GSDefine.h
        GameEngine/GENet/GENetPack.cpp
        GameEngine/GENet/GENetPack.h
        GameEngine/GEDefine.h
        CommonBase/CommomHelp.cpp
        CommonBase/CommomHelp.h
        GameEngine/GEDateTime.cpp
        GameEngine/GEDateTime.h
        GameEngine/GESystemTime.cpp
        GameEngine/GESystemTime.h
        GameEngine/LuaEvent.cpp
        GameEngine/LuaEvent.h
        GameEngine/GENet/GENetConnect.cpp
        GameEngine/GENet/GENetConnect.h
        GameEngine/GENet/GENetConnectMgr.cpp
        GameEngine/GENet/GENetConnectMgr.h
        GameEngine/GEProcess.cpp
        GameEngine/GEProcess.h
        GameEngine/GENet/GENetSendBuf.cpp
        GameEngine/GENet/GENetSendBuf.h
        GameEngine/GENet/GENetBuf.cpp
        GameEngine/GENet/GENetBuf.h GameEngine/GENet/GENetMessage.cpp GameEngine/GENet/GENetMessage.h GameEngine/GENet/GENetRecvBuf.cpp GameEngine/GENet/GENetRecvBuf.h GameEngine/GELog.cpp GameEngine/GELog.h GameEngine/LuaInclude.h)

