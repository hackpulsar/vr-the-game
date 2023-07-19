#include "Math.h"

namespace math
{
	void clamp(const float min, const float max, float& value)
	{
		if (value < min)
			value = min;
		else if (value > max)
			value = max;
	}
}
