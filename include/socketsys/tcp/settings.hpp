#ifndef SOCKETSYS_INCLUDE_SOCKETSYS_TCP_SETTINGS_HPP_
#define SOCKETSYS_INCLUDE_SOCKETSYS_TCP_SETTINGS_HPP_

#include <cstdint>

namespace socketsys::tcp {

class SocketSettings {
public:
    virtual ~SocketSettings() = default;

    virtual uint32_t getLinger() const = 0;
    virtual void setLinger(uint32_t seconds) = 0;

    virtual bool getExclusiveAddressUse() const = 0;
    virtual void setExclusiveAddressUse(bool flag) = 0;

    virtual bool getKeepAlive() const = 0;
    virtual bool setKeepAlive(bool flag) = 0;


};

}

#endif //SOCKETSYS_INCLUDE_SOCKETSYS_TCP_SETTINGS_HPP_
