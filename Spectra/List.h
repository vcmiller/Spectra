#pragma once
#include <initializer_list>
#include <cstring>

#include "Math.h"

namespace spectra {
	template <typename T> class List {
	public:
		List(int initialCapacity = 32) {
			items = new T[initialCapacity];
			arraySize = initialCapacity;
			count = 0;
		}

		List(std::initializer_list<T> items) {
			items = new T[items.size];
			std::memcpy(contents, items.begin(), items.size * sizeof(T));
			arraySize = items.size;
			count = items.size;
		}

		T* begin() {
			return items;
		}

		T* end() {
			return items + count;
		}

		T& operator[] (int index) {
			return items[index];
		}

		T get(int index) const {
			return items[index];
		}

		int length() {
			return count;
		}

		void add(T item) {
			if (count >= arraySize) {
				resize(arraySize * 2);
			}

			items[count++] = item;
		}

		void append(const List<T> &other) {
			if (count + other.count > arraySize) {
				resize(count + other.count);
			}

			std::memcpy(items + count, other.items, other.count * sizeof(T));
			count += other.count;
		}

		int indexOf(T item) {
			for (int i = 0; i < count; i++) {
				if (items[i] == item) {
					return i;
				}
			}

			return -1;
		}

		bool removeItem(T item) {
			int index = indexOf(item);
			if (i != -1) {
				remove(i);
				return true;
			} else {
				return false;
			}
		}

		T remove(int index) {
			T pop = items[index];
			std::memmove(items + index, items + index + 1, (count - index - 1) * sizeof(T));
			count--;
			return T;
		}

	private:
		T* items;
		int count;
		int arraySize;

		void resize(int newSize) {
			T* old = items;
			items = new T[newSize];
			std::memcpy(items, old, arraySize * sizeof(T));
			arraySize = newSize;

			delete old;
		}
	};
}