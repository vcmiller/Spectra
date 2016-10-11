#include "Transform.h"
#include "Quaternion.h"

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
		} else {
			rotation = rot;
		}
	}

	Quaternion Transform::getLocalRotation() const {
		return rotation;
	}

	void Transform::setLocalRotation(Quaternion rot) {
		rotation = rot;
	}

	Vector3 Transform::getLocalScale() const {
		return scale;
	}

	void Transform::setLocalScale(Vector3 scl) {
		scale = scl;
	}

	void Transform::translate(Vector3 delta, Space relative) {
		if (relative == Space::global) {
			setPosition(getPosition() + delta);
		} else {
			position += delta;
		}
	}

	void Transform::rotate(Quaternion delta) {
		rotation *= delta;
	}

	void Transform::rotate(Vector3 euler, Space relative) {
		Quaternion rot = Quaternion::euler(euler);

		if (relative == Space::global && parent) {
			rot = getParentInverseRotation() * rot;
		}

		rotation *= rot;
	}

	void Transform::rotate(Vector3 axis, float angle, Space relative) {
		if (parent) {
			axis = getParentInverseRotation() * axis;
		}

		rotation *= Quaternion::angleAxis(angle, axis);
	}

	void Transform::setParent(Transform* parent, bool keepWorld) {
		if (parent == this->parent) {
			return;
		}

		if (keepWorld) {
			Matrix4 mat;
			Quaternion rot;

			if (this->parent) {
				mat = this->parent->localToWorldMatrix();
				rot = this->parent->getRotation();
			}

			if (parent) {
				mat = parent->worldToLocalMatrix() * mat;
				rot = parent->getRotation().inverse() * rot;
			}

			position = mat.transformPoint(position);
			rotation = rot * rotation;
		}

		if (this->parent) {
			this->parent->children.removeItem(this);
		}

		if (parent) {
			parent->children.add(this);
		}

		this->parent = parent;
	}

	Transform* Transform::getParent() const {
		return parent;
	}

	Transform* Transform::getRoot() {
		if (parent) {
			return parent->getRoot();
		} else {
			return this;
		}
	}

	Vector3 Transform::getForward() const {
		return getRotation() * Vector3::forward;
	}

	Vector3 Transform::getUp() const {
		return getRotation() * Vector3::up;
	}

	Vector3 Transform::getRight() const {
		return getRotation() * Vector3::right;
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
		return children.get(i);
	}

	spectra::Quaternion Transform::getParentRotation() const {
		if (parent) {
			return parent->getRotation();
		} else {
			return Quaternion();
		}
	}

	spectra::Quaternion Transform::getParentInverseRotation() const {
		if (parent) {
			return parent->getRotation().inverse();
		} else {
			return Quaternion();
		}
	}

}