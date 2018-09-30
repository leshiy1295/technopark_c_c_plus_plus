//
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <string.h>
#include <cstring>

#define POLL_SIZE 32
#define LISTEN_QUEUE 5

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
  int listener;
  int port = std::stoi(std::string(argv[1]));
  listener = make_socket(port);
  set_non_blocked_impl(listener, true);
  // int sock = accept(listener, NULL, NULL); will not block

  int numfds = 0;
  struct pollfd fds[POLL_SIZE];
  fds[0].fd = listener;
  fds[0].events = POLLIN;
  fds[0].revents = 0;
  numfds++;

  while (true) {
    // Заполняем множество сокетов
    // Задаём таймаут
    int timeout = 10000;

    // Ждём события в одном из сокетов
    int poll_ret = poll(fds, numfds, /* timeout in msec */ timeout);
    if (poll_ret == 0) {
      continue;
    } else if (poll_ret == -1) {
      throw std::runtime_error(std::string("poll: ") + strerror(errno));
    }

    for (size_t i = 0; i < numfds; ++i) {
      if (fds[i].revents == 0)
        continue;

      if (fds[i].fd == listener) {
        // Определяем тип события и выполняем соответствующие действия
        // Поступил новый запрос на соединение, используем accept
        printf("request received. accept it\n");
        int sock = accept(listener, NULL, NULL);
        if (sock < 0) {
          perror("accept");
          exit(3);
        }

        fds[numfds].fd = sock;
        fds[numfds].events = POLLIN;
        fds[numfds].revents = 0;
        numfds++;
        set_non_blocked_impl(sock, true);
        printf("request accepted\n");
        continue;
      }

      if (fds[i].revents & POLLHUP) {
        // e.g. previous write() was in a already closed sd
        fds[i].revents = 0;
        close(fds[i].fd);
        std::cerr << "client hup\n";
      } else if (fds[i].revents & POLLIN) {
        fds[i].revents = 0;
        char buf[256];
        ssize_t r = read(fds[i].fd, buf, sizeof(buf));
        if (r < 0) {
          std::cerr << "some read error!\n";
          close(fds[i].fd);
          continue;
        } else if (r == 0) {
          std::cerr << "client disconnected: " << fds[i].fd << "\n";
          close(fds[i].fd);
          continue;
        }

        buf[r] = '\0';

        std::string tmp(buf);
        while (tmp[tmp.size() - 1] == '\r' || tmp[tmp.size() - 1] == '\n')
          tmp.pop_back();

        if (r > 0) {
          tmp += "\n";
          std::cerr << "read: " << r << " bytes [" << tmp.c_str() << "]\n";
          fds[i].events = POLLOUT;
          // write(fds[i].fd, &tmp[0], tmp.length()); // don't do like that, use
          // POLLOUT
        }

      } else if (fds[i].revents & POLLOUT) {
        send(fds[i].fd, "hello\n", sizeof("hello"), 0);
        fds[i].events = POLLIN;
      }
    }
  }

  return 0;
}
