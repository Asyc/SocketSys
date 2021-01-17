#ifndef SOCKETSYS_SOCKET_HPP_
#define SOCKETSYS_SOCKET_HPP_

#include <cstdint>
#include <string_view>

#include "socketsys/address.hpp"
#include "socketsys/platform.hpp"

namespace socketsys::tcp {

struct SocketConfig {
    bool
};

/**
 * An abstract approach to interacting with a system's
 * socket functionality.
 *
 * @tparam ImplProvider The provider for which socket operations will be done on.
 */
template<typename ImplProvider>
class SocketInterface {
public:
    using SocketHandle = typename ImplProvider::Handle;

    explicit SocketInterface(AddressFamily family = AddressFamily::IPV4) : m_Handle(ImplProvider::newSocket(family)) {}

    SocketInterface(const SocketInterface& rhs) = delete;
    SocketInterface(SocketInterface&& rhs) noexcept : m_Handle(ImplProvider::INVALID_HANDLE) {
        std::swap(m_Handle, rhs.m_Handle);
    };

    ~SocketInterface() {
        ImplProvider::destroy(m_Handle);
    }

    SocketInterface& operator=(const SocketInterface& rhs) = delete;
    SocketInterface& operator=(SocketInterface&& rhs) noexcept {
        std::swap(m_Handle, rhs.m_Handle);
    }

    void connect(const std::string_view& address, uint16_t port) {
        ImplProvider::connect(m_Handle, address, port);
    }

    void close() {
        SocketInterface<ImplProvider> temp(std::move(*this));
    }

    //size_t read(void* buffer, size_t bufferSize);

private:
    SocketHandle m_Handle;

};

using Socket = SocketInterface<PLATFORM_PROVIDER>;
} // namespace socketsys::tcp

#endif //SOCKETSYS_SOCKET_HPP_
