#ifndef MATHUTILS_H
#define MATHUTILS_H

namespace MathUtils 
{
	static float clamp(float nNum, float nMin, float nMax)
	{
		return nNum < nMin ? nMin : (nNum > nMax ? nMax : nNum);
	}
}

#endif