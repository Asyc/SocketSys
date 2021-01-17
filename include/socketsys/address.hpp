#ifndef SOCKETSYS_ADDRESS_HPP
#define SOCKETSYS_ADDRESS_HPP

#include <cstdint>

namespace socketsys {

enum class AddressFamily : uint8_t {
    IPV4,
    IPV6
};

}   // namespace socketsys

#endif //SOCKETSYS_ADDRESS_HPP
