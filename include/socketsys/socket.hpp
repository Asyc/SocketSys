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
        SocketInterface(const SocketInterface<Provider>& other) = delete;
        SocketInterface(SocketInterface<Provider>&& rhs) noexcept {Provider::swap(m_Handle, rhs.m_Handle);}
        ~SocketInterface() {Provider::destroy(m_Handle);}

        SocketInterface<Provider>& operator=(const SocketInterface<Provider>& rhs) = delete;
        SocketInterface<Provider>& operator=(SocketInterface<Provider>&& rhs) noexcept {
            if (this != &rhs) Provider::swap(m_Handle, rhs);
        }

        void connect(const std::string_view& ip, uint16_t port) {Provider::connect(m_Handle, ip, port);}

        size_t read(char* buffer, size_t length) {return Provider::read(m_Handle, buffer, length);}

        template <typename Container>
        size_t read(Container& container) {return Provider::read(m_Handle, container.data(), container.size());}

        size_t write(const char* buffer, size_t length) {return Provider::write(m_Handle, buffer, length);}

        template <typename Container>
        size_t write(const Container& container) {return Provider::write(m_Handle, container.data(), container.size());}

        // Config Functions
        void setTcpNoDelay(bool flag) {Provider::setTcpNoDelay(m_Handle, flag);}
        void setSoReuseAddress(bool flag) {Provider::setSoReuseAddress(m_Handle, flag);}
        void setSoBroadcast(bool flag) {Provider::setSoBroadcast(m_Handle, flag);}
        void setSoLinger(bool flag, uint16_t seconds) {Provider::setSoLinger(m_Handle, flag, seconds);}
        void setSoReceiveTimeout(uint32_t milliseconds) {Provider::setSoReceiveTimeout(m_Handle, milliseconds);}
        void setSoSendTimeout(uint32_t milliseconds) {Provider::setSoSendTimeout(m_Handle, milliseconds);}
        void setSoSendBufferSize(int size) {Provider::setSoSendBufferSize(m_Handle, size);}
        void setSoReceiveBufferSize(int size) {Provider::setSoReceiveBufferSize(m_Handle, size);}
        void setSoKeepAlive(bool flag) {Provider::setSoKeepAlive(m_Handle, flag);}
        void setInlineOOB(bool flag) {Provider::setSoInlineOOB(m_Handle, flag);}
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
        ~ServerSocketInterface() {Provider::destroy(m_Handle);}

        ServerSocketInterface<Provider>& operator=(const ServerSocketInterface<Provider>& rhs) = delete;
        ServerSocketInterface<Provider>& operator=(ServerSocketInterface<Provider>&& rhs) noexcept {
            if (this != &rhs) Provider::swap(m_Handle, rhs);
        }

        void bind(const std::string_view& ip, uint16_t port) {
            Provider::bind(m_Handle, ip, port, 4);
        }

        ClientHandle accept() {return Provider::accept(m_Handle);}

        void setSoReuseAddress(bool flag) {Provider::setSoReuseAddress(m_Handle, flag);}
    private:
        SocketHandle m_Handle;
    };

    using Socket = SocketInterface<PLATFORM_PROVIDER>;
    using ServerSocket = ServerSocketInterface<SERVER_PLATFORM_PROVIDER>;
}

#endif //SOCKETSYS_SOCKET_HPP
