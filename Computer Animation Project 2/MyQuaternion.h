/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of quaternions independent from any other library.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#pragma once
#include "Vector.h"

using namespace spectra;

class MyQuaternion {
public:
	// Constructor.
	MyQuaternion(Vector3 xyz = Vector3::zero, float w = 1.0f);

	// Construct quaternion given euler angles.
	static MyQuaternion euler(const Vector3 &angles);

	// Construct quaternion given angle and axis.
	static MyQuaternion rotate(const Vector3 &axis, float angle);

	// Get normalized (unit) quaternion.
	MyQuaternion normalized() const;

	// Add two quaternions.
	MyQuaternion operator + (const MyQuaternion &other) const;

	// Multiply (concatenate) two quaternions.
	MyQuaternion operator * (const MyQuaternion &other) const;

	// Multiply by scalar. Resulting quaternion represents same rotation.
	MyQuaternion operator * (float scalar) const;

	// Calculate inverse quaternion.
	MyQuaternion inverse() const;

	// Spherical interpolation between two quaternions.
	static MyQuaternion slerp(const MyQuaternion &a, const MyQuaternion &b, float alpha);

	// Dot product of two quaternions.
	static float dot(const MyQuaternion &a, const MyQuaternion &b);

	// Angle between two quaternions (acos of dot product).
	static float angle(const MyQuaternion &a, const MyQuaternion &b);

	// Vector component.
	Vector3 xyz;

	// Scalar component.
	float w;

};