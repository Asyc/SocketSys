#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_ADDRESS_RESOLVER_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_ADDRESS_RESOLVER_HPP_

#include <memory>
#include <string_view>

#include "address.hpp"
#include "ipv4.hpp"
#include "ipv6.hpp"

namespace socketsys::address {

class AddressResolver {
public:
    AddressResolver();
    ~AddressResolver();

    AddressV4 resolveIPv4(const std::string_view& name, uint16_t service);
    AddressV6 resolveIPv6(const std::string_view& name, uint16_t service);

    std::unique_ptr<Address> resolveAddressHeap(const std::string_view& name, uint16_t service);
};

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_ADDRESS_RESOLVER_HPP_
