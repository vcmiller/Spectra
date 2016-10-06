#pragma once
#include <math.h>
#include <ostream>

namespace spectra {
	namespace internal {
		template <char S, class T> class Vector {
		public:
			float &operator [] (int index) {
				return data[index];
			}

			float get(int index) const {
				return data[index];
			}

			static float dot(const T &v1, const T &v2) {
				float f = 0;

				for (int i = 0; i < S; i++) {
					f += v1[i] * v2[i];
				}

				return f;
			}

			static float angle(const T &v1, const T &v2) {
				float d = dot(v1, v2);

				return d;
			}

			float magnitude() const {
				return sqrt(sqrMagnitude());
			}

			float sqrMagnitude() const {
				return dot(*this, *this);
			}

			T normalized() const {
				return (*this) / magnitude();
			}

			T operator+(const T &rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = data[i] + rhs[i];
				}

				return result;
			}

			T operator-(const T &rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = data[i] - rhs[i];
				}

				return result;
			}

			T operator*(const T &rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = data[i] * rhs[i];
				}

				return result;
			}

			T operator*(float rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = data[i] * rhs;
				}

				return result;
			}

			T operator/(float rhs) const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = data[i] / rhs;
				}

				return result;
			}

			bool operator==(const T &rhs) const {
				bool result = true;

				for (int i = 0; i < S; i++) {
					result &= (data[i] == rhs[i]);
				}

				return result;
			}

			bool operator!=(const T &rhs) const {
				bool result = true;

				for (int i = 0; i < S; i++) {
					result &= (data[i] != rhs[i]);
				}

				return result;
			}

			T operator-() const {
				T result;

				for (int i = 0; i < S; i++) {
					result[i] = -data[i];
				}
			}

		protected:
			float data[S];

			Vector() { }
		};

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

	class Vector2 : public internal::Vector<2, Vector2> {
	public:
		Vector2(float x, float y);
		Vector2();

		float &x = data[0];
		float &y = data[1];
	};
	
	class Vector3 : public internal::Vector<3, Vector3> {
	public:
		Vector3(float x, float y, float z);
		Vector3();

		float &x = data[0];
		float &y = data[1];
		float &z = data[2];

		static Vector3 cross(const Vector3 &v1, const Vector3 &v2);
	};

	class Vector4 : public internal::Vector<4, Vector4> {
		Vector4(float x, float y, float z, float w);
		Vector4();

		float &x = data[0];
		float &y = data[1];
		float &z = data[2];
		float &w = data[3];
	};
}