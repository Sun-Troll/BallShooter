#include "CircF.h"

CircF::CircF(const Vec2& center_in, float radius_in)
	:
	center(center_in),
	radius(radius_in)
{
}

bool CircF::IsOverlapping(const CircF& circ) const
{
	const float OverlapDist = circ.radius + radius;
	return (circ.center - center).GetLengthSq() < OverlapDist * OverlapDist;
}
