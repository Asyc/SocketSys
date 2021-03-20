#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_EXCEPTION_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_EXCEPTION_HPP_

#include <stdexcept>

#define defineException(name, defaultMessage) \
class name : public SocketException {         \
public:                                       \
    explicit name(size_t platformError) : SocketException(defaultMessage, platformError) {} \
    name(const char* msg, size_t platformError) : SocketException(msg, platformError) {}                                              \
}

namespace socketsys::error {

class SocketException : public std::runtime_error {
public:
    SocketException(const char* message, size_t platformCode) : std::runtime_error(message), m_PlatformError(platformCode) {}
    explicit SocketException(size_t platformCode) : SocketException("Undefined Exception", platformCode) {}

    /**
     * @return Return's the operating system's "errno". If this is a library exception, this will return "-1".
     */
    [[nodiscard]] size_t getPlatformError() const { return m_PlatformError; }

private:
    size_t m_PlatformError;
};

defineException(UnsupportedAddressFamilyError, "Address family is unsupported");
defineException(HostNotFoundError, "Target host with service not found");
defineException(MemoryAllocationError, "An error in memory allocation occurred");

defineException(InvalidHostException, "Specified host is invalid");

namespace windows {
defineException(WindowsBlockingActionError, "Already operation in progress");   //10036
defineException(WindowsNetworkDownError, "The underlying network is down"); //10050

defineException(WindowsMaxInstanceError, "Max instances of WinSock reached on implementation"); //10067
defineException(WindowsSystemNotReadyError, "Network subsystem is unavailable");    //10091
defineException(WindowsUnsupportedVersionError, "Unsupported version of WinSock");  //10092

}

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_EXCEPTION_HPP_
