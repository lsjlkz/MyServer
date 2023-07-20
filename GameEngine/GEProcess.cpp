//
// Created by lsjlkz on 2023/3/6.
//

#include <cstring>
#include "GEProcess.h"
#include "GELog.h"

GEProcess::GEProcess(){
	// 给个默认值
	m_sProcessName = new char[1];
	m_sProcessName[0] = '0';
}

void GEProcess::SetProcessName(const char *processName, GE::Uint16 size) {
	GE_SAFE_DELETE_ARRAY_POINT(m_sProcessName);
	m_sProcessName = new char[size];
	memcpy(m_sProcessName, processName, size);
}

void GEProcess::SetClientRedirect(GE::Uint32 RedirectType, GE::Uint32 SessionID){
	GELog::Instance()->Log("SetClientRedirect ", RedirectType, "\t", SessionID);
	this->ClientRedirectArray[RedirectType] = SessionID;
}

const GE::Uint32 GEProcess::GetClientRedirect(GE::Uint32 RedirectType) {
	if(RedirectType >= CLIENT_REDIRECT_MAX){
		// 超了，为啥这里不返回0，因为0也是sessionID
		GELog::Instance()->Log("error client redirect too max");
		return MAX_UINT32;
	}
	return this->ClientRedirectArray[RedirectType];
}

