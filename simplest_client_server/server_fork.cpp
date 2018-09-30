#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <string>

void set_reuse_addr(int sd) {
  int yes = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    ::close(sd);
    return;
  }
}

int make_named_socket(const char* filename) {
  struct sockaddr_un name;
  int sock;
  size_t size;
  sock = socket(PF_UNIX, SOCK_DGRAM, 0); // create unix domain socket
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  name.sun_family = PF_UNIX;
  strcpy(name.sun_path, filename);
  size = (offsetof(struct sockaddr_un, sun_path) + strlen(name.sun_path) + 1);
  if (bind(sock, (struct sockaddr*)&name, size) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  return sock;
}

int make_socket(uint16_t port) {
  /* Create the socket. */
  // sock = socket(PF_INET, SOCK_STREAM, 0);
  int sock = socket(PF_INET, SOCK_STREAM, 0);  // IPPROTO_TCP
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  set_reuse_addr(sock);
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  int queue_size = 3;
  if (::listen(sock, queue_size) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  exit(0);
  return sock;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " port" << std::endl;
    return 0;
  }
  int sock;
  int port = std::stoi(std::string(argv[1]));

  int listener = socket(PF_INET, SOCK_STREAM, 0);  // IPPROTO_TCP
  if (listener < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  set_reuse_addr(listener);

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

  char buf[1024];
  int bytes_read;

  while (true) {
    printf("waiting for connection and then accept it, pid = %d \n", getpid());
    sock = accept(listener, NULL, NULL);
    printf("request accepted , pid = %d \n", getpid());
    if (sock < 0) {
      perror("accept");
      exit(3);
    }

    pid_t pid = fork();
    if (pid > 0) {
      std::cerr << "parent: " << getpid() << std::endl;
    } else {
      std::cerr << "child: " << getpid() << std::endl;
    }

    switch (pid) {
      case -1:
        perror("fork");
        break;

      case 0:
        close(listener);  // потомок закрывает Ненужную копия слушающего сокета
        while (true) {
          bytes_read = recv(sock, buf, 1024, 0);
          printf("data received, size == %d , pid = %d \n", bytes_read,
                 getpid());
          if (bytes_read <= 0) {
            printf("Client disconnected \n");
            break;
          }
          ////

          send(sock, buf, bytes_read, 0);
        }

        close(sock);
        break;
      default:
        close(sock); // родитель закрывает свой дескриптор подключенного сокета, это ненужная копия
        break;
    }
  }

  close(listener);
  return 0;
}
