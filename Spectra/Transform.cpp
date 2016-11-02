#include "Transform.h"
#include "Quaternion.h"

namespace spectra {

	Transform::Transform() { 
		position = Vector3();
		rotation = Quaternion();
		scale = Vector3(1, 1, 1);
		parent = nullptr;
	}

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
		if (relative == Space::Local) {
			position += rotation * delta;
		} else if (relative == Space::Parent || !parent) {
			position += delta;
		} else {
			position += getParentInverseRotation() * delta;
		}
	}

	void Transform::rotate(Quaternion delta, Space relative) {
		if (relative == Space::Local) {
			rotation *= delta;
		} else if (relative == Space::Parent || !parent) {
			rotation = delta * rotation;
		} else {
			rotation = delta * getParentInverseRotation() * rotation;
		}

		rotation = delta * rotation;
	}

	void Transform::rotate(Vector3 euler, Space relative) {
		rotate(Quaternion::euler(euler), relative);
	}

	void Transform::rotate(Vector3 axis, float angle, Space relative) {
		rotate(Quaternion::angleAxis(angle, axis), relative);
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
		Vector3 cur = getForward();
		rotation *= Quaternion::fromToRotation(cur, vec);
	}

	void Transform::setUp(Vector3 vec) {
		Vector3 cur = getUp();
		rotation *= Quaternion::fromToRotation(cur, vec);
	}

	void Transform::setRight(Vector3 vec) {
		Vector3 cur = getRight();
		rotation *= Quaternion::fromToRotation(cur, vec);
	}

	Matrix4 Transform::localToWorldMatrix() const {
		Matrix4 l2p = localToParentMatrix();
		if (parent) {
			return parentToWorldMatrix() * l2p;
		} else {
			return l2p;
		}
	}

	Matrix4 Transform::worldToLocalMatrix() const {
		Matrix4 p2l = parentToLocalMatrix();
		if (parent) {
			return p2l * worldToParentMatrix();
		} else {
			return p2l;
		}
	}

	Matrix4 Transform::parentToLocalMatrix() const {
		Vector4 right(rotation * Vector3::right / scale.x, 0);
		Vector4 up(rotation * Vector3::up / scale.y, 0);
		Vector4 forward(rotation * Vector3::forward / scale.z, 0);
		return Matrix4::transpose(right, up, forward, Vector4(0, 0, 0, 1)).translate(-position);
	}

	Matrix4 Transform::localToParentMatrix() const {
		Vector4 right(rotation * Vector3::right * scale.x, 0);
		Vector4 up(rotation * Vector3::up * scale.y, 0);
		Vector4 forward(rotation * Vector3::forward * scale.z, 0);
		Vector4 pos(position, 1);
		return Matrix4(right, up, forward, pos);
	}

	Matrix4 Transform::parentToWorldMatrix() const {
		return parent->localToWorldMatrix();
	}

	Matrix4 Transform::worldToParentMatrix() const {
		return parent->worldToLocalMatrix();
	}

	Vector3 Transform::transformVector(Vector3 vec) const {
		return localToWorldMatrix().transformVector(vec);
	}

	Vector3 Transform::transformPoint(Vector3 point) const {
		return localToWorldMatrix().transformPoint(point);
	}

	Quaternion Transform::transformRotation(Quaternion quat) const {
		return getRotation() * quat;
	}

	Vector3 Transform::inverseTransformVector(Vector3 vec) const {
		return worldToLocalMatrix().transformVector(vec);
	}

	Vector3 Transform::inverseTransformPoint(Vector3 point) const {
		return worldToLocalMatrix().transformPoint(point);
	}

	Quaternion Transform::inverseTransformRotation(Quaternion quat) const {
		return getRotation().inverse() * quat;
	}

	int Transform::getChildCount() const {
		return children.length();
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