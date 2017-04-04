/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of SplineMovementComponent.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#include "SplineMovementComponent.h"
#include "Time.h"
#include "FMath.h"
#include "Quaternion.h"

void SplineMovementComponent::init(PathInterpolator * path, RotationInterpolator * rotation, float(*timeFunc)(float), bool play, bool loop) {
	this->path = path;
	this->rotation = rotation;
	this->timeFunc = timeFunc;
	this->playing = play;
	this->loop = loop;
	this->timePosition = 0.0f;
}

void SplineMovementComponent::update() {
	// Advance position.
	if (playing) {
		timePosition += Time::delta();
	}

	// Wrap around.
	if (loop) {
		timePosition = FMath::fmod(timePosition, path->getTimeLimit());
	}

	// Map time position into [0, 1]
	float tNorm = timePosition / path->getTimeLimit();

	//float a = timeFunc(tNorm); // Time to arc
	//float u = path->arcToU(a); // Arc to param
	Vector3 sPos = path->getLocation(tNorm); // Get location
	MyQuaternion q = rotation->getRotation(tNorm); // Get rotation
	
	transform.setPosition(sPos);
	transform.setRotation(Quaternion(q.w, q.xyz.x, q.xyz.y, q.xyz.z)); // Convert to quaternion class used by engine.
}
