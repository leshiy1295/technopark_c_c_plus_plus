#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

void write_to_server(int filedes, std::string msg) {
  size_t left = msg.size();
  ssize_t sent = 0;

  int flags = 0;
  while (left > 0) {
    sent = ::send(filedes, msg.data() + sent, msg.size() - sent, flags);
    if (-1 == sent)
      throw std::runtime_error("write failed: " + std::string(strerror(errno)));

    left -= sent;
  }

  printf("write message %s to socket \n", msg.c_str());
}

void read_from_server(int filedes) {
  char buf[1024];
#ifdef __APPLE__
  // mac os x don't defines MSG_NOSIGNAL
  int n = ::recv(filedes, buf, sizeof(buf), 0);
#else
  int n = ::recv(filedes, buf, sizeof(buf), MSG_NOSIGNAL);
#endif

  if (-1 == n && errno != EAGAIN)
    throw std::runtime_error("read failed: " + std::string(strerror(errno)));
  if (0 == n)
    throw std::runtime_error("client: " + std::to_string(filedes) +
                             " disconnected");
  if (-1 == n)
    throw std::runtime_error("client: " + std::to_string(filedes) +
                             " timeouted");

  std::string ret(buf, buf + n);
  std::cerr << "client: " << filedes << ", recv: \n"
            << ret << " [" << n << " bytes]" << std::endl;
}

void init_sockaddr(struct sockaddr_in* name,
                   const char* hostname,
                   uint16_t port) {
  name->sin_family = AF_INET;
  name->sin_port = htons(port);

  struct hostent* hostinfo;
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s.\n", hostname);
    exit(EXIT_FAILURE);
  }
  name->sin_addr = *(struct in_addr*)hostinfo->h_addr;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " host port" << std::endl;
    return 0;
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket (client)");
    exit(EXIT_FAILURE);
  }

  std::string host(argv[1]);
  int port = std::stoi(argv[2]);
  struct sockaddr_in servername;
  init_sockaddr(&servername, host.c_str(), port);

  if (0 > connect(sock, (struct sockaddr*)&servername, sizeof(servername))) {
    perror("connect (client)");
    exit(EXIT_FAILURE);
  }

  write_to_server(sock, "This is my first message");
  read_from_server(sock);

  close(sock);
  exit(EXIT_SUCCESS);
}
