/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : A general interpolator for position along a spline.
*               A subclass must implement the getLocation function to define the actual math function to use.
                Despite the name, this can actually be an interpolator or approximator.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#pragma once

#include "Spline.h"
#include "Vector.h"
#include "List.h"
#include "CubicSegmentVector.h"

// Number of samples to use for forward differencing.
#define NUM_SAMPLES 256

using namespace spectra;

class PathInterpolator {
public:
	// Constructor.
	PathInterpolator(Spline *spline);

	// Get a location along the curve.
	// u: parametric value. Need not be evenly spaced.
	virtual Vector3 getLocation(float u) = 0;
	
	// Get the desired time to traverse the spline.
	float getTimeLimit();

	// Convert an evenly spaced arc length to parametric value.
	// Sample curve must be called prior to this function (such as in the subclass constructor).
	float arcToU(float arc);

protected:
	// Reference spline.
	Spline *spline;

	// Construct table of arc / parametric values.
	// This is not called automatically in the constructor, as it must refer to a subclass function.
	void sampleCurve();

	// A single sample.
	struct Sample {
		// Parametric value.
		float u;

		// Arc length.
		float arc;
	};

	// Table of samples for forward differencing.
	Sample samples[NUM_SAMPLES];
};