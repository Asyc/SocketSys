#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_IPV6_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_IPV6_HPP_

#include "address.hpp"

#include <array>

namespace socketsys::address {

class AddressV6 : public Address {
public:
    using AddressBinary = std::array<uint16_t, 8>;

    AddressV6(const AddressBinary& binary, uint16_t service);
    AddressV6(const std::string_view& ipRepresentation, uint16_t service);

    [[nodiscard]] virtual AddressFamily getAddressFamily() const override;
    [[nodiscard]] virtual std::string_view getTextRepresentation() const override;
    [[nodiscard]] virtual uint16_t getService() const override;

    [[nodiscard]] const AddressBinary& getBinary() const;
private:
    [[nodiscard]] const void* data() const override;

    AddressBinary m_Binary;
    uint16_t m_Service;

    std::array<std::string::value_type, 4 * 8 + 7 + 1> m_IpRepresentation;
};

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_ADDRESS_IPV6_HPP_
