#include "Transform.h"

namespace spectra {

	Transform::Transform() { }

	Transform::Transform(const Transform &other) { }

	void Transform::operator=(const Transform &other) { }

	Vector3 Transform::getPosition() const {
		if (parent) {
			return parentToWorldMatrix().transformPoint(position);
		} else {
			return position;
		}
	}

	void Transform::setPosition(Vector3 pos) {
		if (parent) {
			position = worldToParentMatrix().transformPoint(pos);
		} else {
			position = pos;
		}
	}

	Vector3 Transform::getLocalPosition() const {
		return position;
	}

	void Transform::setLocalPosition(Vector3 pos) {
		position = pos;
	}

	Quaternion Transform::getRotation() const {
		if (parent) {
			return getParentRotation() * rotation;
		} else {
			return rotation;
		}
	}

	void Transform::setRotation(Quaternion rot) {
		if (parent) {
			rotation = getParentInverseRotation() * rot;
		}
	}

	Quaternion Transform::getLocalRotation() const {

	}

	void Transform::setLocalRotation(Quaternion rot) {

	}

	Vector3 Transform::getLocalScale() const {

	}

	void Transform::setLocalScale(Vector3 scl) {

	}

	void Transform::translate(Vector3 delta, Space relative /*= Space::global*/) {

	}

	void Transform::rotate(Quaternion delta) {

	}

	void Transform::rotate(Vector3 euler, Space relative /*= Space::global*/) {

	}

	void Transform::rotate(Vector3 axis, float angle, Space relative /*= Space::global*/) {

	}

	void Transform::setParent(Transform* parent, bool keepWorld /*= true*/) {

	}

	Transform* Transform::getParent() const {

	}

	Transform* Transform::getRoot() const {

	}

	Vector3 Transform::getForward() const {

	}

	Vector3 Transform::getUp() const {

	}

	Vector3 Transform::getRight() const {

	}

	void Transform::setForward(Vector3 vec) {

	}

	void Transform::setUp(Vector3 vec) {

	}

	void Transform::setRight(Vector3 vec) {

	}

	Matrix4 Transform::localToWorldMatrix() const {

	}

	Matrix4 Transform::worldToLocalMatrix() const {

	}

	Matrix4 Transform::parentToWorldMatrix() const {

	}

	Matrix4 Transform::worldToParentMatrix() const {

	}

	Vector3 Transform::transformVector(Vector3 vec) const {

	}

	Vector3 Transform::transformPoint(Vector3 point) const {

	}

	Quaternion Transform::transformRotation(Quaternion quat) const {

	}

	Vector3 Transform::inverseTransformVector(Vector3 vec) const {

	}

	Vector3 Transform::inverseTransformPoint(Vector3 point) const {

	}

	Quaternion Transform::inverseTransformRotation(Quaternion quat) const {

	}

	int Transform::getChildCount() const {

	}

	Transform * Transform::getChild(int i) const {

	}

}