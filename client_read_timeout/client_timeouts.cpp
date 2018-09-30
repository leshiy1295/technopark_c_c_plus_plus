#include <errno.h>
#include <string.h>
#include <iostream>
#include <memory>  // shared_ptr
#include <stdexcept>
#include "../common/socket.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " host port" << std::endl;
    return 0;
  }

  try {
    std::string host(argv[1]);
    int port = std::stoi(argv[2]);

    Socket s;
    s.connect(host, port);
    s.setNonBlocked(true);
    s.recvTimed(3);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
