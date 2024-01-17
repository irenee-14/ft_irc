#include "Client.hpp"

Client::Client(void) {}

Client::Client(int fd) : _clnt_fd(fd) {}

Client::Client(const Client& src) { *this = src; }

Client::~Client(void) {}

Client& Client::operator=(Client const& rhs) {
  if (this != &rhs) {
    _clnt_fd = rhs._clnt_fd;
    _nick = rhs._nick;
    _user = rhs._user;
    _real_name = rhs._real_name;
    _server_name = rhs._server_name;
  }
  return (*this);
}

void Client::setFd(const int fd) { _clnt_fd = fd; }
void Client::setNick(const std::string nick) { _nick = nick; }
void Client::setUser(const std::string user) { _user = user; }
void Client::setRealName(const std::string real) { _real_name = real; }
void Client::setServerName(const std::string server_name) {
  _server_name = server_name;
}

int Client::getFd(void) const { return (_clnt_fd); }
std::string Client::getNick(void) const { return (_nick); }
std::string Client::getUser(void) const { return (_user); }
std::string Client::getRealName(void) const { return (_real_name); }
std::string Client::getServerName(void) const { return (_server_name); }
