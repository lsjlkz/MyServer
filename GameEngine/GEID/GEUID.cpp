//
// Created by lsjlkz on 2023/7/3.
//

#include "GEDateTime.h"
#include "GELog.h"
#include "GEUID.h"

GEUID::GEUID() {
	this->UpdateLastSeconds();
}

void GEUID::UpdateLastSeconds() {
	if(this->uLastUpdateSeconds == GEDateTime::Instance()->Seconds()){
		return;
	}
	this->uLastUpdateSeconds = GEDateTime::Instance()->Seconds();
	// 从1开始
	this->uCurCount = 1;
}

GE::Uint64 GEUID::AllotUID() {
	this->UpdateLastSeconds();
	if(this->uCurCount >= this->uMaxCount){
		GELog::Instance()->Log("error allot uid max:",  this->uCurCount);
		return 0;
	}
	GE::Uint64 curSecondsID = (this->uLastUpdateSeconds & GEUID_CUR_SECONDS_BIT) << GEUID_CUR_SECONDS_OFFSET;
	// 目标就是当前时间戳吧
	GE::Uint64 targetSecondsID = (this->uLastUpdateSeconds & GEUID_TARGET_SECONDS_BIT) << GEUID_TARGET_SECONDS_OFFSET;
	return curSecondsID | targetSecondsID | (this->uCurCount++);
}

GE::Uint64 GEUID::AllotUID(GE::Uint64 uTargetSeconds) {
	this->UpdateLastSeconds();
	if(this->uCurCount >= this->uMaxCount){
		GELog::Instance()->Log("error allot uid max:",  this->uCurCount);
		return 0;
	}
	GE::Uint64 curSecondsID = (this->uLastUpdateSeconds & GEUID_CUR_SECONDS_BIT) << GEUID_CUR_SECONDS_OFFSET;
	GE::Uint64 targetSecondsID = (uTargetSeconds & GEUID_TARGET_SECONDS_BIT) << GEUID_TARGET_SECONDS_OFFSET;
	return curSecondsID | targetSecondsID | (this->uCurCount++);
}