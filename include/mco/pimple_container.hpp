#pragma once
#include "base_types.hpp"

namespace mco {
	/// A simple container for type-erased pimpl
	/// without requiring heap allocations.
	template <usize MaxSize>
	struct PimpleContainer {
		template <class T, class... Args>
		void constructType(Args&&... args) {
			static_assert(sizeof(T) <= MaxSize, "Type is too large to fit!");
			new(&data[0]) T(static_cast<Args&&>(args)...);
		}

		template <class T>
		T& getType() {
			return *reinterpret_cast<T*>(&data[0]);
		}

        template<class T>
        void destructType() {
            getType<T>().~T();
        }

		u8 data[MaxSize];
	};
} // namespace mco