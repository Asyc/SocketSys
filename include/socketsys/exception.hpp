#ifndef SOCKETSYS_EXCEPTION_HPP
#define SOCKETSYS_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace socketsys {

    class SocketInitException : public std::runtime_error {
    public:
        explicit SocketInitException(const char* error, int code) : std::runtime_error(error + std::to_string(code)) {}
    };

    class IOException : public std::runtime_error {
    public:
        explicit IOException(const char* error, int code) : std::runtime_error(error + std::to_string(code)) {}
    };

    class SocketOptionError : public std::runtime_error {
    public:
        explicit SocketOptionError(const char* error, int code) : std::runtime_error(error + std::to_string(code)) {}
    };

    class NameResolveException : public std::runtime_error {
    public:
        explicit NameResolveException(const char* error, int code) : std::runtime_error(error + std::to_string(code)) {}
    };

    class SocketBindException : public std::runtime_error {
    public:
        explicit SocketBindException(const char* message, int error) : std::runtime_error(message + std::to_string(error)) {}
    };

}

#endif //SOCKETSYS_EXCEPTION_HPP
