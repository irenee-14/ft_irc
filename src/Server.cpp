#include "Server.hpp"

#include <arpa/inet.h>  // htons
#include <fcntl.h>      //fcntl
#include <sys/socket.h>

#include <iostream>

void error_handling(const std::string& message) {
  std::cerr << message << std::endl;
  exit(1);
}

Server::Server(void) {}

Server::Server(char** argv) {
  // 서버 소켓 생성 serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (this->serv_fd == -1) {
    error_handling("socket() error");
  }

  // 서버에 소켓 주소 생성
  memset(&serv_adr, 0, sizeof(this->serv_adr));
  this->serv_adr.sin_family = AF_INET;
  this->serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->serv_adr.sin_port = htons(atoi(argv[1]));

  int enable = 1;
  if (setsockopt(this->serv_fd, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(enable)) == -1)
    error_handling("setsockopt() error");

  if (fcntl(this->serv_fd, F_SETFL, O_NONBLOCK) == -1)
    error_handling("fcntl() error");

  // 소켓과 서버 주소 연결
  if (bind(this->serv_fd, (struct sockaddr*)&this->serv_adr,
           sizeof(this->serv_adr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(this->serv_fd, 5) == -1) {
    error_handling("listen() error");
  }
  fds[0].fd = this->serv_fd;
  fds[0].events = POLLIN;
}

Server::Server(const Server& src) { *this = src; }

Server::~Server(void) {}

Server& Server::operator=(Server const& rhs) {
  if (this != &rhs) {
    *this = rhs;
  }
  return (*this);
}

const int Server::getServFd() const { return (this->serv_fd); }

const std::vector<struct pollfd> Server::getPollFds() const {
  return (this->fds);
}
