//
// Created by lsjlkz on 2022/5/23.
//


#include "LuaEngine.h"
#include "GELog.h"
#include "boost/filesystem.hpp"
#include "vector"
#include "LuaBridge/LuaBridge.h"
namespace FS = boost::filesystem;

bool LuaEngine::Del() {
	if(GE_IS_POINT_NULL(m_pMainLuaState)){
		GELog::Instance()->Log("close lua_state repeat");
		return false;
	}
	lua_close(m_pMainLuaState);
	m_pMainLuaState = nullptr;
	return true;
}

bool LuaEngine::Init() {
	if(GE_IS_POINT_NOT_NULL(m_pMainLuaState)){
		GELog::Instance()->Log("new lua_state repeat");
		return false;
	}
	m_pMainLuaState = luaL_newstate();
	luaL_openlibs(m_pMainLuaState);
	executeString("LuaEngine Init Successfully");
	return true;
}

GE::Int32 LuaEngine::executeString(const char *codes) {
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	if(GE_IS_POINT_NULL(L)){
		GELog::Instance()->Log("lua engine not init");
		return 0;
	}
	luaL_dostring(L, codes);
	return 0;
}


GE::Int32 LuaEngine::LoadFile(const char *filepath) {
	if(this->HasLoadFile.count(filepath) != 0){
		// 已经加载过了
		return 1;
	}
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	if(GE_IS_POINT_NULL(L)){
		GELog::Instance()->Log("lua engine not init");
		return 1;
	}
	GE::Int32 ret = luaL_dofile(this->GetMainLuaState(), filepath);
	if(ret){
		GELog::Instance()->Log("load lua file error:", filepath);
		return 1;
	}
	// 这个时候已经把file中返回的table压栈了
	if(lua_istable(L, -1)){
		// 栈顶是表的话，应该是全局表，看看有没有init
		lua_getfield(L, -1, "init");
		if(lua_isfunction(L, -1)){
			// 存在init方法
			lua_pcall(L, 0, 0, 0);
		}
	}
	// 出栈
	lua_pop(L, 1);
	this->HasLoadFile.insert(filepath);
	return 0;
}

GE::Int32 LuaEngine::LoadModule(const char *pkgName) {
	std::stringstream ss;
	ss << "../LuaCode/" << pkgName;
	FS::path pkgPath(ss.str());
	std::vector<std::string> dirVector;
	if(!FS::exists(pkgPath) || !FS::is_directory(pkgPath)){
		GELog::Instance()->Log("error pkg", pkgName);
		return 0;
	}
	dirVector.emplace_back(ss.str());
	while (!dirVector.empty()){
		std::string path = dirVector.back();
		dirVector.pop_back();
		FS::path dirPath(path);
		if(!FS::exists(dirPath) || !FS::is_directory(dirPath)){
			continue;
		}
		for(const FS::directory_entry &entry: FS::directory_iterator(dirPath)){
			if(FS::is_regular_file(entry.status())){
				// 文件
				this->LoadFile(entry.path().string().c_str());
			}else if(FS::is_directory(entry.status())){
				// 目录
				dirVector.emplace_back(entry.path().string());
			}
		}
	}
	return 1;
}


GE::Int32 LuaEngine::DoFile(const char *filepath) {
	lua_State* L = LuaEngine::Instance()->GetMainLuaState();
	if(GE_IS_POINT_NULL(L)){
		GELog::Instance()->Log("lua engine not init");
		return 1;
	}
	GE::Int32 ret = luaL_dofile(L, filepath);
	if(ret){
		GELog::Instance()->Log("load lua file error:", filepath);
	}
	return ret;
}

LuaEngine::LuaEngine():m_pMainLuaState(nullptr){

}
