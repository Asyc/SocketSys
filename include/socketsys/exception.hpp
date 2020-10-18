#ifndef SOCKETSYS_EXCEPTION_HPP
#define SOCKETSYS_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace socketsys {

    class InitException : public std::runtime_error {
    public:
        explicit InitException(int error) : std::runtime_error("failed to initialize new socket (Error Code: " + std::to_string(error) + ')') {}
    };

    class NameResolveException : public std::runtime_error {
    public:
        explicit NameResolveException(int error) : std::runtime_error("failed to resolve name (Error Code: " + std::to_string(error) + ')') {}
    };

    class BindException : public std::runtime_error {
    public:
        explicit BindException(int error) : std::runtime_error("failed to bind new socket (Error Code: " + std::to_string(error) + ')') {}
    };

    class ReadException : public std::runtime_error {
    public:
        explicit ReadException(int error) : std::runtime_error("failed to read from socket (Error Code: " + std::to_string(error) + ')') {}
    };

}

#endif //SOCKETSYS_EXCEPTION_HPP
