#include <gtest/gtest.h>

#include <socketsys/address/ipv4.hpp>
#include <socketsys/address/ipv6.hpp>

using socketsys::address::AddressV4;
using socketsys::address::AddressV6;

template<typename Address>
void testAddressPair(const Address& address) {}

template<typename Address, typename First, typename ... Rest>
void testAddressPair(const Address& address, First string, Rest... rest) {
    EXPECT_TRUE(address.getBinary() == Address(string, 80).getBinary());
    testAddressPair(address, rest...);
}

template<typename Address, typename ... Strings>
void testAddress(const Address& originAddress, Strings... strings) {
    testAddressPair(originAddress, strings...);
}

TEST(ipv4, parse) {
    testAddress(
        AddressV4({192, 168, 0, 1}, 80),
        "192.168.0.1"
    );
}

TEST(ipv6, parse) {
    testAddress(
        AddressV6({0, 0, 0, 0, 0, 0, 1, 1}, 80),
        "0000:0000:0000:0000:0000:0000:0001:0001",
        "::0:0:1:1",
        "::0.1.0.1",
        "0:0::0:0:1:1"
    );
}