//
// Created by lsjlkz on 2022/7/22.
//

#include "GEDateTime.h"

GEDateTime::GEDateTime() {
	// 初始化默认时间
	tzset();
	this->UpdateDateTime();
}

void GEDateTime::CacheDateTime() {
	time_t _tt = static_cast<time_t>(this->tm_unixTime);
	tm* _tm = localtime(&_tt);
	this->tm_year = _tm->tm_year;
	this->tm_month = _tm->tm_mon + 1;
	this->tm_day = _tm->tm_mday;
	this->tm_hour = _tm->tm_hour;
	this->tm_minute = _tm->tm_min;
	this->tm_second = _tm->tm_sec;
	this->tm_weekDay = _tm->tm_wday;
	this->tm_yearDay = _tm->tm_yday;
}

void GEDateTime::UpdateDateTime() {
	GE::Uint64 mseconds = GESystemTime::GetSystemMSeconds();
	GE::Uint64 now_time = mseconds / 1000;
	if(this->tm_unixTime != now_time){
		// 过了一秒
		this->tm_unixTime = now_time;
		this->CacheDateTime();
	}
}

GE::Int32 GEDateTime::Year() {
	return this->tm_year;
}

GE::Int32 GEDateTime::Month() {
	return this->tm_month;
}

GE::Int32 GEDateTime::Day() {
	return this->tm_day;
}

GE::Int32 GEDateTime::Hour() {
	return this->tm_hour;
}

GE::Int32 GEDateTime::Minute() {
	return this->tm_minute;
}

GE::Int64 GEDateTime::Second() {
	return this->tm_second;
}

GE::Int64 GEDateTime::UnixTime() {
	return this->tm_unixTime;
}
