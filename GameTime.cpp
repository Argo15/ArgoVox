#include "GameTime.h"
#include "InputManager.h"

GameTime::GameTime() 
{
	m_nHourOffset = 0;
	m_nMinuteOffset = 0;
}

void GameTime::updateToSystemTime() 
{
	time_t t = time(0);
	struct tm * now = localtime( & t );
    if (InputManager::getInstance()->isKeyDownOnce('-')) 
	{
		m_nHourOffset -= 1;
	}
	if (InputManager::getInstance()->isKeyDownOnce('=')) 
	{
		m_nHourOffset += 1;
	}
	if (InputManager::getInstance()->isKeyDown('[')) 
	{
		m_nMinuteOffset -= 1.0;
	}
	if (InputManager::getInstance()->isKeyDown(']')) 
	{
		m_nMinuteOffset += 1.0;
	}

	m_datetime = boost::posix_time::second_clock::local_time() + boost::posix_time::hours(m_nHourOffset) + boost::posix_time::minutes((long)m_nMinuteOffset);
}

int GameTime::getYear() 
{ 
	return m_datetime.date().year();
}

int GameTime::getMonth() 
{ 
	return m_datetime.date().month();
}

int GameTime::getDay() 
{ 
	return m_datetime.date().day();
}

int GameTime::getHours() 
{ 
	return m_datetime.time_of_day().hours();
}

int GameTime::getMinutes() 
{ 
	return m_datetime.time_of_day().minutes();
}

int GameTime::getSeconds() 
{ 
	return m_datetime.time_of_day().seconds();
}

int GameTime::getMilliseconds() 
{ 
	return m_datetime.time_of_day().total_milliseconds();
}

int GameTime::getSecondsSinceMidnight() 
{
	return getHours()*60*60+getMinutes()*60+getSeconds();
}