#ifndef SOCKETSYS_PROVIDER_HPP
#define SOCKETSYS_PROVIDER_HPP

#include <cstdint>
#include <string_view>

#include "platform.hpp"

namespace socketsys {

    class WindowsProvider {
    public:
        using SocketHandle = PLATFORM_HANDLE;

        static SocketHandle init();
        static void bind(SocketHandle handle, const std::string_view& address, uint16_t port);
        static size_t read(SocketHandle handle, char* buffer, size_t length);
        static void destroy(SocketHandle handle);
    };


}


#endif //SOCKETSYS_PROVIDER_HPP
