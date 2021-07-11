#include "vector.h"

struct vec2
{
	float x, y;

	vec2 operator-(vec2 ape)
	{
		return { x - ape.x, y - ape.y };
	}

	vec2 operator+(vec2 ape)
	{
		return { x + ape.x, y + ape.y };
	}

	vec2 operator*(float ape)
	{
		return { x * ape, y * ape };
	}

	vec2 operator/(float ape)
	{
		return { x / ape, y / ape };
	}

	vec2 operator/=(float ape)
	{
		x /= ape;
		y /= ape;

		return *this;
	}

	vec2 operator+=(vec2 ape)
	{
		return { x += ape.x, y += ape.y };
	}

	vec2 operator-=(vec2 ape)
	{
		return { x -= ape.x, y -= ape.y };
	}

	void Normalize()
	{
		if (x > 89.0f)
			x -= 180.f;

		if (x < -89.0f)
			x += 180.f;

		if (y > 180.f)
			y -= 360.f;

		if (y < -180.f)
			y += 360.f;
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float Dist2D(vec2 ape)
	{
		return (*this - ape).Length2D();
	}
};
