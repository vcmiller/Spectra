#include "Matrix.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

namespace spectra {
	Matrix2::Matrix2() { }

	Matrix2::Matrix2(float r0c0, float r0c1, float r1c0, float r1c1) {
		set(0, 0, r0c0);
		set(0, 1, r0c1);

		set(1, 0, r1c0);
		set(1, 1, r1c1);
	}

	Matrix3::Matrix3() { }

	Matrix3::Matrix3(float r0c0, float r0c1, float r0c2, float r1c0, float r1c1, float r1c2, float r2c0, float r2c1, float r2c2) {
		set(0, 0, r0c0);
		set(0, 1, r0c1);
		set(0, 2, r0c2);

		set(1, 0, r1c0);
		set(1, 1, r1c1);
		set(1, 2, r1c2);

		set(2, 0, r2c0);
		set(2, 1, r2c1);
		set(2, 2, r2c2);
	}

	Matrix4::Matrix4() { }

	Matrix4::Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3) {
		set(0, 0, r0c0);
		set(0, 1, r0c1);
		set(0, 2, r0c2);
		set(0, 3, r0c3);

		set(1, 0, r1c0);
		set(1, 1, r1c1);
		set(1, 2, r1c2);
		set(1, 3, r1c3);

		set(2, 0, r2c0);
		set(2, 1, r2c1);
		set(2, 2, r2c2);
		set(2, 3, r2c3);

		set(3, 0, r3c0);
		set(3, 1, r3c1);
		set(3, 2, r3c2);
		set(3, 3, r3c3);
	}

	Matrix4::Matrix4(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {
		mat = glm::mat4(c0.x, c0.y, c0.z, c0.w, c1.x, c1.y, c1.z, c1.w, c2.x, c2.y, c2.z, c2.w, c3.x, c3.y, c3.z, c3.w);
	}

	Vector3 Matrix4::transformPoint(Vector3 point) const {
		Vector4 vec(point.x, point.y, point.z, 1.0f);
		vec = (*this) * vec;
		return Vector3(vec.x, vec.y, vec.z);
	}

	Vector3 Matrix4::transformVector(Vector3 vector) const {
		Vector4 vec(vector.x, vector.y, vector.z, 0.0f);
		vec = (*this) * vec;
		return Vector3(vec.x, vec.y, vec.z);
	}

	// Construct a translation matrix.
	Matrix4 Matrix4::translation(Vector3 vector) {
		Matrix4 mat;
		mat.mat = glm::translate(mat.mat, glm::vec3(vector.x, vector.y, vector.z));
		return mat;
	}

	// Construct an euler rotation matrix.
	Matrix4 Matrix4::euler(Vector3 euler) {
		Matrix4 mat;
		mat.mat = glm::rotate(mat.mat, euler.y, glm::vec3(0, 1, 0));
		mat.mat = glm::rotate(mat.mat, euler.x, glm::vec3(1, 0, 0));
		mat.mat = glm::rotate(mat.mat, euler.z, glm::vec3(0, 0, 1));
		return mat;
	}

	// Construct a scale matrix.
	Matrix4 Matrix4::scale(Vector3 scale) {
		Matrix4 mat;
		mat.mat = glm::scale(mat.mat, glm::vec3(scale.x, scale.y, scale.z));
		return mat;
	}

	// Construct rotation matrix.
	Matrix4 Matrix4::rotation(Quaternion quat) {
		Matrix4 mat;
		mat.mat = glm::toMat4(glm::quat(quat.w, quat.x, quat.y, quat.z));
		return mat;
	}

	// Construct perspective matrix.
	Matrix4 Matrix4::perspective(float fovy, float aspect, float near, float far) {
		Matrix4 mat;
		mat.mat = glm::perspective(fovy, aspect, near, far);
		return mat;
	}
}