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

		List(const List<T>& other) {
			items = new T[other.arraySize];
			std::memcpy(this->items, other.items, other.length() * sizeof(T));
			arraySize = other.arraySize;
			count = other.count;
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

		int length() const {
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
			if (index != -1) {
				remove(index);
				return true;
			} else {
				return false;
			}
		}

		bool removeWhere(bool (*pred) (T)) {
			int index = -1;
			for (int i = 0; i < count; i++) {
				if (pred(items[i])) {
					index = i;
					break;
				}
			}

			if (index >= 0) {
				removeItem(index);
				return true;
			} else {
				return false;
			}
		}

		T remove(int index) {
			T pop = items[index];
			if (index < count - 1 && index < arraySize - 1) {
				std::memmove(items + index, items + index + 1, (count - index - 1) * sizeof(T));
			}
			count--;
			return pop;
		}

		void clear() {
			count = 0;
		}

		void resize(int newSize) {
			T* old = items;
			items = new T[newSize];
			std::memcpy(items, old, arraySize * sizeof(T));
			arraySize = newSize;

			delete old;
		}

	private:
		T* items;
		int count;
		int arraySize;
	};
}