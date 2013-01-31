#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <string>
#include "ConfigNames.h"
#include "Config.h"
#include "Matrix.h"
#include "MathUtils.h"

namespace TimeUtils 
{
	enum TimeOfDay
	{ 
		MORNING, 
		DAY, 
		EVENING, 
		NIGHT 
	};

	static std::string SKY_NEAR_SUN_COLORS[] = {
		"MORNING_SKY_NEAR_SUN_COLOR",
		"DAY_SKY_NEAR_SUN_COLOR",
		"EVENING_SKY_NEAR_SUN_COLOR",
		"NIGHT_SKY_NEAR_SUN_COLOR"
	};

	static std::string SKY_AWAY_SUN_COLORS[] = {
		"MORNING_SKY_AWAY_SUN_COLOR",
		"DAY_SKY_AWAY_SUN_COLOR",
		"EVENING_SKY_AWAY_SUN_COLOR",
		"NIGHT_SKY_AWAY_SUN_COLOR"
	};

	static std::string CLOUD_COLORS[] = {
		"MORNING_CLOUD_COLOR",
		"DAY_CLOUD_COLOR",
		"EVENING_CLOUD_COLOR",
		"NIGHT_CLOUD_COLOR"
	};


	static Vector3 getSkyNearSunColor(TimeOfDay time) 
	{
		std::string sColor = Config::getInstance()->getFieldString(SKY_NEAR_SUN_COLORS[time]);
		std::vector<std::string> strs;
		boost::split(strs, sColor, boost::is_any_of(" "));
		if (strs.size() >= 3) 
		{
			return Vector3(atoi(strs[0].c_str())/255.0f, atoi(strs[1].c_str())/255.0f, atoi(strs[2].c_str())/255.0f);
		} 
		else 
		{
			return Vector3(255, 0, 0);	// Default return red
		}
	}

	static Vector3 getSkyAwaySunColor(TimeOfDay time) 
	{
		std::string sColor = Config::getInstance()->getFieldString(SKY_AWAY_SUN_COLORS[time]);
		std::vector<std::string> strs;
		boost::split(strs, sColor, boost::is_any_of(" "));
		if (strs.size() >= 3) 
		{
			return Vector3(atoi(strs[0].c_str())/255.0f, atoi(strs[1].c_str())/255.0f, atoi(strs[2].c_str())/255.0f);
		} 
		else 
		{
			return Vector3(255, 0, 0);	// Default return red
		}
	}

	static Vector3 getCloudColor(TimeOfDay time) 
	{
		int nColor = Config::getInstance()->getFieldInt(CLOUD_COLORS[time]);
		return Vector3(nColor/255.0f, nColor/255.0f, nColor/255.0f);
	}

	static float getWeightForTimeOfDay(TimeOfDay time)
	{
		int nMorningHour = Config::getInstance()->getFieldInt(ConfigNames::MORNING_HOUR);
		int nEveningHour = Config::getInstance()->getFieldInt(ConfigNames::EVENING_HOUR);
		int nTransitionLength = Config::getInstance()->getFieldInt(ConfigNames::TRANSITION_LEGTH);
		int nCurrentHour = TimeManager::getInstance()->getGameTime().getHours();
		int nMorningSeconds = nMorningHour * 60 * 60;
		int nEveningSeconds = nEveningHour * 60 * 60;
		int nTransitionSeconds = nTransitionLength * 60 * 60;
		int nCurrentSeconds = TimeManager::getInstance()->getGameTime().getSecondsSinceMidnight();
		float nColorWeights[4] = {0, 0, 0, 0};
		if (nCurrentSeconds <= nMorningSeconds) 
		{
			nColorWeights[MORNING] = 1.0f - (float)(nMorningSeconds-nCurrentSeconds) / (float)nTransitionSeconds;
			nColorWeights[NIGHT] = (float)(nMorningSeconds-nCurrentSeconds) / (float)nTransitionSeconds;
		} 
		else if (nCurrentSeconds >= nEveningSeconds) 
		{
			nColorWeights[EVENING] = 1.0f - (float)(nCurrentSeconds - nEveningSeconds) / (float)nTransitionSeconds;
			nColorWeights[NIGHT] = (float)(nCurrentSeconds - nEveningSeconds) / (float)nTransitionSeconds;
		} 
		else if (nCurrentSeconds >= nMorningSeconds && nCurrentSeconds <= nEveningSeconds-nTransitionSeconds) 
		{
			nColorWeights[MORNING] = 1.0f - (float)(nCurrentSeconds - nMorningSeconds) / (float)nTransitionSeconds;
			nColorWeights[DAY] = (float)(nCurrentSeconds - nMorningSeconds) / (float)nTransitionSeconds;
		} 
		else 
		{
			nColorWeights[EVENING] = 1.0f - (float)(nEveningSeconds - nCurrentSeconds) / (float)nTransitionSeconds;
			nColorWeights[DAY] = (float)(nEveningSeconds - nCurrentSeconds) / (float)nTransitionSeconds;
		}
		nColorWeights[time] = sqrt(MathUtils::clamp(nColorWeights[time], 0, 1.0f));
		return nColorWeights[time];
	}

}

#endif