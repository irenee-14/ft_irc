#include "Server.hpp"

void Server::pass(int fd, std::string password) {
  if (password != this->_password) {
    throw std::string("wrong password");
  }
  // fd에 해당하는 client의 pass_flag를 true로 바꿔줌
  clients[fd].setPassFlag(true);
}
