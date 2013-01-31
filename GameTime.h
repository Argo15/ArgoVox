#ifndef GAMETIME_H
#define GAMETIME_H

#include <time.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/time_clock.hpp"

class GameTime
{
private:
	boost::posix_time::ptime m_datetime;

	int m_nHourOffset;
	float m_nMinuteOffset;

public:
	GameTime();
	void updateToSystemTime();
	int getYear();
	int getMonth();
	int getDay();
	int getHours();
	int getMinutes();
	int getSeconds();
	int getMilliseconds();
	int getSecondsSinceMidnight();
};

#endif