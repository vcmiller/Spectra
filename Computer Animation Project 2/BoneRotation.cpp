#include "BoneRotation.h"
#include "Time.h"

void BoneRotation::update() {
	transform.setLocalRotation(function(Time::time()));
}

void BoneRotation::init(Quaternion(*function)(float t)) {
	this->function = function;
}
