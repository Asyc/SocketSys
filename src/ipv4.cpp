#include "socketsys/address/ipv4.hpp"

#include "socketsys/error/exception.hpp"

namespace socketsys::address {

AddressV4::AddressV4(const address::AddressV4::AddressBinary& binary, uint16_t service) : m_Octets(binary), m_Service(service) {
    size_t index = 0;
    for (auto octet : m_Octets) {
        if (index != 0) {
            m_IpRepresentation[index++] = '.';
        }

        index += sprintf(m_IpRepresentation.data() + index, "%d", octet);
    }

    m_IpRepresentation[index + 1] = '\0';
}

AddressV4::AddressV4(const std::string_view& ipRepresentation, uint16_t service) : m_Service(service), m_Octets() {
    std::array<std::string_view::value_type, 4> octetBuffer{};
    size_t octetIndex = 0;
    size_t binaryIndex = 0;

    if (ipRepresentation[0] == '.' || ipRepresentation[ipRepresentation.size() - 1] == '.')
        throw error::InvalidHostException(-1);

    for (char i : ipRepresentation) {
        if (i != '.' && (i < '0' || i > '9')) {
            throw socketsys::error::InvalidHostException(-1);
        }

        if (octetIndex >= octetBuffer.size()) {
            throw socketsys::error::InvalidHostException(-1);
        }

        if (i == '.') {
            octetBuffer[octetIndex] = '\0';
            octetIndex = 0;
            m_Octets[binaryIndex++] = atoi(octetBuffer.data());   //NOLINT
            continue;
        }

        octetBuffer[octetIndex++] = i;
    }

    if (binaryIndex + 1 != 4) throw error::InvalidHostException(-1);

    octetBuffer[octetIndex] = '\0';
    m_Octets[binaryIndex++] = atoi(octetBuffer.data()); //NOLINT
    memcpy(m_IpRepresentation.data(), ipRepresentation.data(), ipRepresentation.size());
}

AddressFamily AddressV4::getAddressFamily() const {
    return address::AddressFamily::IPV4;
}
std::string_view AddressV4::getTextRepresentation() const {
    return std::string_view{m_IpRepresentation.data(), m_IpRepresentation.size() - 1};
}
uint16_t AddressV4::getService() const {
    return m_Service;
}

const AddressV4::AddressBinary& address::AddressV4::getBinary() const {
    return m_Octets;
}

const void* AddressV4::data() const {
    return m_Octets.data();
}

}