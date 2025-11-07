#pragma once

#include <typeindex>

namespace mco::objective {

    /// Base class for services.
	class Service {
	   protected:
		std::type_index typeIndex;

	   public:
		virtual ~Service() = default;
		std::type_index getTypeIndex() const {
			return typeIndex;
		}
	};

    /// Service mix-in. Inherit from this.
	template <class T>
	class ServiceMixin : public Service {
		ServiceMixin() {
            // Fill in the type index field in Service.
            // Techically there is a tradeoff here (a Service is 2 pointers large now),
            // however getTypeIndex() not being a virtual method should balance things out
			typeIndex = std::type_index(typeid(T));
		}
	};

} // namespace mco