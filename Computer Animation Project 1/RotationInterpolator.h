/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Used to interpolate quaternions along a spline.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#pragma once

#include "Spline.h"
#include "MyQuaternion.h"

class RotationInterpolator {
public:
	// Constructor.
	RotationInterpolator(Spline *spline);

	// Get rotation given parametric value.
	MyQuaternion getRotation(float u);

private:
	// List of quaternions calculated from the spline.
	List<MyQuaternion> rotations;
};