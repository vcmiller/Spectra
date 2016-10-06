#include "Vector.h"
#include <iostream>

namespace spectra {
	Vector2::Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector2::Vector2() : Vector2(0, 0) { }

	Vector3::Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3::Vector3() : Vector3(0, 0, 0) { }

	Vector3 Vector3::cross(const Vector3 &v1, const Vector3 &v2) {
		float nx = v1.y * v2.z - v1.z * v2.y;
		float ny = v1.z * v2.x - v1.x * v2.z;
		float nz = v1.x * v2.y - v1.y * v2.x;

		return Vector3(nx, ny, nz);
	}

	Vector4::Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4::Vector4() : Vector4(0, 0, 0, 0) {}
}