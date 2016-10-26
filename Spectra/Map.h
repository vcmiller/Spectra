#pragma once

#include "List.h"
#include <exception>

namespace spectra {
	template <typename K, typename V> class Map {
	public:
		Map(int initialCapacity = 32) : data(32) { }

		Map(const Map<K, V> &other) : data(other.data) { }

		const V &get(const K &key) const {
			for (int i = 0; i < data.length(); i++) {
				const Pair &pair = data.get(i);
				if (pair.key == key) {
					return pair.value;
				}
			}

			throw std::runtime_error("Key not found.");
		}

		V &operator[] (const K &key) {
			for (int i = 0; i < data.length(); i++) {
				Pair &pair = data[i];
				if (pair.key == key) {
					return pair.value;
				}
			}

			data.add(Pair(key));
			return data[data.length() - 1];
		}

		bool contains(const K &key) {
			for (int i = 0; i < data.length(); i++) {
				Pair &pair = data[i];
				if (pair.key == key) {
					return true;
				}
			}

			return false;
		}

		struct Pair {
			Pair() { }

			Pair(K key) {
				this->key = key;
			}

			K key;
			V value;
		};


	private:
		List<Pair> data;
	};
}