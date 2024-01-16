#include "Client.hpp"

Client::Client(void) {}

Client::Client(int fd) : clnt_fd(fd) {}

Client::Client(const Client& src) { *this = src; }

Client::~Client(void) {}

Client& Client::operator=(Client const& rhs) {
  if (this != &rhs) {
    clnt_fd = rhs.clnt_fd;
  }
  return (*this);
}

void Client::setFd(const int fd) { clnt_fd = fd; }

int Client::getFd(void) const { return (clnt_fd); }
