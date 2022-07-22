//
// Created by lsjlkz on 2022/7/22.
//

#include "GESystemTime.h"

unsigned long long GESystemTime::GetSystemMSeconds() {
	std::time_t result = std::time(NULL);
	return 1000 * result;
}
