#pragma once
#include <math.h>
#include <ostream>

namespace spectra {
	namespace internal {
		// A generic Vector structure, with components stored in a fixed array.
		// First type parameter is number of components.
		// Second is the subclass returned by vector operations.
		template <char S, class T> class Vector {
		public:
			// Default constructor, no initialization
			Vector() {}

			// Get a reference to the component at the given index.
			float &operator [] (int index) {
				return data[index];
			}

			// Get the value of the component at the given index.
			float get(int index) const {
				return data[index];
			}

			// Compute the dot product of two vectors.
			static float dot(const T &v1, const T &v2) {
				float f = 0;

				for (int i = 0; i < S; i++) {
					f += v1[i] * v2[i];
				}

				return f;
			}

			// Compute the angle between two vectors, in radians.
			static float angle(const T &v1, const T &v2) {
				float d = dot(v1.normalized(), v2.normalized());

				return acos(d);
			}

			// Compute the magnitude of the vector.
			// As this is a fairly expensive operation, it is recommended to use sqrMagnitude() if possible.
			float magnitude() const {
				return sqrt(sqrMagnitude());
			}

			// Compute the square magnitude of the vector.
			// This is far less expensive than magnitude(), as it doesn't require a square root computation.
			float sqrMagnitude() const {
				return dot(*this, *this);
			}

			// Return a normalized version of this vector (length 1, same direction).
			T normalized() const {
				return (*this) / magnitude();
			}

			// Compute the sum of two vectors.
			T operator+(const T &rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = get(i) + rhs.get(i);
				}

				return result;
			}

			// Compute the difference of two vectors.
			T operator-(const T &rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = get(i) - rhs.get(i);
				}

				return result;
			}

			// Multiply the components of this vector by those of another.
			// This is not technically a valid vector operation, but is often helpful for scaling.
			T operator*(const T &rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = get(i) * rhs.get(i);
				}

				return result;
			}

			// Compute the product of this vector and a scalar.
			T operator*(float rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = get(i) * rhs;
				}

				return result;
			}

			// Compute the result of dividing this vector by a scalar.
			T operator/(float rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = get(i) / rhs;
				}

				return result;
			}

			// Determine if two vectors are equal.
			bool operator==(const T &rhs) const {
				bool result = true;

				for (int i = 0; i < S; i++) {
					result &= (get(i) == rhs.get(i));
				}

				return result;
			}

			// Determine if two vectors are unequal.
			bool operator!=(const T &rhs) const {
				bool result = true;

				for (int i = 0; i < S; i++) {
					result &= (get(i) != rhs.get(i));
				}

				return result;
			}

			// Return the negation of this vector.
			T operator-() const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = -get(i);
				}
			}

			// In-place addition.
			void operator+=(const T &rhs) {
				for (int i = 0; i < S; i++) {
					data[i] += rhs.get(i);
				}
			}

			// In-place subtraction.
			void operator-=(const T &rhs) {
				for (int i = 0; i < S; i++) {
					data[i] -= rhs.get(i);
				}
			}

			// In-place component multiplication.
			void operator*=(const T &rhs) {
				for (int i = 0; i < S; i++) {
					data[i] *= rhs.get(i);
				}
			}

			// In-place scalar multiplication.
			void operator*=(float f) {
				for (int i = 0; i < S; i++) {
					data[i] *= f;
				}
			}

			// In-place scalar division.
			void operator/=(float f) {
				for (int i = 0; i < S; i++) {
					data[i] /= f;
				}
			}

			// Implicitly cast to subtype.
			operator T() const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = get(i);
				}

				return result;
			}

		protected:
			// Components of this vector.
			float data[S];
		};

		// Output a vector to a stream.
		template<char S, typename T> std::ostream &operator<<(std::ostream &stream, const Vector<S, T> &vec) {
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
	class Vector2 : public internal::Vector<2, Vector2> {
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
		float &x = data[0];

		// Direct access to y component.
		float &y = data[1];
	};
	
	// A three-component vector.
	class Vector3 : public internal::Vector<3, Vector3> {
	public:
		// Initialize x, y, and z.
		Vector3(float x, float y, float z);

		// Initialize x, y, and z to zero.
		Vector3();

		// Copy constructor.
		Vector3(const Vector3 &other);

		// Assignment operator.
		Vector3& operator=(const Vector3 &other);

		// Direct access to x component.
		float &x = data[0];

		// Direct access to y component.
		float &y = data[1];

		// Direct access to z component.
		float &z = data[2];

		// Compute cross product of two vectors.
		// This operation only makes sense for three-component vectors.
		static Vector3 cross(const Vector3 &v1, const Vector3 &v2);
	};

	// A four-component vector.
	class Vector4 : public internal::Vector<4, Vector4> {
		// Initialize x, y, z, and w.
		Vector4(float x, float y, float z, float w);

		// Initialize x, y, z, and w to zero.
		Vector4();

		// Copy constructor.
		Vector4(const Vector4 &other);

		// Assignment operator.
		Vector4& operator=(const Vector4 &other);

		// Direct access to x component.
		float &x = data[0];

		// Direct access to y component.
		float &y = data[1];

		// Direct access to z component.
		float &z = data[2];

		// Direct access to w component.
		float &w = data[3];
	};
}