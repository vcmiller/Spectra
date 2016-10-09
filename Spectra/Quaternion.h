#pragma once

#include "Vector.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace spectra {
	// A mathematical structure used to represent 3D rotations.
	class Quaternion {
	public:
		// Construct a quaternion given components.
		// This will probably not be used often.
		Quaternion(float w, float x, float y, float z);
		
		// Construct identity quaternion.
		Quaternion();

		// Compute inverse quaternion.
		Quaternion inverse();

		// Combine the rotations of two quaternions.
		Quaternion operator*(const Quaternion &rhs);

		// Apply quaternion to vector.
		Vector3 operator*(const Vector3 &rhs);

		// Check for equality.
		bool operator==(const Quaternion &rhs);

		// Check for inequality.
		bool operator!=(const Quaternion &rhs);

		// Compute angle between two quaternions.
		static float angle(const Quaternion &q1, const Quaternion &q2);

		// Compute dot product of two quaternions.
		static float dot(const Quaternion &q1, const Quaternion &q2);

		// Construct quaternion given rotation angle and axis.
		static Quaternion angleAxis(float angle, const Vector3 &axis);

		// Construct quaternion given euler angles.
		static Quaternion euler(const Vector3 &euler);

		// Linear interpolation between two quaternions.
		static Quaternion lerp(const Quaternion &a, const Quaternion &b, float f);

		// Spherical interpolation between two quaternions.
		static Quaternion slerp(const Quaternion &a, const Quaternion &b, float f);

		// Rotate one quaternion towards another.
		// Similar to Quaternion::slerp, except maxDelta is an absolute amount.
		static Quaternion rotateTowards(const Quaternion &a, const Quaternion &b, float maxDelta);

		float &x = quat.x;
		float &y = quat.y;
		float &z = quat.z;
		float &w = quat.w;

	private:
		glm::quat quat;
	};
}