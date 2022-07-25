//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_GEDATETIME_H
#define MYSERVER_GEDATETIME_H

#include "GESystemTime.h"

class GEDateTime:public SingleTon<GEDateTime> {
public:
	GEDateTime();

public:
	void CacheDateTime();
	void UpdateDateTime();

	long Year();
	long Month();
	long Day();
	long Hour();
	long Minute();
	long Second();
	long UnixTime();

private:
	int tm_year;
	int tm_month;
	int tm_day;
	int tm_hour;
	int tm_minute;
	int tm_second;
	int tm_weekDay;
	int tm_yearDay;
	long tm_unixTime;
};


#endif //MYSERVER_GEDATETIME_H
