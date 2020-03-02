#include <iostream>
#include <CoreSocket.hpp>

using namespace acl::CoreSocket;

int main(unsigned argc, const char* argv[])
{
  // Test closing a bad socket.
  {
    SOCKET s = BAD_SOCKET;
    if (-100 != close_socket(s)) {
      std::cerr << "Error closing BAD_SOCKET" << std::endl;
      return 1;
    }
  }

  // Test creating and destroying various types of server sockets
  // using the most-basic open command.
  {
    SOCKET s;
    s = open_socket(SOCK_STREAM, nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening stream socket on any port and interface" << std::endl;
      return 101;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing stream socket on any port and interface" << std::endl;
      return 102;
    }

    s = open_socket(SOCK_DGRAM, nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening datagram socket on any port and interface" << std::endl;
      return 103;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing datagram socket on any port and interface" << std::endl;
      return 104;
    }
  }

  // Test creating and destroying various types of server sockets
  // using the type-specific open commands.
  {
    SOCKET s;
    s = open_tcp_socket(nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening TCP socket on any port and interface" << std::endl;
      return 201;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing TCP socket on any port and interface" << std::endl;
      return 202;
    }

    s = open_udp_socket(nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening UDP socket on any port and interface" << std::endl;
      return 203;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing UDP socket on any port and interface" << std::endl;
      return 204;
    }
  }

  /// @todo More tests
  return 0;
}