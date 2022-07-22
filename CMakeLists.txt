cmake_minimum_required(VERSION 3.20)
project(MyServer)

set(CMAKE_CXX_STANDARD 11)


set(INC_DIR ${PROJECT_SOURCE_DIR}/Third/Include)
set(LINK_DIR ${PROJECT_SOURCE_DIR}/Third/CLib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

include_directories(GameEngine CommonBase)
link_directories(GameEngine CommonBase)


add_executable(MyServer main.cpp CommonBase/Vector2.cpp CommonBase/Vector2.h GameEngine/SingleTon.h GameEngine/LuaEngine.cpp GameEngine/LuaEngine.h GameEngine/LuaGameServer.cpp GameEngine/LuaGameServer.h GameEngine/GameServer.cpp GameEngine/GameServer.h GameEngine/GENetWork.cpp GameEngine/GENetWork.h GameEngine/GSDefine.h GameEngine/GENetPack.cpp GameEngine/GENetPack.h GameEngine/GEDefine.cpp GameEngine/GEDefine.h GameEngine/Head.h Test/Test.cpp Test/Test.h CommonBase/CommomHelp.cpp CommonBase/CommomHelp.h GameEngine/GEDateTime.cpp GameEngine/GEDateTime.h GameEngine/GESystemTime.cpp GameEngine/GESystemTime.h GameEngine/LuaEvent.cpp GameEngine/LuaEvent.h)
