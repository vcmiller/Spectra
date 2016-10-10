#include "Quaternion.h"

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

	Quaternion Quaternion::inverse() {
		Quaternion result;
		result.quat = glm::inverse(quat);
		return result;
	}

	Quaternion Quaternion::operator*(const Quaternion &rhs) {
		Quaternion result;
		result.quat = quat * rhs.quat;
		return result;
	}

	Vector3 Quaternion::operator*(const Vector3 &rhs) {
		glm::vec3 result = quat * glm::vec3(rhs.x, rhs.y, rhs.z);
		return Vector3(result.x, result.y, result.z);
	}

	bool Quaternion::operator==(const Quaternion &rhs) {
		return quat == rhs.quat;
	}

	bool Quaternion::operator!=(const Quaternion &rhs) {
		return quat != rhs.quat;
	}


	float Quaternion::angle(const Quaternion &q1, const Quaternion &q2) {
		return glm::angle(q1.quat * glm::inverse(q2.quat));
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
		result.quat = glm::quat(glm::vec3(euler.x, euler.y, euler.z));
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
			slerpAmount = angle / maxDelta;
		}
		
		return slerp(a, b, slerpAmount);
	}
}