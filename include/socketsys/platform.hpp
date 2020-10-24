#ifndef SOCKETSYS_PLATFORM_HPP
#define SOCKETSYS_PLATFORM_HPP

#include <cstdint>

namespace socketsys {
#if defined(_WIN64) || defined(_WIN32)
    using PLATFORM_PROVIDER = class WinSockProvider;
    using SERVER_PLATFORM_PROVIDER = class WinSockServerProvider;
#elif defined(__APPLE__) || defined(__MACH__)
#error "MACOS Not Supported Yet"
#elif defined(__linux__)
#error "Linux Not Supported Yet"
#else
#error "Unknown platform"
#endif
}


#endif //SOCKETSYS_PLATFORM_HPP