#include <iostream>
#include <CoreSocket.hpp>

using namespace acl::CoreSocket;

int main()
{
  const char* ip_address = "10.0.0.89";
  unsigned short port_number = 8090;
  char write_buffer[6] = {0};
  char read_buffer[10] = {0};

  SOCKET s = connect_udp_port(ip_address, port_number);
  if (s == BAD_SOCKET){
    std::cerr << "Failed to open UDP socket!" << std::endl;
    return -1;
  }

  if (6 != noint_block_write(s, write_buffer, 6)) {
    std::cerr <<"Error while writing!" << std::endl;
    return -2;
  }

  if (10 != noint_block_read(s, read_buffer, 10)) {
    std::cerr <<"Error while reading!" << std::endl;
    return -3;
  }

  close_socket(s);
  std::cout << "Success!" << std::endl;
  return 0;
}

