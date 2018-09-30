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
    s.connect(host, port, /*timeout, sec*/ 10);
    // Проверка что таймаут работает:
    // google.com 81
    // 192.168.0.0, 10.255.255.1, 192.168.255.255 e.t.c - existing, but no
    // routed hosts

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
