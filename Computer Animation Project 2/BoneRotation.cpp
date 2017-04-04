/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of BoneRotation.
*
* Date        : 2017/04/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#include "BoneRotation.h"
#include "Time.h"

void BoneRotation::update() {
	transform.setLocalRotation(function(Time::time()));
}

void BoneRotation::init(Quaternion(*function)(float t)) {
	this->function = function;
}
