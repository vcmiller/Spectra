/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Used to interpolate a 3-value point along a set of cubic functions (wrapper for CubicSegment).
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#pragma once

#include "CubicSegment.h"

class CubicSegmentVector {
public:
	// Default constructor.
	CubicSegmentVector();

	// Constructor. Find cubic functions to fit given points.
	CubicSegmentVector(float uStart, float uEnd, Vector3 start, Vector3 end, Vector3 tangentStart, Vector3 tangentEnd);

	// Get point along cubic function.
	// u: the parametric value of the function (not necessarily evenly spaced).
	Vector3 getPoint(float u);
private:
	// The parametric function for the x component.
	CubicSegment x;

	// The parametric function for the y component.
	CubicSegment y;

	// The parametric function for the z component.
	CubicSegment z;
};