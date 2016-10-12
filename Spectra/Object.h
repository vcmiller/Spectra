#pragma once
#include <string>

#include "ObjectSet.h"
#include "List.h"

namespace spectra {
	class Object {
	public:
		Object();

		std::string getName() const;

		void setName(std::string name);

		template <class T> void assignIndex(const ObjectSet<T> *set, int index) {
			for (SetPlacement s : sets) {
				if (s.set == set) {
					throw std::runtime_error("assignIndex called on Object " + name + ", which is already in the ObjectSet. Use updateIndex to update.");
				}
			}

			sets.add(SetPlacement((void*)set, index));
		}

		template <class T> void updateIndex(const ObjectSet<T> *set, int index) {
			for (SetPlacement &s : sets) {
				if (s.set == set) {
					s.index = index;
					break;
				}
			}
		}

		template <class T> void removeIndex(const ObjectSet<T> *set) {
			for (int i = 0; i < sets.length(); i++) {
				if (sets[i].set == set) {
					sets.remove(i);
					break;
				}
			}
		}

		template <class T> int getIndex(const ObjectSet<T> *set) const {
			for (int i = 0; i < sets.length(); i++) {
				if (sets.get(i).set == set) {
					return sets.get(i).index;
				}
			}

			return -1;
		}

	private:
		struct SetPlacement {
			SetPlacement(void* set, int index);
			SetPlacement();

			void* set;
			int index;
		};

		List<SetPlacement> sets = List<SetPlacement>(2);

		std::string name;

		int id;
	};

}
