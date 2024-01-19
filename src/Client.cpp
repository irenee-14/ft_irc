#include "Client.hpp"

#include <time.h>

#include <iostream>
Client::Client(void) : _timestamp(time(0)) {}

Client::Client(int fd) : _clnt_fd(fd), _timestamp(time(0)) {}

Client::Client(const Client& src) : _timestamp(time(0)) { *this = src; }

Client::~Client(void) {}

Client& Client::operator=(Client const& rhs) {
  if (this != &rhs) {
    _clnt_fd = rhs._clnt_fd;
    _nick = rhs._nick;
    _user = rhs._user;
    _real_name = rhs._real_name;
    _server_name = rhs._server_name;
    _pass_flag = rhs._pass_flag;
    _timestamp = rhs._timestamp;
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
void Client::setPassFlag(const bool flag) { _pass_flag = flag; }
void Client::setTimestamp(const u_int32_t timestamp) { _timestamp = timestamp; }

int Client::getFd(void) const { return (_clnt_fd); }
std::string Client::getNick(void) const { return (_nick); }
std::string Client::getUser(void) const { return (_user); }
std::string Client::getRealName(void) const { return (_real_name); }
std::string Client::getServerName(void) const { return (_server_name); }
u_int32_t Client::getTimestamp(void) const { return (_timestamp); }
bool Client::getPassFlag(void) const { return (_pass_flag); }