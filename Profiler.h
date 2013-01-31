#ifndef PROFILER_H
#define PROFILER_H

#include <string>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

class Profiler 
{
private:
	static Profiler *m_pInstance;
	std::string m_sLastSection;
	boost::unordered_map<std::string, int> m_hmSectionLastTimes;
	boost::unordered_map<std::string, int> m_hmSectionAverages;
	boost::unordered_map<std::string, int> m_hmSectionMinimum;
	boost::unordered_map<std::string, int> m_hmSectionMaximum;
	boost::unordered_map<std::string, int> m_hmSectionCounts;

	Profiler();

public:
	static Profiler *getInstance();
	void startProfile(std::string sSection);
	void endProfile();
	void logProfile();
};

#endif