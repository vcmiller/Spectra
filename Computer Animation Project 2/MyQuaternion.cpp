/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of MyQuaternion.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#include "MyQuaternion.h"
#include "FMath.h"
#include "Log.h"

MyQuaternion::MyQuaternion(Vector3 xyz, float w) {
	this->xyz = xyz;
	this->w = w;
}

MyQuaternion MyQuaternion::euler(const Vector3 & angles) {
	// Rotate around z, then x, then y.
	return MyQuaternion::rotate(Vector3::up, angles.y) * MyQuaternion::rotate(Vector3::right, angles.x) * MyQuaternion::rotate(Vector3::forward, angles.z);
}

MyQuaternion MyQuaternion::rotate(const Vector3 & axis, float angle) {
	MyQuaternion ret;
	ret.w = FMath::cos(angle / 2.0f); // w = cos(angle / 2)
	ret.xyz = axis.normalized() * FMath::sin(angle / 2.0f); // xyz = axis * sin(angle / 2)
	return ret;
}

MyQuaternion MyQuaternion::normalized() const {
	float lengthSquared = xyz.sqrMagnitude() + w * w;
	float length = FMath::sqrt(lengthSquared);
	return *this * (1.0f / length);
}

MyQuaternion MyQuaternion::operator+(const MyQuaternion & other) const {
	return MyQuaternion(xyz + other.xyz, w + other.w);
}

MyQuaternion MyQuaternion::operator*(const MyQuaternion & other) const {
	MyQuaternion ret;

	// Quaternion multiplication formula from reading.
	ret.w = w * other.w - Vector3::dot(xyz, other.xyz);
	ret.xyz = other.xyz * w + xyz * other.w + Vector3::cross(xyz, other.xyz);

	return ret;
}

MyQuaternion MyQuaternion::operator*(float scalar) const {
	return MyQuaternion(xyz * scalar, w * scalar);
}

MyQuaternion MyQuaternion::inverse() const {
	return MyQuaternion(xyz, -w);
}

MyQuaternion MyQuaternion::slerp(const MyQuaternion & a, const MyQuaternion & b, float alpha) {
	MyQuaternion b2 = b;

	// Rotate along the shortest path.
	if (MyQuaternion::dot(a, b) < 0) {
		b2 = b2 * -1;
	}

	float ang = angle(a, b2);

	// Slerp function will return NaN for angle=0, so check that as a special case.
	if (ang == 0) {
		return a;
	}

	// Make sure both are unit quaternions.
	MyQuaternion an = a.normalized();
	MyQuaternion bn = b2.normalized();

	// Quaternion slerp function from reading.
	MyQuaternion q = (an * (FMath::sin(ang * (1 - alpha)) / FMath::sin(ang))) + (b2 * (FMath::sin(ang * alpha) / FMath::sin(ang)));
	
	// Normalize result.
	q = q.normalized();
	return q;
}

float MyQuaternion::dot(const MyQuaternion & a, const MyQuaternion & b) {
	return Vector3::dot(a.xyz, b.xyz) + a.w * b.w;
}

float MyQuaternion::angle(const MyQuaternion & a, const MyQuaternion & b) {
	return FMath::acos(dot(a.normalized(), b.normalized()));
}
