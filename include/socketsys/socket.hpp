#ifndef SOCKETSYS_SOCKET_HPP
#define SOCKETSYS_SOCKET_HPP

#include <cstdint>

#include "config.hpp"
#include "platform.hpp"
#include "provider.hpp"

namespace socketsys {
    template <typename Provider>
    class SocketInterface {
    public:
        using SocketHandle = typename Provider::SocketHandle;
        explicit SocketInterface(AddressFamily family = AddressFamily::IPV4, SocketProtocol protocol = SocketProtocol::TCP) : m_Handle(Provider::init(family, protocol)) {}
        explicit SocketInterface(SocketHandle handle) {m_Handle = handle;}
    private:
        SocketHandle m_Handle;
    };

    template <typename Provider>
    class ServerSocketInterface {
    public:
        using SocketHandle = typename Provider::SocketHandle;
        using ClientHandle = typename Provider::ClientHandle;

        explicit ServerSocketInterface(AddressFamily family = AddressFamily::IPV4, SocketProtocol protocol = SocketProtocol::TCP) : m_Handle(Provider::init(family, protocol)) {}
        ServerSocketInterface(const ServerSocketInterface<Provider>& other) = delete;
        ServerSocketInterface(ServerSocketInterface<Provider>&& rhs) noexcept {Provider::swap(m_Handle, rhs.m_Handle);}

        ServerSocketInterface<Provider>& operator=(const ServerSocketInterface<Provider>& rhs) = delete;
        ServerSocketInterface<Provider>& operator=(ServerSocketInterface<Provider>&& rhs) noexcept {
            if (this != &rhs) Provider::swap(m_Handle, rhs);
        }


        void bind(const std::string_view& ip, uint16_t port) {
            Provider::bind(m_Handle, ip, port, 4);
        }

        ClientHandle accept() {return Provider::accept(m_Handle);}

        ~ServerSocketInterface() {Provider::destroy(m_Handle);}
    private:
        SocketHandle m_Handle;
    };

    using Socket = SocketInterface<PLATFORM_PROVIDER>;
    using ServerSocket = ServerSocketInterface<SERVER_PLATFORM_PROVIDER>;
}

#endif //SOCKETSYS_SOCKET_HPP
