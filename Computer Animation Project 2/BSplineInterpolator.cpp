/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of BSplineInterpolator.
*
* Date        : 2017/27/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#include "BSplineInterpolator.h"

BSplineInterpolator::BSplineInterpolator(Spline * spline) : PathInterpolator(spline) {
	// Constructor doesn't need to calculate any additional information.
	sampleCurve();
}

Vector3 BSplineInterpolator::getLocation(float u) {

	// Linear sum of basis for each segment.
	Vector3 sum = Vector3::zero;
	for (int i = 1; i < spline->getNumPoints() + 1; i++) {
		sum += spline->getPosition(i - 1) * basis(i, 4, u);
	}

	return sum;
}

float BSplineInterpolator::uknot(int index) {
	// Uniform knot vector, so simply divide index by length of the vector.
	return float(index - 1) / float(spline->getNumPoints() + 3);
}

float BSplineInterpolator::basis(int i, int k, float t) {
	if (k == 1) { // Base case
		if (uknot(i) <= t && t < uknot(i + 1)) {
			return 1;
		} else {
			return 0;
		}
	} else { // Recursive case
		float a1 = (t - uknot(i)) / (uknot(i + k - 1) - uknot(i));
		float a2 = (uknot(i + k) - t) / (uknot(i + k) - uknot(i + 1));

		return a1 * basis(i, k - 1, t) + a2 * basis(i + 1, k - 1, t);
	}
}
