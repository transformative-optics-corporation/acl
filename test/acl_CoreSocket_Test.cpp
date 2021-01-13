#include <iostream>
#include <thread>
#include <vector>
#include <CoreSocket.hpp>

using namespace acl::CoreSocket;

/// @brief Function to read and verify the specified number of bytes from a socket.
///
/// This will ensure that it can read the requested number of bytes from the socket
/// and that the bytes contain modulo-128 numbers, 0 through 127 and then repeating.
/// @param [in] s Socket to read from
/// @param [in] bytes Total number of bytes to read
/// @param [in] chunkSize Size of chunks to read from the socket.
/// @param [out] result true on success, false on failure.
void TestReadFromSocket(bool &result, SOCKET s, int bytes, int chunkSize)
{
  int sofar = 0;
  int remaining = bytes;
  std::vector<char> buf(bytes);

  // Get all the bytes
  while (remaining > 0) {
    int nextChunk = chunkSize;
    if (nextChunk > remaining) {
      nextChunk = remaining;
    }

    if (nextChunk != noint_block_read(s, &buf[sofar], nextChunk)) {
      result = false;
      return;
    }
    sofar += nextChunk;
    remaining -= nextChunk;
  }
  
  // Check the values
  for (int i = 0; i < bytes; i++) {
    if (buf[i] != (i % 128)) {
      result = false;
      return;
    }
  }

  result = true;
  return;
}

/// @brief Function to write the specified number of modulo-128 bytes to a socket.
///
/// This will ensure that it can write the requested number of bytes to the socket.
/// @param [in] s Socket to write to
/// @param [in] bytes Total number of bytes to write
/// @param [in] chunkSize Size of chunks to write to the socket.
/// @param [out] result true on success, false on failure.
void TestWriteToSocket(bool& result, SOCKET s, int bytes, int chunkSize)
{
  int sofar = 0;
  int remaining = bytes;

  // Fill in the values
  std::vector<char> buf(bytes);
  for (int i = 0; i < bytes; i++) {
    buf[i] = i % 128;
  }

  // Send all the bytes
  while (remaining > 0) {
    int nextChunk = chunkSize;
    if (nextChunk > remaining) {
      nextChunk = remaining;
    }

    if (nextChunk != noint_block_write(s, &buf[sofar], nextChunk)) {
      result = false;
      return;
    }
    sofar += nextChunk;
    remaining -= nextChunk;

    // Sleep very briefly to keep from flooding the receiver in UDP tests.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  result = true;
  return;
}

int main(int argc, const char* argv[])
{

  // Test closing a bad socket.
  {
    SOCKET s = BAD_SOCKET;
    if (-100 != close_socket(s)) {
      std::cerr << "Error closing BAD_SOCKET" << std::endl;
      return 1;
    }
  }

  // Test creating and destroying both types of server sockets
  // using the most-basic open command.
  {
    SOCKET s;
    s = open_socket(SOCK_STREAM, nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening stream socket on any port and interface" << std::endl;
      return 101;
    }
    if (!set_tcp_socket_options(s)) {
      std::cerr << "Error setting stream socket options on any port and interface" << std::endl;
      return 102;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing stream socket on any port and interface" << std::endl;
      return 103;
    }

    s = open_socket(SOCK_DGRAM, nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening datagram socket on any port and interface" << std::endl;
      return 104;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing datagram socket on any port and interface" << std::endl;
      return 105;
    }
  }

  // Test creating and destroying both types of server sockets
  // using the type-specific open commands.
  {
    SOCKET s;
    s = open_tcp_socket(nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening TCP socket on any port and interface" << std::endl;
      return 201;
    }
    if (!set_tcp_socket_options(s)) {
      std::cerr << "Error setting TCP socket options on any port and interface" << std::endl;
      return 202;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing TCP socket on any port and interface" << std::endl;
      return 203;
    }

    s = open_udp_socket(nullptr, nullptr);
    if (s == BAD_SOCKET) {
      std::cerr << "Error opening UDP socket on any port and interface" << std::endl;
      return 204;
    }
    if (0 != close_socket(s)) {
      std::cerr << "Error closing UDP socket on any port and interface" << std::endl;
      return 205;
    }
  }

  // Test opening TCP server socket and a remote on different
  // threads and sending a bunch of data between them.  We use different threads to
  // avoid blocking when the network buffers get full.
  {
    // Construct and connect our writing and reading sockets.  First we make a listening socket
    // then connect a read to it and accept the write on it.
    int port;
    SOCKET lSock = get_a_TCP_socket (&port);
    if (lSock == BAD_SOCKET) {
      std::cerr << "Error Opening listening socket on arbitrary port" << std::endl;
      return 301;
    }
    SOCKET rSock;
    if (!connect_tcp_to("localhost", port, nullptr, &rSock)) {
      std::cerr << "Error Opening read socket" << std::endl;
      return 302;
    }
    if (!set_tcp_socket_options(rSock)) {
      std::cerr << "Error setting TCP socket options on arbitrary port" << std::endl;
      return 303;
    }
    SOCKET wSock;
    if (1 != poll_for_accept(lSock, &wSock, 10.0)) {
      std::cerr << "Error Opening write socket" << std::endl;
      return 304;
    }
    if (!set_tcp_socket_options(wSock)) {
      std::cerr << "Error setting TCP socket options on write socket" << std::endl;
      return 305;
    }

    // Store the results of our threads, testing reading and writing.
    bool writeWorked = false, readWorked = false;
    std::thread wt(TestWriteToSocket, std::ref(writeWorked), wSock, 1000000, 65000);
    std::thread rt(TestReadFromSocket, std::ref(readWorked), rSock, 1000000, 65000);
    wt.join();
    rt.join();
    if (!writeWorked) {
      std::cerr << "Writing to socket failed" << std::endl;
      return 310;
    }
    if (!readWorked) {
      std::cerr << "Reading from socket failed" << std::endl;
      return 311;
    }

    // Done with the sockets
    if (0 != close_socket(wSock)) {
      std::cerr << "Error closing write socket on any port and interface" << std::endl;
      return 320;
    }
    if (0 != close_socket(lSock)) {
      std::cerr << "Error closing listening socket on any port and interface" << std::endl;
      return 321;
    }
    if (0 != close_socket(rSock)) {
      std::cerr << "Error closing read socket on any port and interface" << std::endl;
      return 322;
    }
  }

  // Test opening UDP server socket and a remote on different
  // threads and sending a bunch of data between them.  We use different threads to
  // avoid blocking when the network buffers get full.
  {
    // Construct and connect our writing and reading sockets.  First we make a server socket
    // then connect a client to it.
    // We set the port number to 0 to select "any port".
    unsigned short port = 0;
    SOCKET sSock = open_udp_socket(&port, "localhost");
    if (sSock == BAD_SOCKET) {
      std::cerr << "Error Opening UDP socket on arbitrary port" << std::endl;
      return 401;
    }
    SOCKET rSock = connect_udp_port("localhost", port, nullptr);
    if (rSock == BAD_SOCKET) {
      std::cerr << "Error Opening UDP remote socket" << std::endl;
      return 402;
    }

    // Store the results of our threads, testing reading and writing.
    bool writeWorked = false, readWorked = false;
    std::thread wt(TestWriteToSocket, std::ref(writeWorked), rSock, 1000000, 65000);
    std::thread rt(TestReadFromSocket, std::ref(readWorked), sSock, 1000000, 65000);
    wt.join();
    rt.join();
    if (!writeWorked) {
      std::cerr << "Writing to UDP socket failed" << std::endl;
      return 410;
    }
    if (!readWorked) {
      std::cerr << "Reading from UDP socket failed" << std::endl;
      return 411;
    }

    // Done with the sockets
    if (0 != close_socket(sSock)) {
      std::cerr << "Error closing UDP server socket on any port and interface" << std::endl;
      return 420;
    }
    if (0 != close_socket(rSock)) {
      std::cerr << "Error closing UDP remote socket on any port and interface" << std::endl;
      return 421;
    }
  }

  /// @todo Test reuseAddr parameter to open_socket() on both TCP and UDP.

  /// @todo More tests

  std::cout << "Success!" << std::endl;
  return 0;
}
