#ifndef LOGGER_H
#define LOGGER_H

#include "LoggerHelper.h"

static std::string LN_LOGGING_FILE = "nexus";
static std::string PROFILING_FILE  = "profiler";

namespace Logging 
{
	static LoggerHelper *GAME		= new LoggerHelper(LN_LOGGING_FILE, "GAME");
	static LoggerHelper *GRAPHICS	= new LoggerHelper(LN_LOGGING_FILE, "GRAPHICS");
	static LoggerHelper *RESOURCE	= new LoggerHelper(LN_LOGGING_FILE, "RESOURCE");
	static LoggerHelper *PHYSICS	= new LoggerHelper(LN_LOGGING_FILE, "PHYSICS");
	static LoggerHelper *PROFILER	= new LoggerHelper(PROFILING_FILE, "PROFILER");
};

#endif