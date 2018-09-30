#include <iostream>
#include <stdexcept>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include "../common/socket.hpp"

void set_reuse_addr(int sd) {
  int yes = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    ::close(sd);
    return;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " port" << std::endl;
    return 0;
  }
  int port = std::stoi(std::string(argv[1]));

  int listener = socket(PF_INET, SOCK_STREAM, 0);  // IPPROTO_TCP
  if (listener < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  int yes = 1;
  if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    ::close(listener);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listener, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  int queue_size = 3;
  if (listen(listener, queue_size) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (true) {
    std::cout << "start new accept \n";
    int sock = accept(listener, NULL, NULL);
    std::cout << "accepted " << sock << " \n";

    char buf[1024];
    int bytes_read = recv(sock, buf, 1024, 0);
    printf("data received, size == %d \n", bytes_read);
    if (bytes_read <= 0) {
      printf("Client disconnected \n");
      break;
    }

    send(sock, buf, bytes_read, 0);
    close(sock);
  }

  close(listener);

  return 0;
}
