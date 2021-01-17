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

    /**
     * Time in seconds where the socket will "linger" after closing; finishing
     * any pending messages, etc.
     *
     * A value of zero will result in no linger.
     */
    uint16_t soLinger;

    // Protocol Options

    /**
     * Gets or sets the number of TCP keep alive probes that will
     * be sent before the connection is terminated.
     */
    uint8_t tcpKeepAliveCount;

    /**
     * Enables or disables the Nagle algorithm for TCP sockets.
     */
    bool tcpNoDelay;

    /**
     * Enables or disables RFC 1323 time stamps.
     */
    bool tcpTimestamps;

    /**
     * Gets or sets the number of seconds a TCP connection will remain idle before
     * keep-alive probes are sent to the remote.
     */
    uint32_t tcpKeepAlive;

    /**
     * Gets or sets the number of seconds a TCP connection will wait for a keep-alive
     * response before sending another keep-alive probe.
     */
    uint32_t tcpKeepAliveInterval;

};

}   // namespace socketsys::tcp

#endif //SOCKETSYS_CONFIG_HPP
