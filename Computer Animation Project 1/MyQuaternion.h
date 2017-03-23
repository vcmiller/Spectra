#pragma once
#include "Vector.h"

using namespace spectra;

class MyQuaternion {
public:
	MyQuaternion(Vector3 xyz = Vector3::zero, float w = 1.0f);

	static MyQuaternion euler(const Vector3 &angles);

	static MyQuaternion rotate(const Vector3 &axis, float angle);

	MyQuaternion normalized() const;


	MyQuaternion operator + (const MyQuaternion &other) const;
	MyQuaternion operator * (const MyQuaternion &other) const;
	MyQuaternion operator * (float scalar) const;

	MyQuaternion inverse() const;

	static MyQuaternion slerp(const MyQuaternion &a, const MyQuaternion &b, float alpha);
	static float dot(const MyQuaternion &a, const MyQuaternion &b);
	static float angle(const MyQuaternion &a, const MyQuaternion &b);

private:
	Vector3 xyz;
	float w;
};