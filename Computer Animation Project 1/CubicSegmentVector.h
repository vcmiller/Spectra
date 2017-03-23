#pragma once

#include "CubicSegment.h"

class CubicSegmentVector {
public:
	CubicSegmentVector();
	CubicSegmentVector(float uStart, float uEnd, Vector3 start, Vector3 end, Vector3 tangentStart, Vector3 tangentEnd);

	Vector3 getPoint(float u);
private:
	CubicSegment x;
	CubicSegment y;
	CubicSegment z;
};