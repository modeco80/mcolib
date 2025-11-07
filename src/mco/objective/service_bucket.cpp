#include <mco/objective/service_bucket.hpp>

namespace mco::objective {

	// This concrete code is used by the templates in ServiceBucket
    // to reduce the size of the code when using it.
    // Some other decisions have been made based on this.

	Ref<Service> ServiceBucket::getServiceBase(std::type_index idx) {
		// Try and find
		if(auto it = registeredServices.find(idx); it != registeredServices.end()) {
			return it->second;
		}

		return nullptr;
	}

	void ServiceBucket::registerServiceBase(Ref<Service> service) {
		if(service) {
			registeredServices.insert({ service->getTypeIndex(), service });
		}
	}

	void ServiceBucket::unregisterServiceBase(std::type_index idx) {
		registeredServices.erase(idx);
	}

} // namespace mco::objective
