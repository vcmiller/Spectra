#pragma once
#include <initializer_list>
#include <cstring>

#include "Math.h"
#include "List.h"

namespace spectra {
	template <class T> class ObjectSet {
	public:
		ObjectSet(int initialCapacity = 128) {
			items = new T*[initialCapacity];
			arraySize = initialCapacity;
			count = 0;
		}

		T** begin() {
			return items;
		}

		T** end() {
			return items + count;
		}

		T* operator[] (int index) const {
			return items[index];
		}

		T* get(int index) const {
			return items[index];
		}

		int length() const {
			return count;
		}

		void add(T* item) {
			if (count >= arraySize) {
				resize(arraySize * 2);
			}

			item->assignIndex(this, count);
			items[count++] = item;
		}

		int indexOf(T* item) {
			return item->getIndex(this);
		}

		bool remove(T* item) {
			int index = indexOf(item);
			if (index != -1) {
				remove(index);
				return true;
			} else {
				return false;
			}
		}

	private:
		T** items;
		int count;
		int arraySize;

		void resize(int newSize) {
			T** old = items;
			items = new T*[newSize];
			std::memcpy(items, old, arraySize * sizeof(T));
			arraySize = newSize;

			delete old;
		}

		T* remove(int index) {
			T* pop = items[index];
			T* sub = items[count - 1];
			sub->updateIndex(this, index);
			items[index] = sub;

			pop->removeIndex(this);
			count--;
			return pop;
		}

		ObjectSet(const ObjectSet<T> &other) {}
		void operator=(const ObjectSet<T> &other) {}
	};
}