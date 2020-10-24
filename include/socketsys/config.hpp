#ifndef SOCKETSYS_CONFIG_HPP
#define SOCKETSYS_CONFIG_HPP

namespace socketsys {

    enum class AddressFamily {
        IPV4,
        IPV6
    };

    enum class SocketProtocol {
        TCP,
        UDP
    };
}


#endif //SOCKETSYS_CONFIG_HPP
