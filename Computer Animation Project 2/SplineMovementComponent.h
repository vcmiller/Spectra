/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Component class that moves and rotates an object along a spline.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#pragma once

#include "Component.h"
#include "PathInterpolator.h"
#include "RotationInterpolator.h"

using namespace spectra;

class SplineMovementComponent : public Component {
public:
	// Initialize the component. Call right after constructing, before the first update() happens.
	void init(PathInterpolator *path, RotationInterpolator *rotation, float(*timeFunc)(float), bool play = true, bool loop = true);

	// Called every frame.
	virtual void update() override;

	// Whether playing or paused.
	bool playing;

	// Whether to loop when end is reached.
	bool loop;

	// Time position of the animation in seconds.
	float timePosition;

	// Time function. Maps normalized time (range [0, 1]) to normalized arc (also [0, 1]).
	float(*timeFunc)(float);

	// The interpolator for position.
	PathInterpolator *path;

	// The interpolator for rotation.
	RotationInterpolator *rotation;
};