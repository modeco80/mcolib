#pragma once

#include <mco/base_types.hpp>
#include <mco/objective/service.hpp>
#include <typeindex>
#include <unordered_map>

namespace mco::objective {

    /// A basic implementation of the service locator pattern.
    /// Holds implementations of service interfaces.
    class ServiceBucket {
        std::unordered_map<std::type_index, Ref<Service>> registeredServices;

        Ref<Service> getServiceBase(std::type_index typeIndex);
        void registerServiceBase(Ref<Service> service);
        void unregisterServiceBase(std::type_index typeIndex);

    public:

        template<class TInterface>
        void registerService(Ref<TInterface> service) {
            registerServiceBase(service);
        }

        template<class TInterface>
        Ref<TInterface> getService() {
            return getServiceBase(std::type_index(typeid(TInterface)));
        }

        template<class TInterface>
        void unregisterService() {
            return unregisterServiceBase(std::type_index(typeid(TInterface)));
        }

    };

}