/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of interpolator that uses a b-spline for approximation.
*
* Date        : 2017/27/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#pragma once

#include "PathInterpolator.h"

class BSplineInterpolator : public PathInterpolator {
public:
	// Constructor
	BSplineInterpolator(Spline *spline);

	// Get a point along the curve.
	// u: the parametric value of the function (not necessarily evenly spaced).
	virtual Vector3 getLocation(float u) override;


private:
	// Knot vector. Since uniform, just use a function of index.
	float uknot(int index);
	
	// Recursive basis function.
	// i: index of the segment.
	// k: order of the spline (constant).
	// t: parametric value for the curve.
	float basis(int i, int k, float t);
};