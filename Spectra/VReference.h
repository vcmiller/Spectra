#pragma once

namespace spectra {
	namespace internal {
		/// <summary> Class for managing a reference to a Vulkan object </summary>
		template <typename T> class VReference {
		public:

			/// <summary> Initialize with empty deletion function </summary>
			VReference() : VReference([](T, VkAllocationCallbacks*) {}) {}

			/// <summary> Initialize with deletion function that consumes only a pointer to the object and callbacks </summary>
			/// <param name="deletef"> The deletion function </param>
			VReference(std::function<void(T, VkAllocationCallbacks*)> deletef) {
				this->deleter = [=](T obj) { deletef(obj, nullptr); };
			}

			/// <summary> Initialize with deletion function that consumes a VkInstance, a pointer to the object and callbacks </summary>
			/// <param name="deletef"> The deletion function </param>
			VReference(const VReference<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef) {
				this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
			}

			/// <summary> Initialize with deletion function that consumes a VkDevice, a pointer to the object and callbacks </summary>
			/// <param name="deletef"> The deletion function </param>
			VReference(const VReference<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef) {
				this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
			}

			/// <summary> Call the given deletion function to cleanup the referenced Vulkan object </summary>
			~VReference() {
				cleanup();
			}

			/// <summary> Get a pointer to the Vulkan object itself, not this </summary>
			/// <returns> A pointer to the managed object </returns>
			const T* operator &() const {
				return &object;
			}

			/// <summary> Cleanup the old reference, get a pointer to the managed object location </summary>
			/// <returns> A pointer to the managed object </returns>
			T* replace() {
				cleanup();
				return &object;
			}

			/// <summary> Cast this as the managed object type </summary>
			/// <returns> The managed object </returns>
			operator T() const {
				return object;
			}

			/// <summary> Cleanup the old object, and replace it with a new one </summary>
			/// <param name="rhs"> The object to replace the old one with </param>
			void operator=(T rhs) {
				cleanup();
				object = rhs;
			}

			/// <summary> Compare managed object to another object </summary>
			/// <param name="rhs"> Object to compare reference with </param>
			/// <returns> Whether the objects compare equal </returns>
			template<typename V>
			bool operator==(V rhs) {
				return object == T(rhs);
			}

		private:
			/// <summary> The managed object </summary>
			T object{ VK_NULL_HANDLE };

			/// <summary> Function used to delete the managed object </summary>
			std::function<void(T)> deleter;

			/// <summary> Call the deletion function on the managed object, as long as the object is non-null </summary>
			void cleanup() {
				if (object != VK_NULL_HANDLE) {
					deleter(object);
				}
				object = VK_NULL_HANDLE;
			}

		};
	}
}