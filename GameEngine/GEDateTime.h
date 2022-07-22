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

	long Year(){return this->tm_year;}
	long Month(){return this->tm_month;}
	long Day(){return this->tm_day;}
	long Hour(){return this->tm_hour;}
	long Minute(){return this->tm_minute;}
	long Second(){return this->tm_second;}
	unsigned long long UnixTime(){return this->tm_unixTime;}

private:
	int tm_year;
	int tm_month;
	int tm_day;
	int tm_hour;
	int tm_minute;
	int tm_second;
	int tm_weekDay;
	int tm_yearDay;
	unsigned long long tm_unixTime;
};


#endif //MYSERVER_GEDATETIME_H
