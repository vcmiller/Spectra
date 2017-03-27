/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Component class that swaps the interpolation function.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#pragma once

#include "Component.h"
#include "PathInterpolator.h"
#include "SplineMovementComponent.h"

using namespace spectra;

class SplineSwapComponent : public Component {
public:
	// Called when added to GameObject.
	void onCreate() override;

	// Called every frame.
	void update() override;

	// Call to initialize values.
	void init(PathInterpolator *alt);

private:
	// Alternate interpolation.
	PathInterpolator *alt;

	// Affected spline movement.
	SplineMovementComponent *spline;
};