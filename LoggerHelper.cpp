#include "LoggerHelper.h"
#include "TimeManager.h"
#include <direct.h>

LoggerHelper::LoggerHelper(std::string sFileName, std::string sLoggingType) 
{
	m_sFileName = std::string(sFileName);
	m_sLoggingType = std::string(sLoggingType);
	m_nYear = 0;
}

std::string LoggerHelper::getLogDir() 
{
	char cPath[100];
	sprintf(cPath, "Logs/%i/", m_nYear);
	_mkdir(cPath);
	sprintf(cPath, "Logs/%i/%i/", m_nYear, m_nMonth);
	_mkdir(cPath);
	sprintf(cPath, "Logs/%i/%i/%i/", m_nYear, m_nMonth, m_nDay);
	_mkdir(cPath);
	return std::string(cPath);
}

void LoggerHelper::commonLogError(std::string sMessage, std::string sMessageType) 
{
	if (m_nYear == 0) 
	{
		m_nYear = TimeManager::getInstance()->getGameTime().getYear();
		m_nMonth = TimeManager::getInstance()->getGameTime().getMonth();
		m_nDay = TimeManager::getInstance()->getGameTime().getDay();
	}
	int timestamp = TimeManager::getInstance()->getTimeStamp();
	std::string sPath = getLogDir();
	char cFileName[200];
	sprintf(cFileName, "%s%s_%i.log", sPath.c_str(), m_sFileName.c_str(), timestamp);
	std::ofstream logfile;
	logfile.open(cFileName, std::ios::out | std::ios::app);
	char cMessage[500];
	sprintf(cMessage, "%s [%s] [%s]: %s\n", TimeManager::getInstance()->toString().c_str(), m_sLoggingType.c_str(), sMessageType.c_str(), sMessage.c_str());
	if (logfile.is_open())
	{
		logfile.seekp(0, std::ios::end);
		logfile << cMessage;
		logfile.close();
	}
	std::cout << cMessage;
}

void LoggerHelper::info(std::string sMessage) 
{
	commonLogError(sMessage, "INFO");
}

void LoggerHelper::warn(std::string sMessage) 
{
	commonLogError(sMessage, "WARNING");
}

void LoggerHelper::error(std::string sMessage) 
{
	commonLogError(sMessage, "ERROR");
}