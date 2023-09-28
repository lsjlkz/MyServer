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
add_compile_definitions(WINDEBUG)            # 宏定义WIN

add_compile_options(/wd4819)            # 关闭编译器对于代码中“非ANSI字符”的警告
#add_compile_options(-O2)                # release编译优化

add_definitions(-D_WIN32_WINNT=0x0601)  # 最低操作系统
add_definitions(-DJTHREADS=4)           # 多线程编译


AUX_SOURCE_DIRECTORY(. GAME_SRCS)
ADD_EXECUTABLE(MyServer ${GAME_SRCS})


ADD_SUBDIRECTORY(GameEngine)
ADD_SUBDIRECTORY(GameServer)
ADD_SUBDIRECTORY(Third/Include/LuaBridge)

TARGET_LINK_LIBRARIES(MyServer GAME_ENGINE )
TARGET_LINK_LIBRARIES(MyServer GENET )
TARGET_LINK_LIBRARIES(MyServer GEID )
TARGET_LINK_LIBRARIES(MyServer GAME_SERVER )
TARGET_LINK_LIBRARIES(MyServer LUA_SERVER )
#TARGET_LINK_LIBRARIES(MyServer GETHREAD )
TARGET_LINK_LIBRARIES(MyServer LUA_TASK )
#TARGET_LINK_LIBRARIES(MyServer LuaBridge )


