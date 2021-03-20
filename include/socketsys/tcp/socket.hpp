#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_TCP_SOCKET_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_TCP_SOCKET_HPP_

#include <variant>

#include "socketsys/address/ipv4.hpp"
#include "socketsys/address/ipv6.hpp"

namespace socketsys::tcp {

class Socket {
public:
    virtual ~Socket() = default;

    void bind(const address::Address& address);
    void connect(const address::Address& address);
    void close();

    uint32_t setLinger();
    uint32_t getLinger() const;

    [[nodiscard]] const address::Address& getRemoteAddres() const;
    [[nodiscard]] const address::Address& getBoundAddress() const;
private:
    std::variant<address::AddressV4, address::AddressV6> m_RemoteAddress;
    std::variant<address::AddressV4, address::AddressV6> m_BoundAddress;
};

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_TCP_SOCKET_HPP_
