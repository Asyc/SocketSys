#include "socketsys/address/address_resolver.hpp"

#include <WS2tcpip.h>

#include <array>
#include <iostream>

#include "winsock.hpp"

namespace socketsys::address {

AddressResolver::AddressResolver() {
    windows::startup();
}

AddressResolver::~AddressResolver() {
    windows::cleanup();
}

AddressV4 AddressResolver::resolveIPv4(const std::string_view& name, uint16_t service) {
    std::array<char, 5 + 1> serviceBuffer{};
    sprintf_s(serviceBuffer.data(), serviceBuffer.size(), "%d");

    addrinfo hints{
        NULL,
        PF_INET,
    };

    addrinfo* results;
    auto status = getaddrinfo(name.data(), serviceBuffer.data(), &hints, &results);
    windows::mapError(status);

    in_addr ip_address = reinterpret_cast<sockaddr_in*>(results->ai_addr)->sin_addr;
    freeaddrinfo(results);

    return AddressV4(AddressV4::AddressBinary{
        ip_address.S_un.S_un_b.s_b1,
        ip_address.S_un.S_un_b.s_b2,
        ip_address.S_un.S_un_b.s_b3,
        ip_address.S_un.S_un_b.s_b4
    }, service);
}

AddressV6 AddressResolver::resolveIPv6(const std::string_view& name, uint16_t service) {
    std::array<char, 5 + 1> serviceBuffer{};
    sprintf_s(serviceBuffer.data(), serviceBuffer.size(), "%d");

    addrinfo hints{
        NULL,
        PF_INET6,
    };

    addrinfo* results;
    auto status = getaddrinfo(name.data(), serviceBuffer.data(), &hints, &results);
    windows::mapError(status);

    in_addr6 ip_address = reinterpret_cast<sockaddr_in6*>(results->ai_addr)->sin6_addr;
    freeaddrinfo(results);

    return AddressV6(AddressV6::AddressBinary{
        ip_address.u.Word[0],
        ip_address.u.Word[1],
        ip_address.u.Word[2],
        ip_address.u.Word[3],
        ip_address.u.Word[4],
        ip_address.u.Word[5],
        ip_address.u.Word[6],
        ip_address.u.Word[7]
    }, service);
}

}