//
// Created by lsjlkz on 2023/3/6.
//

#include <cstring>
#include "GEProcess.h"

void GEProcess::SetProcessName(const char *processName, GE::Uint16 size) {
	GE_SAFE_DELETE_ARRAY_POINT(m_sProcessName);
	m_sProcessName = new char[size];
	memcpy(m_sProcessName, processName, size);
}

GEProcess::GEProcess(){
	// 给个默认值
	m_sProcessName = new char[1];
	m_sProcessName[0] = '0';
}

