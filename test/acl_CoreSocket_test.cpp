#include <iostream>
#include <CoreSocket.hpp>

using namespace acl::CoreSocket;

int main(unsigned argc, const char* argv[])
{
  // Test creating and destroying various types of server sockets
  // using the most-basic open command.
  {
    SOCKET s = open_socket(SOCK_STREAM, nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening stream socket on any port and interface" << std::endl;
      return 1;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing stream socket on any port and interface" << std::endl;
      return 2;
    }
  }

  /// @todo More tests
  return 0;
}