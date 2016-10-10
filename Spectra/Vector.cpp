#include "Vector.h"
#include <iostream>
#include "Quaternion.h"
#include "Math.h"

namespace spectra {
	// ========================== Vector2 ========================== 
	Vector2::Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector2::Vector2() : Vector2(0, 0) { }

	Vector2::Vector2(const Vector2 &other) {
		x = other.x;
		y = other.y;
	}

	Vector2& Vector2::operator=(const Vector2 &other) {
		x = other.x;
		y = other.y;
	}

	// ========================== Vector3 ========================== 
	Vector3::Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3::Vector3() : Vector3(0, 0, 0) {}

	Vector3::Vector3(const Vector3 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	Vector3 &Vector3::operator=(const Vector3 &other) {
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	Vector3 Vector3::cross(const Vector3 &v1, const Vector3 &v2) {
		Vector3 result;
		result.vec = glm::cross(v1.vec, v2.vec);
		return result;
	}

	Vector3 Vector3::rotate(const Vector3 &vector, const Vector3 &axis, float angle) {
		return vector * Math::cos(angle) + cross(axis, vector) * Math::sin(angle) + axis * dot(vector, axis) * (1 - Math::cos(angle));
	}

	Vector3 Vector3::slerp(const Vector3 &v1, const Vector3 &v2, float amount) {
		Vector3 axis = cross(v1, v2).normalized();
		float angle = Vector3::angle(v1, v2);
		return rotate(v1, axis, angle * amount);
	}

	Vector3 Vector3::rotateTowards(const Vector3 &v1, const Vector3 &v2, float maxDeltaAngle, float maxDeltaLength) {
		Vector3 axis = cross(v1, v2).normalized();
		float angle = Vector3::angle(v1, v2);

		Vector3 result;

		if (angle <= maxDeltaAngle) {
			result = v2;
		} else {
			result = rotate(v1, axis, maxDeltaAngle);
		}

		if (maxDeltaLength != 0) {
			float l = v1.magnitude();
			float l2 = v2.magnitude();

			result = result / l * Math::moveTowards(l, l2, maxDeltaLength);
		}

		return result;
	}

	const Vector3 Vector3::right = Vector3(1, 0, 0);
	const Vector3 Vector3::left = Vector3(-1, 0, 0);
	const Vector3 Vector3::up = Vector3(0, 1, 0);
	const Vector3 Vector3::down = Vector3(0, -1, 0);
	const Vector3 Vector3::forward = Vector3(0, 0, 1);
	const Vector3 Vector3::back = Vector3(0, 0, -1);
	const Vector3 Vector3::zero = Vector3(0, 0, 0);

	// ========================== Vector4 ========================== 
	Vector4::Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4::Vector4() : Vector4(0, 0, 0, 0) {}

	Vector4::Vector4(const Vector4 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	Vector4& Vector4::operator=(const Vector4 &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}
}