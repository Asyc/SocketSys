# SocketSys [![Asyc Actions Status](https://github.com/Asyc/SocketSys/workflows/build/badge.svg)](https://github.com/Asyc/SocketSys/actions)

### Introduction
SocketSys is a modular C++17 socket wrapper, providing a SocketInterface class for interacting <br>
with the Operating System's socket functionality.

This project is licensed under the MIT License.

### Usage
```C++
#include <socketsys/socket.hpp>

using namespace socketsys;

int main() {
    Socket socket;
    socket.connect("127.0.0.1", 80);
    
    ServerSocket serverSocket;
    serverSocket.bind("0.0.0.0", 80);
}
```

#### Alternatively, you can manually specify a custom "provider" or operating system implementation
```C++
SocketInterface<WinSockProvider> socket;
```

The default `Socket` and `ServerSocket` classes are typedefs with a OS-Macro dependant provider specified.

### Building
This project uses CMake to build. Cloning submodules is not required unless you desire testing and do not
have GTest on your path.
```bash
$ git clone https://github.com/Asyc/SocketSys.git
```

It is recommended to use the CMake script to add this library to your project
```cmake
add_subdirectory(socketsys)
target_link_libraries(<Project> <PUBLIC/PRIVATE> SocketSys)
```
Alternatively, you can build the static library manually.

Enter the directory and create a build folder, enter that as well. You can then build the project by using
```bash
$ cmake ../
$ cmake --build . --config release
```