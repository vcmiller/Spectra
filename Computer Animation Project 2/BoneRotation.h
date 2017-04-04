/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Component that rotates an object in local space given a time->quaternion function.
*
* Date        : 2017/04/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#pragma once

#include "Component.h"
#include "Vector.h"

using namespace spectra;

class BoneRotation : public Component {
public:
	// Called every frame.
	void update() override;

	// Initialize.
	void init(Quaternion(*function)(float t));

	// Pointer to rotation function.
	Quaternion(*function)(float t);
};