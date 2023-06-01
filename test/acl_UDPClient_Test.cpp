/**
 *    \copyright Copyright 2021 Aqueti, Inc. All rights reserved.
 *    \license This project is released under the MIT Public License.
**/

#include <iostream>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <CoreSocket.hpp>

using namespace acl::CoreSocket;

int main()
{
  const char* ip_address = "10.0.0.89";
  unsigned short port_number = 8090;
  char write_buffer[6] = {0x00,0x01,0x00,0x00,0x00,0x00};
  char read_buffer[10] = {};
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  std::cout << "This test sends 6 bytes to port " << port_number
    << " at " << ip_address << " and then reads 10 bytes." << std::endl;

  SOCKET s = connect_udp_port(ip_address, port_number);
  if (s == BAD_SOCKET){
    std::cerr << "Failed to open UDP socket!" << std::endl;
    return -1;
  }else{
    std::cerr <<"Connected UDP port" << std::endl;
  }

  if (6 != noint_block_write(s, write_buffer, 6)) {
    std::cerr <<"Error while writing!" << std::endl;
    return -2;
  }else{
    std::cerr <<"Write Successful" << std::endl;
  }

  int n = noint_block_read_timeout(s, read_buffer, 10, &timeout);
  if (n<=0) {
    std::cerr <<"Error while reading: " << n << " bytes read" << std::endl;
    return -3;
  }else{
    std::cerr <<"Read Successful" << std::endl;
  }

  for(int i=0;i<n;++i)
    printf("%02X\n", (unsigned char)read_buffer[i]);

  close_socket(s);
  std::cout << "Success closing the socket!" << std::endl;
  return 0;
}

