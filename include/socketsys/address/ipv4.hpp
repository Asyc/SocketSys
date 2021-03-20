#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_IPV4_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_IPV4_HPP_

#include "address.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace socketsys::address {

class AddressV4 : public Address {
public:
    using AddressBinary = std::array<uint8_t, 4>;

    AddressV4(const AddressBinary& binary, uint16_t service);
    AddressV4(const std::string_view& ipRepresentation, uint16_t service);

    [[nodiscard]] AddressFamily getAddressFamily() const override;
    [[nodiscard]] std::string_view getTextRepresentation() const override;
    [[nodiscard]] uint16_t getService() const override;

    [[nodiscard]] const AddressBinary& getBinary() const;
private:
    [[nodiscard]] const void* data() const override;
    AddressBinary m_Octets;
    uint16_t m_Service;

    std::array<std::string::value_type, 3 * 4 + 2 + 1> m_IpRepresentation;
};

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_IPV4_HPP_
