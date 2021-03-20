#include "socketsys/address/ipv6.hpp"

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

#include "socketsys/address/ipv4.hpp"
#include "socketsys/error/exception.hpp"

#include <iostream>

namespace socketsys::address {

inline bool checkHex(uint8_t c) {
    if (c >= '0' && c <= '9') return true;
    if (c >= 'a' && c <= 'f') return true;
    if (c >= 'A' && c <= 'F') return true;
    return false;
}

inline uint32_t fromHex(uint8_t c) {
    if (!checkHex(c)) throw error::InvalidHostException(-1);

    return (c & 0xF) + 9 * (c >> 6);
}

inline uint16_t fromHex(const uint8_t* src) {
    return fromHex(src[0]) << 12 | fromHex(src[1]) << 8 | fromHex(src[2]) << 4 | fromHex(src[3]);
}

inline constexpr size_t toHexString(const uint16_t src, char* out) {
    const char* hexTable = "0123456789abcdef";

    out[0] = hexTable[(src >> 12) & 0xf];
    out[1] = hexTable[(src >> 8) & 0xf];
    out[2] = hexTable[(src >> 4) & 0xf];
    out[3] = hexTable[src & 0xf];

    return 4;
}

inline void writeHexString(const uint16_t* src, size_t length, std::array<char, 4 * 8 + 7 + 1>& out) {
    size_t charIndex = 0;
    for (size_t i = 0; i < length; i++) {
        if (i != 0) out[charIndex++] = ':';
        charIndex += toHexString(src[i], out.data() + charIndex);
    }

    out[charIndex] = '\0';
}

inline std::pair<std::size_t, std::optional<AddressV4>> tokenize(std::string_view str, char delimiter, std::array<std::string_view, 8>& out) {
    std::string::size_type lastPos = 0;
    std::string::size_type pos = str.find_first_of(delimiter, lastPos);

    bool emptyFound = false;
    size_t tokenIndex = 0;

    // check if beginning/end is "::"
    if (str[0] == ':' && str[1] == ':') {
        str.remove_prefix(1);
    } else if (str[0] == ':') {
        throw error::InvalidHostException(-1);
    }

    if (str[str.size() - 1] == ':' && str[str.size() - 2] == ':') {
        str.remove_suffix(1);
    } else if (str[str.size() - 1] == ':') {
        throw error::InvalidHostException(-1);
    }

    while (std::string::npos != pos && std::string::npos != lastPos) {
        auto view = str.substr(lastPos, pos - lastPos);
        if (view.size() > 4) throw error::InvalidHostException(-1);

        out[tokenIndex++] = view;

        if (view.empty()) {
            if (emptyFound)
                throw error::InvalidHostException(-1);
            emptyFound = true;
        }

        lastPos = pos + 1;
        pos = str.find_first_of(delimiter, lastPos);
    }

    // Check if there is no reason for a "::" to be in the string
    if (emptyFound && tokenIndex >= 8) throw error::InvalidHostException(-1);

    auto view = str.substr(lastPos, pos - lastPos);
    out[tokenIndex++] = view;
    if (view.empty()) {
        if (emptyFound) throw error::InvalidHostException(-1);
    }

    if (8 - tokenIndex >= 1) {
        try {
            AddressV4 embed(out[tokenIndex - 1], NULL);
            if (!emptyFound && tokenIndex + 1 != 8) throw error::InvalidHostException(-1);
            return {tokenIndex - 1, embed};
        } catch (const error::InvalidHostException& ex) {}
    }

    if ((!emptyFound && tokenIndex != 8) || (emptyFound && tokenIndex == 8)) {
        throw error::InvalidHostException(-1);
    }

    return {tokenIndex, {}};  //already incremented
}

AddressV6::AddressV6(const std::string_view& ipRepresentation, uint16_t service) : m_Binary(), m_Service(service) {
    if (ipRepresentation.size() < 2) throw error::InvalidHostException(-1);
    if (ipRepresentation.size() == 2 && ipRepresentation[0] == ':' && ipRepresentation[1] == ':') {
        writeHexString(m_Binary.data(), m_Binary.size(), m_IpRepresentation);
        return;
    }

    std::array<std::string_view, 8> tokenized{};

    size_t binaryIndex = 0;

    auto[count, embed] = tokenize(ipRepresentation, ':', tokenized);

    for (size_t i = 0; i < count; i++) {
        auto& token = tokenized[i];

        // We can assure that "::" appears only once because of the tokenize function's inline check for multiple empty tokens
        if (token.empty()) {
            binaryIndex += 8 - (count + (embed.has_value() ? 1 : -1));
            continue;
        }

        uint16_t groupValue;

        // If the size is not 4, create a new buffer and pad with zeros at the start
        if (token.size() != 4) {
            std::array<uint8_t, 4> buffer{'0', '0', '0', '0'};
            memcpy(buffer.data() + (4 - token.size()), token.data(), token.size());
            groupValue = fromHex(buffer.data());
        } else {
            groupValue = fromHex(reinterpret_cast<const uint8_t*>(token.data()));
        }

        m_Binary[binaryIndex++] = groupValue;
    }

    if (embed.has_value()) {
        auto* data = embed->getBinary().data();
        m_Binary[binaryIndex++] = (data[1]<<0) | (data[0]<<8);
        m_Binary[binaryIndex++] = (data[3]<<0) | (data[2]<<8);
    }

    writeHexString(m_Binary.data(), m_Binary.size(), m_IpRepresentation);
}

AddressV6::AddressV6(const AddressV6::AddressBinary& binary, uint16_t service) : m_Binary(binary), m_Service(service) {
    writeHexString(m_Binary.data(), m_Binary.size(), m_IpRepresentation);
}

AddressFamily AddressV6::getAddressFamily() const {
    return AddressFamily::IPV6;
}
std::string_view AddressV6::getTextRepresentation() const {
    return std::string_view(m_IpRepresentation.data());
}
uint16_t AddressV6::getService() const {
    return m_Service;
}
const AddressV6::AddressBinary& AddressV6::getBinary() const {
    return m_Binary;
}
const void* AddressV6::data() const {
    return m_Binary.data();
}

}