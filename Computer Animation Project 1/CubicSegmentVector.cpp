#include "CubicSegmentVector.h"

CubicSegmentVector::CubicSegmentVector() {}

CubicSegmentVector::CubicSegmentVector(float uStart, float uEnd, Vector3 start, Vector3 end, Vector3 tangentStart, Vector3 tangentEnd) {
	x = CubicSegment(uStart, uEnd, start.x, end.x, tangentStart.x, tangentEnd.x);
	y = CubicSegment(uStart, uEnd, start.y, end.y, tangentStart.y, tangentEnd.y);
	z = CubicSegment(uStart, uEnd, start.z, end.z, tangentStart.z, tangentEnd.z);
}


Vector3 CubicSegmentVector::getPoint(float u) {
	return Vector3(x.getPoint(u), y.getPoint(u), z.getPoint(u));
}
