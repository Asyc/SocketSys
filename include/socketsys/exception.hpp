#ifndef SOCKETSYS_EXCEPTION_HPP
#define SOCKETSYS_EXCEPTION_HPP

#include <stdexcept>

namespace socketsys {

class SocketException : public std::runtime_error {
public:
    SocketException(const char* error, size_t platformError) : std::runtime_error(error), m_PlatformError(platformError) {}

    [[nodiscard]] size_t getPlatformError() const {
        return m_PlatformError;
    }
private:
    size_t m_PlatformError;
};

#define CREATE_EXCEPTION(name, error)                                                                                       \
    class name : public SocketException {                                                                                   \
    public:                                                                                                                 \
        explicit name(size_t platformError) : SocketException(error, platformError) {}                                      \
        explicit name(const char* errorMessage, size_t platformError) : SocketException(errorMessage, platformError) {}     \
    }

#define CREATE_CHILD_EXCEPTION(parent, name, error)                                                                \
    class name : public parent {                                                                                   \
    public:                                                                                                        \
        explicit name(size_t platformError) : parent(error, platformError) {}                                      \
        explicit name(const char* errorMessage, size_t platformError) : parent(errorMessage, platformError) {}     \
    }

CREATE_EXCEPTION(SocketCreateError, "Failed to create socket");
CREATE_CHILD_EXCEPTION(SocketCreateError, AddressFamilyUnsupported, "Socket address family is unsupported");
CREATE_CHILD_EXCEPTION(SocketCreateError, UnsupportedProtocolError, "Socket protocol is unsupported");
CREATE_CHILD_EXCEPTION(SocketCreateError, MaximumSocketsError, "Maximum amount of socket connections");

CREATE_EXCEPTION(AddressUnresolvableError, "Unable to resolve address name");

CREATE_EXCEPTION(SocketOperationError, "Failed to run socket operation");

CREATE_EXCEPTION(SocketConnectError, "Socket failed to connect");
CREATE_CHILD_EXCEPTION(SocketConnectError, NetworkDownError, "The network subsystem has failed");
CREATE_CHILD_EXCEPTION(SocketConnectError, AddressInUseError, "The socket's local address is already in use");
CREATE_CHILD_EXCEPTION(SocketConnectError, ConnectionRefusedError, "The attempt to connect was forcefully rejected");
CREATE_CHILD_EXCEPTION(SocketConnectError, SocketAlreadyConnectedError, "The socket is already connected");
CREATE_CHILD_EXCEPTION(SocketConnectError, HostUnreachableError, "The socket attempted to connect to an unreachable host");



#undef CREATE_EXCEPTION

}
#endif //SOCKETSYS_EXCEPTION_HPP
