#include <iostream>
#include <CoreSocket.hpp>

using namespace acl::CoreSocket;

SOCKET g_socket;
unsigned short port_number = 8090;
const char* ip_address = "10.0.0.89";
uint8_t write_buffer[6] = {0x00,0x01,0x00,0x00,0x00,0x00};
uint8_t read_buffer[10] = {0};

void connect_camera(unsigned short* portno, const char* IPaddress){

    g_socket = open_udp_socket(portno, IPaddress, true);
    //g_socket = connect_udp_port(IPaddress, 8090);
	if(g_socket == BAD_SOCKET){
		std::cout << "Failed to open UDP socket ! Exiting !" << std::endl;
		exit(0);
	}
}

void disconnect_camera(){

    close_socket(g_socket);
}

int write_data(uint8_t* buffer, int bytes, SOCKET s, int chunkSize){

   int sofar = 0;
   int remaining = bytes;

   // Send all the bytes 
   while (remaining > 0) {
     int nextChunk = chunkSize;
     if (nextChunk > remaining) {
       nextChunk = remaining;
     }

     printf("Writing: %X\n",(unsigned char)buffer[sofar]);
     if (nextChunk != noint_block_write(s, (const char*)buffer[sofar], nextChunk)) {
        std::cout <<"Error while writing !" << std::endl;
       return -1;
     }
     sofar += nextChunk;
     remaining -= nextChunk;
   }

   return 0;
 }

int read_data(uint8_t* buffer, int bytes, SOCKET s, int chunkSize){

  int sofar = 0;
  int remaining = bytes;

  // Get all the bytes
  while (remaining > 0) {
    int nextChunk = chunkSize;
    if (nextChunk > remaining) {
      nextChunk = remaining;
    }

    if (nextChunk != noint_block_read(s, (char*)buffer[sofar], nextChunk)) {
        std::cout <<"Error while reading !" << std::endl;
      return -1;
    }
    sofar += nextChunk;
    remaining -= nextChunk;
  }

  return 0;
}

int main(){

	connect_camera(&port_number, ip_address);

	write_data(write_buffer, 6, g_socket, 1);

	read_data(read_buffer, 10, g_socket, 1);

	disconnect_camera();

}	
