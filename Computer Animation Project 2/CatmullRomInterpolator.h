/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of interpolator that uses a catmull-rom spline for strict interpolation.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#pragma once

#include "PathInterpolator.h"

class CatmullRomInterpolator : public PathInterpolator {
public:
	// Constructor
	CatmullRomInterpolator(Spline *spline);

	// Get a point along the curve.
	// u: the parametric value of the function (not necessarily evenly spaced).
	virtual Vector3 getLocation(float u) override;

	// Get a tangent at a given index.
	// Used for visualization.
	Vector3 getTangent(int index);

private:
	// Calculate a tangent at a given index, as long as that index is not an endpoint.
	Vector3 calcInternalTangent(int point);

	// Tangents, stored so they can be visualized.
	List <Vector3> tangents;

	// Each segment of the piecewise spline function.
	List<CubicSegmentVector> segments;
};