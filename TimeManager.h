#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <time.h>
#include <string>
#include "GameTime.h"

class TimeManager 
{
private:
	static TimeManager *m_pInstance;
	GameTime m_gameTime;
	int m_nTimestamp;

	TimeManager();

public:
	static TimeManager *getInstance();
	std::string toString();
	void tick();
	GameTime getGameTime();
	int getTimeStamp();
	int getTimeInMilliseconds();
	int getTimeInMicroseconds();
};

#endif