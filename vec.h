#include "vector.h"

struct SVector
{
	float x;
	float y;
	float z;
	SVector(float x1, float y1, float z1)
    {
		x = x1;
		y = y1;
		z = z1;
	}

	SVector(QAngle q)
    {
		x = q.x;
		y = q.y;
		z = q.z;
	}
};