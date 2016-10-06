#pragma once

namespace util {
	template <typename T> class ReadProperty {
	public:
		ReadProperty(T* container, T(*getter) (T)) {
			this->container = container;
			this->getter = getter;
		}

		operator const T&() const {
			return getter(*container);
		}

	private:
		ReadProperty(const ReadProperty<T> &) { }
		ReadProperty(ReadProperty<T> &&) {}

		void operator =(const ReadProperty &val) { }

		void *operator new   (size_t) { }
		void *operator new[](size_t) { }
		void operator delete[](void*) { }
		void operator delete   (void *) { }

	protected:
		T* container;
		T(*getter) (T);
	};

	template <typename T> class Property : public ReadProperty<T> {
	public:
		Property(T* container, T(*getter) (T), T(*setter) (T)) : ReadProperty(container, getter) {
			this->setter = setter;
		}

		void operator =(const T &val) {
			*container = setter(val);
		}

		void operator +=(const T &val) {
			*container = setter(*container + val);
		}

		void operator *=(const T &val) {
			*container = setter(*container * val);
		}

		void operator -=(const T &val) {
			*container = setter(*container - val);
		}

		void operator /=(const T &val) {
			*container = setter(*container / val);
		}

		void operator %=(const T &val) {
			*container = setter(*container % val);
		}

	private:
		Property(const Property<T> &) {}
		Property(Property<T> &&) {}

		void operator =(const Property &val) {}

		T(*setter) (T);
	};
}