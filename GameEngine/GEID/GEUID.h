//
// Created by lsjlkz on 2023/7/3.
//

#ifndef MYSERVER_GEUID_H
#define MYSERVER_GEUID_H

#include "GESingleton.h"
#include "GEDefine.h"

#define GEUID_MAX_COUNT_OFFSET 0
#define GEUID_MAX_COUNT_BIT_LENGTH 16		// 保留16个bit，每秒可注册数量，应该够了吧这个，
#define GEUID_CUR_SECONDS_OFFSET (GEUID_MAX_COUNT_OFFSET+GEUID_MAX_COUNT_BIT_LENGTH)
#define GEUID_CUR_SECONDS_BIT_LENGTH 16		// 当前时间戳，16个bit，应该够了
#define GEUID_TARGET_SECONDS_OFFSET (GEUID_CUR_SECONDS_OFFSET+GEUID_CUR_SECONDS_BIT_LENGTH)
#define GEUID_TARGET_SECONDS_BIT_LENGTH 32	// 目标时间戳，32个bit，够了
//#define GEUID_TARGET_SECONDS_BIT ((1UL<<32)-1)	// 第二个性能更好点
#define GEUID_TARGET_SECONDS_BIT 0XFFFFFFFF	// 32个bit
#define GEUID_CUR_SECONDS_BIT 0XFFFF		// 16个bit

class GEUID: public GESingleton<GEUID> {
public:
	GEUID();

	void UpdateLastSeconds();
	GE::Uint64 AllotUID();
	GE::Uint64 AllotUID(GE::Uint64 uTargetSeconds);

private:
	GE::Uint64 uMaxCount = (1 << GEUID_MAX_COUNT_BIT_LENGTH) - 1;
	GE::Uint64 uCurCount = 1;
	GE::Uint64 uLastUpdateSeconds;
	// 其中28个bit表示当前时间戳，再拼28个bit表示执行时间戳，也就是maxCout最大2^8-1，也够用了
};


#endif //MYSERVER_GEUID_H
