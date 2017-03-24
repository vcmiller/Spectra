/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Used to interpolate a single value along a cubic function.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#pragma once

#include "Vector.h"

using namespace spectra;

class CubicSegment {
public:
	// Default constructor.
	CubicSegment();

	// Constructor. Find cubic function to fit given points.
	CubicSegment(float uStart, float uEnd, float start, float end, float tangentStart, float tangentEnd);

	// Get point along cubic function.
	// u: the parametric value of the function (not necessarily evenly spaced).
	float getPoint(float u);

private:
	// First coefficient (a * x^3)
	float a;

	// Second coefficient (b * x^2)
	float b;

	// Third coefficient (c * x)
	float c;

	// Fourth coefficient (a)
	float d;
};