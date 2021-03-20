#include <iostream>

#include <socketsys/address/address_resolver.hpp>
#include <socketsys/address/ipv4.hpp>
#include <socketsys/address/ipv6.hpp>
#include <socketsys/error/exception.hpp>

using socketsys::address::AddressV4;
using socketsys::address::AddressV6;
using socketsys::address::AddressResolver;

int main() {
    AddressResolver resolver;
    AddressV4 a = resolver.resolveIPv4("google.com", 80);
    AddressV4 test({192, 168, 0, 1}, 80);
}