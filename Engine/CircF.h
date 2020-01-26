#pragma once

#include "Vec2.h"

class CircF
{
public:
	CircF(const Vec2& center_in, float radius_in);
	bool IsOverlapping(const CircF& circ) const;
private:
	Vec2 center;
	float radius;
};