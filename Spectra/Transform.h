#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "List.h"
#include "Space.h"

namespace spectra {
	class Transform {
	public:
		Transform();

		Vector3 getPosition() const;
		void setPosition(Vector3 pos);
		Vector3 getLocalPosition() const;
		void setLocalPosition(Vector3 pos);

		Quaternion getRotation() const;
		void setRotation(Quaternion rot);
		Quaternion getLocalRotation() const;
		void setLocalRotation(Quaternion rot);

		Vector3 getLocalScale() const;
		void setLocalScale(Vector3 scl);

		void translate(Vector3 delta, Space relative = Space::global);
		void rotate(Quaternion delta);
		void rotate(Vector3 euler, Space relative = Space::global);
		void rotate(Vector3 axis, float angle, Space relative = Space::global);

		void setParent(Transform* parent, bool keepWorld = true);
		Transform* getParent() const;
		Transform* getRoot() const;

		Vector3 getForward() const;
		Vector3 getUp() const;
		Vector3 getRight() const;

		void setForward(Vector3 vec);
		void setUp(Vector3 vec);
		void setRight(Vector3 vec);

		Matrix4 localToWorldMatrix() const;
		Matrix4 worldToLocalMatrix() const;

		Matrix4 parentToWorldMatrix() const;
		Matrix4 worldToParentMatrix() const;

		Vector3 transformVector(Vector3 vec) const;
		Vector3 transformPoint(Vector3 point) const;
		Quaternion transformRotation(Quaternion quat) const;

		Vector3 inverseTransformVector(Vector3 vec) const;
		Vector3 inverseTransformPoint(Vector3 point) const;
		Quaternion inverseTransformRotation(Quaternion quat) const;

		int getChildCount() const;
		Transform *getChild(int i) const;

		Quaternion getParentRotation() const;
		Quaternion getParentInverseRotation() const;

	private:
		// Don't allow copy.
		Transform(const Transform &other);

		// Don't allow assign.
		void operator=(const Transform &other);

		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		List<Transform*> children;
		Transform *parent;
	};
}