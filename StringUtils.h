#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <boost/lexical_cast.hpp>

namespace StringUtils 
{
	static std::string valueOf(int nNumber) 
	{
		return boost::lexical_cast<std::string>(nNumber);
	}
};

#endif