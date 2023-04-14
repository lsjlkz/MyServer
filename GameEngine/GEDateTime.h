//
// Created by lsjlkz on 2022/7/22.
//

#ifndef MYSERVER_GEDATETIME_H
#define MYSERVER_GEDATETIME_H

#include "GEDefine.h"
#include "GESingleton.h"
#include "string"

class GEDateTime:public GESingleton<GEDateTime> {
public:
	GEDateTime();

public:
	void CacheDateTime();
	void UpdateDateTime();

	GE::Int32 Year();
	GE::Int32 Month();
	GE::Int32 Day();
	GE::Int32 Hour();
	GE::Int32 Minute();
	GE::Int64 Second();
	GE::Int64 UnixTime();
	const std::string GetDateTimeString();

private:
	GE::Int32 tm_year;
	GE::Int32 tm_month;
	GE::Int32 tm_day;
	GE::Int32 tm_hour;
	GE::Int32 tm_minute;
	GE::Int64 tm_second;
	GE::Int32 tm_weekDay;
	GE::Int32 tm_yearDay;
	GE::Int64 tm_unixTime;
};


#endif //MYSERVER_GEDATETIME_H
