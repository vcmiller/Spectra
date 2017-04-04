#include "Quaternion.h"
#include "Log.h"
#include "Math.h"
#include "Time.h"
#include "FMath.h"

namespace spectra {
	Quaternion::Quaternion(float w, float x, float y, float z) {
		quat = glm::quat(w, x, y, z);
	}

	Quaternion::Quaternion() {
		quat = glm::quat();
	}

	Quaternion::Quaternion(const Quaternion &q) {
		quat = q.quat;
	}

	void Quaternion::operator=(const Quaternion &q) {
		quat = q.quat;
	}

	Quaternion Quaternion::inverse() const {
		Quaternion result;
		result.quat = glm::inverse(quat);
		return result;
	}

	Quaternion Quaternion::normalized() const {
		Quaternion result;
		result.quat = glm::normalize(quat);
		return result;
	}

	Quaternion Quaternion::operator*(const Quaternion &rhs) const {
		Quaternion result;
		result.quat = quat * rhs.quat;
		return result;
	}

	Vector3 Quaternion::operator*(const Vector3 &rhs) const {
		glm::vec3 result = quat * glm::vec3(rhs.x, rhs.y, rhs.z);
		return Vector3(result.x, result.y, result.z);
	}

	void Quaternion::operator *= (const Quaternion &rhs) {
		quat *= rhs.quat;
	}

	bool Quaternion::operator==(const Quaternion &rhs) const {
		return quat == rhs.quat;
	}

	bool Quaternion::operator!=(const Quaternion &rhs) const {
		return quat != rhs.quat;
	}


	float Quaternion::angle(const Quaternion &q1, const Quaternion &q2) {
		float f = FMath::acos((q2.quat * glm::inverse(q1.quat)).w) * 2;

		if (f > FMath::halfCircle) {
			f -= FMath::fullCircle;
			f = FMath::abs(f);
		}

		return f;
	}

	float Quaternion::dot(const Quaternion &q1, const Quaternion &q2) {
		return glm::dot(q1.quat, q2.quat);
	}

	Quaternion Quaternion::angleAxis(float angle, const Vector3 &axis) {
		Quaternion result;
		result.quat = glm::angleAxis(angle, glm::vec3(axis.x, axis.y, axis.z));
		return result;
	}

	Quaternion Quaternion::euler(const Vector3 &euler) {
		Quaternion result;

		result.quat = glm::rotate(result.quat, euler.y, glm::vec3(0, 1, 0));
		result.quat = glm::rotate(result.quat, euler.x, glm::vec3(1, 0, 0));
		result.quat = glm::rotate(result.quat, euler.z, glm::vec3(0, 0, 1));
		return result;
	}

	Quaternion Quaternion::lerp(const Quaternion &a, const Quaternion &b, float f) {
		Quaternion result;
		result.quat = glm::lerp(a.quat, b.quat, f);
		return result;
	}

	Quaternion Quaternion::slerp(const Quaternion &a, const Quaternion &b, float f) {
		Quaternion result;
		result.quat = glm::slerp(a.quat, b.quat, f);
		return result;
	}

	Quaternion Quaternion::rotateTowards(const Quaternion &a, const Quaternion &b, float maxDelta) {
		float angle = Quaternion::angle(a, b);
		float slerpAmount = 1.0f;
		if (angle > maxDelta) {
			slerpAmount = maxDelta / angle;
		}
		
		return slerp(a, b, slerpAmount);
	}

	Quaternion Quaternion::fromToRotation(const Vector3 & v1, const Vector3 & v2) {
		Vector3 v1c = v1.normalized();
		Vector3 v2c = v2.normalized();

		Vector3 axis = Vector3::cross(v1c, v2c).normalized();
		float angle = Vector3::angle(v1c, v2c);

		return angleAxis(angle, axis);
	}
	Quaternion Quaternion::lookRotation(const Vector3 & direction, const Vector3 & up) {
		Vector3 right = Vector3::cross(direction, up).normalized();
		Vector3 trueUp = Vector3::cross(right, direction).normalized();
		Vector3 forward = direction.normalized();

		Quaternion ret;
		ret.w = FMath::sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
		float recip = 1.0f / (4.0f * ret.w);
		ret.x = (forward.y - trueUp.z) * recip;
		ret.y = (right.z - forward.x) * recip;
		ret.z = (trueUp.x - right.y) * recip;

		return ret;
	}
}