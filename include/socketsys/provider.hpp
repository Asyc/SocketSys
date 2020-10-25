#ifndef SOCKETSYS_PROVIDER_HPP
#define SOCKETSYS_PROVIDER_HPP

#include <cstdint>
#include <string_view>

#include "config.hpp"
#include "platform.hpp"

namespace socketsys {

    template <typename Provider> class SocketInterface;

    class WinSockProvider {
    public:
        using SocketHandle = uint64_t;

        static SocketHandle init(AddressFamily addressFamily, SocketProtocol protocol);
        static void destroy(SocketHandle handle);
        static void swap(SocketHandle& lhs, SocketHandle& rhs);

        static void connect(SocketHandle handle, const std::string_view& address, uint16_t port);

        static size_t read(SocketHandle handle, char* buffer, size_t length);
        static size_t write(SocketHandle handle, const char* buffer, size_t length);

        static void setTcpNoDelay(SocketHandle handle, bool flag);
        static void setSoReuseAddress(SocketHandle handle, bool flag);
        static void setSoBroadcast(SocketHandle handle, bool flag);
        static void setSoLinger(SocketHandle handle, bool flag, uint16_t linger);
        static void setSoReceiveTimeout(SocketHandle handle, uint32_t timeout);
        static void setSoSendTimeout(SocketHandle handle, uint32_t timeout);
        static void setSoSendBufferSize(SocketHandle handle, int size);
        static void setSoReceiveBufferSize(SocketHandle handle, int size);
        static void setSoKeepAlive(SocketHandle handle, bool flag);
        static void setSoInlineOOB(SocketHandle handle, bool flag);
    };

    class WinSockServerProvider {
    public:
        using SocketHandle = uint64_t;
        using ClientHandle = SocketInterface<WinSockProvider>;

        static SocketHandle init(AddressFamily addressFamily, SocketProtocol protocol);
        static void swap(SocketHandle& lhs, SocketHandle& rhs);
        static void destroy(SocketHandle handle);

        static void bind(SocketHandle handle, const std::string_view& address, uint16_t port, size_t backlog);
        static ClientHandle accept(SocketHandle handle);

        static void setSoReuseAddress(SocketHandle handle, bool flag);
        static void setSoLinger(SocketHandle handle, bool flag, uint16_t seconds);
    };

    class UnixProvider {
    public:
        using SocketHandle = int;

        static SocketHandle init(AddressFamily addressFamily, SocketProtocol protocol);
        static void destroy(SocketHandle handle);
        static void swap(SocketHandle& lhs, SocketHandle& rhs);

        static void connect(SocketHandle handle, const std::string_view& address, uint16_t port);

        static size_t read(SocketHandle handle, char* buffer, size_t length);
        static size_t write(SocketHandle handle, const char* buffer, size_t length);

        static void setTcpNoDelay(SocketHandle handle, bool flag);
        static void setSoReuseAddress(SocketHandle handle, bool flag);
        static void setSoBroadcast(SocketHandle handle, bool flag);
        static void setSoLinger(SocketHandle handle, bool flag, uint16_t linger);
        static void setSoReceiveTimeout(SocketHandle handle, uint32_t timeout);
        static void setSoSendTimeout(SocketHandle handle, uint32_t timeout);
        static void setSoSendBufferSize(SocketHandle handle, int size);
        static void setSoReceiveBufferSize(SocketHandle handle, int size);
        static void setSoKeepAlive(SocketHandle handle, bool flag);
        static void setSoInlineOOB(SocketHandle handle, bool flag);
    };

    class UnixServerProvider {
    public:
        using SocketHandle = int;
        using ClientHandle = SocketInterface<UnixProvider>;

        static SocketHandle init(AddressFamily addressFamily, SocketProtocol protocol);
        static void swap(SocketHandle& lhs, SocketHandle& rhs);
        static void destroy(SocketHandle handle);

        static void bind(SocketHandle handle, const std::string_view& address, uint16_t port, size_t backlog);
        static ClientHandle accept(SocketHandle handle);

        static void setSoReuseAddress(SocketHandle handle, bool flag);
        static void setSoLinger(SocketHandle handle, bool flag, uint16_t seconds);
    };
}


#endif //SOCKETSYS_PROVIDER_HPP
