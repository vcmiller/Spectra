#include "MyQuaternion.h"
#include "FMath.h"

MyQuaternion::MyQuaternion(Vector3 xyz, float w) {
	this->xyz = xyz;
	this->w = w;
}

MyQuaternion MyQuaternion::euler(const Vector3 & angles) {
	MyQuaternion::rotate(Vector3::up, angles.y) * MyQuaternion::rotate(Vector3::right, angles.x) * MyQuaternion::rotate(Vector3::forward, angles.z);
}

MyQuaternion MyQuaternion::rotate(const Vector3 & axis, float angle) {
	MyQuaternion ret;
	ret.w = FMath::cos(angle / 2.0f);
	ret.xyz = axis.normalized() * FMath::sin(angle / 2.0f);
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
	float ang = angle(a, b);

	return (a * (FMath::sin(ang * (1 - alpha)) / FMath::sin(ang))) + (b * (FMath::sin(ang * alpha) / FMath::sin(ang)));
}

float MyQuaternion::dot(const MyQuaternion & a, const MyQuaternion & b) {
	return Vector3::dot(a.xyz, b.xyz) + a.w * b.w;
}

float MyQuaternion::angle(const MyQuaternion & a, const MyQuaternion & b) {
	return FMath::acos(dot(a.normalized(), b.normalized()));
}
