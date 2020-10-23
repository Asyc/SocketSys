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
    };

    class WinSockServerProvider {
    public:
        using SocketHandle = uint64_t;
        using ClientHandle = SocketInterface<WinSockProvider>;

        static SocketHandle init(AddressFamily addressFamily, SocketProtocol protocol);
        static void bind(SocketHandle handle, const std::string_view& address, uint16_t port, size_t backlog);
        static ClientHandle accept(SocketHandle handle);

        static void swap(SocketHandle& lhs, SocketHandle& rhs);
        static void destroy(SocketHandle handle);
    };

}


#endif //SOCKETSYS_PROVIDER_HPP
