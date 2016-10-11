#pragma once
#include <ostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace spectra {
	namespace internal {
		template <char S, class T, class V, class G> class Matrix;

		// A generic Vector structure, with components stored in a GLM vector.
		// First type parameter is number of components.
		// Second is the subclass returned by vector operations.
		// Third is base GLM type
		template <char S, class T, class G> class Vector {
		public:
			// Default constructor, no initialization
			Vector() {}

			// Get a reference to the component at the given index.
			float &operator [] (int index) {
				return vec[index];
			}

			// Get the value of the component at the given index.
			float get(int index) const {
				return vec[index];
			}

			// Compute the dot product of two vectors.
			static float dot(const T &v1, const T &v2) {
				return glm::dot(v1.vec, v2.vec);
			}

			// Compute the angle between two vectors, in radians.
			static float angle(const T &v1, const T &v2) {
				return glm::angle(glm::normalize(v1.vec), glm::normalize(v2.vec));
			}

			// Linear interoplation between two vectors.
			static T lerp(const T &v1, const T &v2, float f) {
				T result;
				result.vec = v1.vec * (1.0f - f) + v2.vec * f;
				return result;
			}

			// Move vector towards another vector by absolute distance.
			static T moveTowards(const T &v1, const T &v2, float maxDelta) {
				T result;
				float f = distance(v1, v2);

				if (f <= maxDelta) {
					return v2;
				} else {
					T dir = (v2 - v1) / f;

					result.vec = v1.vec + dir.vec * maxDelta;
					return result;
				}
			}

			// Compute the magnitude of the vector.
			// As this is a fairly expensive operation, it is recommended to use sqrMagnitude() if possible.
			float magnitude() const {
				return glm::length(vec);
			}

			// Compute the square magnitude of the vector.
			// This is far less expensive than magnitude(), as it doesn't require a square root computation.
			float sqrMagnitude() const {
				return glm::dot(vec, vec);
			}

			// Compute distance between vectors.
			static float distance(const T &v1, const T &v2) {
				return (v1 - v2).magnitude();
			}

			// Compute square distance between vectors.
			static float sqrDistance(const T &v1, const T &v2) {
				return (v1 - v2).sqrMagnitude();
			}

			// Return a normalized version of this vector (length 1, same direction).
			T normalized() const {
				return (*this) / magnitude();
			}

			// Compute the sum of two vectors.
			T operator+(const T &rhs) const {
				T result;
				result.vec = vec + rhs.vec;
				return result;
			}

			// Compute the difference of two vectors.
			T operator-(const T &rhs) const {
				T result;
				result.vec = vec - rhs.vec;
				return result;
			}

			// Multiply the components of this vector by those of another.
			// This is not technically a valid vector operation, but is often helpful for scaling.
			T operator*(const T &rhs) const {
				T result;
				result.vec = vec * rhs.vec;
				return result;
			}

			// Compute the product of this vector and a scalar.
			T operator*(float rhs) const {
				T result;
				result.vec = vec * rhs;
				return result;
			}

			// Compute the result of dividing this vector by a scalar.
			T operator/(float rhs) const {
				T result;
				result.vec = vec / rhs;
				return result;
			}

			// Determine if two vectors are equal.
			bool operator==(const T &rhs) const {
				return vec == rhs.vec;
			}

			// Determine if two vectors are unequal.
			bool operator!=(const T &rhs) const {
				return vec != rhs.vec;
			}

			// Return the negation of this vector.
			T operator-() const {
				T result;
				result.vec = -vec;
				return result;
			}

			// In-place addition.
			void operator+=(const T &rhs) {
				vec += rhs.vec;
			}

			// In-place subtraction.
			void operator-=(const T &rhs) {
				vec -= rhs.vec;
			}

			// In-place component multiplication.
			void operator*=(const T &rhs) {
				vec *= rhs.vec;
			}

			// In-place scalar multiplication.
			void operator*=(float f) {
				vec *= f;
			}

			// In-place scalar division.
			void operator/=(float f) {
				vec /= f;
			}

			// Implicitly cast to subtype.
			operator T() const {
				T result;
				result.vec = vec;
				return result;
			}

		protected:
			template <char S, class T, class V, class G> friend class Matrix;
			// Components of this vector.
			G vec;
		};

		// Output a vector to a stream.
		template<char S, class T, class G> std::ostream &operator<<(std::ostream &stream, const Vector<S, T, G> &vec) {
 			stream << "(";
			for (int i = 0; i < S; i++) {
				stream << vec.get(i);
				if (i < S - 1) {
					stream << ", ";
				} else {
					stream << ")";
				}
			}

			return stream;
		}
	}

	// A two-component vector.
	class Vector2 : public internal::Vector<2, Vector2, glm::vec2> {
	public:
		// Initialize x and y.
		Vector2(float x, float y);

		// Initialize x and y to zero.
		Vector2();

		// Copy constructor.
		Vector2(const Vector2 &other);

		// Assignment operator.
		Vector2& operator=(const Vector2 &other);

		// Direct access to x component.
		float &x = vec.x;

		// Direct access to y component.
		float &y = vec.y;
	};
	
	// A three-component vector.
	class Vector3 : public internal::Vector<3, Vector3, glm::vec3> {
	public:
		// Initialize x, y, and z.
		Vector3(float x, float y, float z);

		// Initialize x, y, and z to zero.
		Vector3();

		// Copy constructor.
		Vector3(const Vector3 &other);

		// Assignment operator.
		Vector3& operator=(const Vector3 &other);

		// Rotate vector around axis by angle, in radians.
		// Axis must be normalized, or this will produce unexpected results.
		static Vector3 rotate(const Vector3 &vector, const Vector3 &axis, float angle);

		// Spherical interpolation between two direction vectors by amount.
		static Vector3 slerp(const Vector3 &v1, const Vector3 &v2, float amount);

		// Rotate vector towards another vector by absolute amount, with optional length interpoplation.
		static Vector3 rotateTowards(const Vector3 &v1, const Vector3 &v2, float maxDeltaAngle, float maxDeltaLength = 0.0f);

		// Direct access to x component.
		float &x = vec.x;

		// Direct access to y component.
		float &y = vec.y;

		// Direct access to z component.
		float &z = vec.z;

		// Compute cross product of two vectors.
		// This operation only makes sense for three-component vectors.
		static Vector3 cross(const Vector3 &v1, const Vector3 &v2);

		const static Vector3 right;
		const static Vector3 left;
		const static Vector3 up;
		const static Vector3 down;
		const static Vector3 forward;
		const static Vector3 back;
		const static Vector3 zero;
	};

	// A four-component vector.
	class Vector4 : public internal::Vector<4, Vector4, glm::vec4> {
	public:
		// Initialize x, y, z, and w.
		Vector4(float x, float y, float z, float w);

		// Initialize x, y, z, and w to zero.
		Vector4();

		// Copy constructor.
		Vector4(const Vector4 &other);

		// Assignment operator.
		Vector4& operator=(const Vector4 &other);

		// Direct access to x component.
		float &x = vec.x;

		// Direct access to y component.
		float &y = vec.y;

		// Direct access to z component.
		float &z = vec.z;

		// Direct access to w component.
		float &w = vec.w;
	};
}