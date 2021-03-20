#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_ADDRESS_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_ADDRESS_HPP_

#include <string_view>

namespace socketsys::address {

enum class AddressFamily : uint8_t {
    IPV4,
    IPV6
};

class Address {
public:
    virtual ~Address() = default;

    [[nodiscard]] virtual AddressFamily getAddressFamily() const = 0;
    [[nodiscard]] virtual std::string_view getTextRepresentation() const = 0;
    [[nodiscard]] virtual uint16_t getService() const = 0;
private:
    [[nodiscard]] virtual const void* data() const = 0;
};

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_ADDRESS_HPP_
