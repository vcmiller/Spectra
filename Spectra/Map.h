#pragma once

#include "List.h"
#include <exception>
#include <vector>

namespace spectra {
	template <typename K, typename V> class Map {
	public:
		Map(int initialCapacity = 32) : data(32) { }

		Map(const Map<K, V> &other) : data(other.data) { }

		const V &get(const K &key) const {
			for (int i = 0; i < data.size(); i++) {
				const Pair &pair = data[i];
				if (pair.key == key) {
					return pair.value;
				}
			}

			throw std::runtime_error("Key not found.");
		}

		V &operator[] (const K &key) {
			for (int i = 0; i < data.size(); i++) {
				Pair &pair = data[i];
				if (pair.key == key) {
					return pair.value;
				}
			}

			data.push_back(Pair(key));
			return data[data.size() - 1].value;
		}

		bool contains(const K &key) {
			for (int i = 0; i < data.size(); i++) {
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

		Pair* begin() {
			return &data[0];
		}

		Pair* end() {
			return &data[data.size()];
		}

	private:
		std::vector<Pair> data;
	};
}