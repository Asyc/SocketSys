#ifndef SOCKETSYS_PLATFORM_HPP
#define SOCKETSYS_PLATFORM_HPP

#include <cstdint>
#include <optional>
#include <string_view>

namespace socketsys {
#ifdef WIN32

namespace tcp {

class WinSockProvider {
public:
    using Handle = uint64_t;
    using IpAddress = std::string_view;
    using Port = uint16_t;

    static constexpr Handle INVALID_HANDLE = ~(0);

    static Handle newSocket(AddressFamily family);
    static void destroy(Handle handle);

    static void connect(Handle handle, const IpAddress& remoteAddress, uint16_t remotePort);
};

using PLATFORM_PROVIDER = WinSockProvider;
}

#endif
}

#endif //SOCKETSYS_PLATFORM_HPP
