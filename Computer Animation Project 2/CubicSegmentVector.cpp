/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of CubicSegmentVector.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#include "CubicSegmentVector.h"

CubicSegmentVector::CubicSegmentVector() {}

CubicSegmentVector::CubicSegmentVector(float uStart, float uEnd, Vector3 start, Vector3 end, Vector3 tangentStart, Vector3 tangentEnd) {
	// Create individual curves.
	x = CubicSegment(uStart, uEnd, start.x, end.x, tangentStart.x, tangentEnd.x);
	y = CubicSegment(uStart, uEnd, start.y, end.y, tangentStart.y, tangentEnd.y);
	z = CubicSegment(uStart, uEnd, start.z, end.z, tangentStart.z, tangentEnd.z);
}

Vector3 CubicSegmentVector::getPoint(float u) {
	// Get point along all curves.
	return Vector3(x.getPoint(u), y.getPoint(u), z.getPoint(u));
}
