cmake_minimum_required(VERSION 3.20)
project(MyServer)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(/wd4819)

set(LINK_DIR ${PROJECT_SOURCE_DIR}/Third/CLib)
set(INC_DIR ${PROJECT_SOURCE_DIR}/Third/Include)
set(BOOST_INC_DIR D:/boost_1_73_0)

link_directories(${LINK_DIR})
link_directories(GameEngine CommonBase)

include_directories(${INC_DIR})
include_directories(${BOOST_INC_DIR})
include_directories(GameEngine CommonBase)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_definitions(WIN)

add_executable(MyServer main.cpp
        CommonBase/Vector2.cpp
        CommonBase/Vector2.h
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
        GameEngine/Head.h
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
        GameEngine/GENet/GENetBuf.h GameEngine/GENet/GENetMessage.cpp GameEngine/GENet/GENetMessage.h)

option(WIN "WIN" ON)
