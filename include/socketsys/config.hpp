#ifndef SOCKETSYS_CONFIG_HPP
#define SOCKETSYS_CONFIG_HPP

#include <cstdint>

namespace socketsys::tcp {

struct SocketConfig {
    /**
     * Prevents any other socket from binding to the same address and port.
     */
    bool soExclusiveAddressUse;

    /**
     * Allows a socket to bind to an address and port already in use.
     */
    bool soReuseAddress;

    /**
     * Enable the TCP-Socket Keep Alive.
     */
    bool soKeepAlive;

    /**
     * Inline Out-Of-Band data.
     */
    bool soInlineOOB;

    /**
     * The size of the socket's internal receive buffer.
     */
    uint32_t soRcvBuf;

    /**
     * The size of the socket's internal send buffer.
     */
    uint32_t soSndBuf;

    /**
     * The timeout, in milliseconds, for blocking receive calls.
     * A value of zero will result in no timeout.
     */
    uint32_t soRcvTimeout;

    /**
     * The timeout, in milliseconds, for blocking send calls.
     * A value of zero will result in no timeout.
     */
    uint32_t soSndTimeout;
    uint16_t soLinger;
};

}   // namespace socketsys::tcp

#endif //SOCKETSYS_CONFIG_HPP
