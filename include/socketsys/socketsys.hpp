#ifndef SOCKETSYS_SOCKETSYS_HPP
#define SOCKETSYS_SOCKETSYS_HPP

#include <cstdint>

#include "platform.hpp"
#include "provider.hpp"

namespace socketsys {

    template <typename Provider>
    class SocketInterface {
    public:
        SocketInterface() : m_Handle(Provider::init()) {}

        void bind(const std::string_view& ip, uint16_t port) const {
            Provider::bind(m_Handle, ip, port);
        }

        size_t read(char* buffer, size_t length) const {
            return Provider::read(m_Handle, buffer, length);
        }

        ~SocketInterface() {
            Provider::destroy(m_Handle);
        }
    private:
        PLATFORM_HANDLE m_Handle;
    };

    using Socket = SocketInterface<PLATFORM_PROVIDER>;
}

#endif //SOCKETSYS_SOCKETSYS_HPP
