#ifndef  INLINEHELPERS_H   
#define  INLINEHELPERS_H 

#include <cstdlib>

inline int GetRandom(int lowerBoundary, int upperBoundary)
{
	int range = (upperBoundary - lowerBoundary + 1);

	return (rand() % range) + lowerBoundary;
}

inline float GetRandomPercentage()
{
	int percent = GetRandom(0, 100);

	return percent / 100.0f;
}

inline float GetPositiveOrNegative()
{
	int coinToss = rand() % 2;
	float result = (coinToss == 0) ? -1.0f : 1.0f; 
	return result;
}

inline float EaseInOutQuad(float time, float start, float change, float duration)
{
	time /= (duration / 2);

	if (time < 1.0f)
	{
		return ((change / 2.0f) * (time * time)) + start;
	}

	return -change / 2.0f * (((time - 2.0f) * (--time)) - 1.0f) + start;
}

#endif //   INLINEHELPERS_H 
