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

    class InitException : public std::runtime_error {
    public:
        explicit InitException(int error) : std::runtime_error("failed to initialize new socket (Error Code: " + std::to_string(error) + ')') {}
    };

    class NameResolveException : public std::runtime_error {
    public:
        explicit NameResolveException(const char* error, int code) : std::runtime_error(error + std::to_string(code)) {}
    };

    class BindException : public std::runtime_error {
    public:
        explicit BindException(const char* message, int error) : std::runtime_error(message + std::to_string(error)) {}
    };

    class ReadException : public std::runtime_error {
    public:
        explicit ReadException(int error) : std::runtime_error("failed to read from socket (Error Code: " + std::to_string(error) + ')') {}
    };

}

#endif //SOCKETSYS_EXCEPTION_HPP
