#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <string.h>
#include <cstring>

// using namespace std;
#define MAXMSG 512

void set_reuse_addr(int sd) {
  int yes = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    ::close(sd);
    return;
  }
}

int make_socket(uint16_t port) {
  /* Create the socket. */
  // sock = socket(PF_INET, SOCK_STREAM, 0);
  int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  set_reuse_addr(sock);

  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  /* Give the socket a serv_addr. */
  serv_addr.sin_family = AF_INET;
  // htons -- host to net short
  // htonl -- host to net long
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

  return sock;
}

void set_non_blocked_impl(int sd, bool opt) throw(std::exception) {
  int flags = fcntl(sd, F_GETFL, 0);
  int new_flags = (opt) ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
  if (fcntl(sd, F_SETFL, new_flags) == -1)
    throw std::runtime_error("make nonblocked: " +
                             std::string(strerror(errno)));
}

int main(int argc, char* argv[]) {
  int listener = 0;
  char buf[1024];
  int bytes_read;
  int port = std::stoi(std::string(argv[1]));
  listener = make_socket(port);
  set_non_blocked_impl(listener, true);

  std::set<int> clients;
  clients.clear();

  while (true) {
    // Заполняем множество сокетов
    fd_set readset;
    std::cerr << "sizeof readset = " << sizeof(readset) << std::endl;
    FD_ZERO(&readset);
    FD_SET(listener, &readset);

    for (std::set<int>::iterator it = clients.begin(); it != clients.end();
         it++)
      FD_SET(*it, &readset);

    // Задаём таймаут
    timeval timeout;
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;

    // Ждём события в одном из сокетов
    int mx = listener;
    if (!clients.empty())
      mx = std::max(listener, *max_element(clients.begin(), clients.end()));

    if (select(mx + 1, &readset, NULL, NULL, &timeout) <= 0) {
      perror("select");
      exit(3);
    }

    // Определяем тип события и выполняем соответствующие действия
    if (FD_ISSET(listener, &readset)) {
      // Поступил новый запрос на соединение, используем accept
      printf("request received. accept it\n");
      int sock = accept(listener, NULL, NULL);
      if (sock < 0) {
        perror("accept");
        exit(3);
      }
      printf("request accepted\n");
      set_non_blocked_impl(sock, true);
      clients.insert(sock);
    }

    for (std::set<int>::iterator it = clients.begin(); it != clients.end();
         it++) {
      if (FD_ISSET(*it, &readset)) {
        // Поступили данные от клиента, читаем их
        bytes_read = recv(*it, buf, 1024, 0);
        printf("data received, size == %d\n, data = %s", bytes_read, buf);
        if (bytes_read <= 0) {
          // Соединение разорвано, удаляем сокет из множества
          close(*it);
          clients.erase(*it);
          // FD_CLR
          if (clients.empty())
            break;
          continue;
        }

        std::string ret(buf, buf + bytes_read);
        while (ret.back() == '\r' || ret.back() == '\n')
          ret.pop_back();

        // Отправляем данные обратно клиенту
        ret += " from server\n";
        send(*it, ret.c_str(), ret.size(), 0);
      }
    }
  }

  return 0;
}
