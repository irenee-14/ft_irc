#include "Server.hpp"

void Server::nick(int fd, std::string token) {
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " NICK :" + token +
                   "\r\n";

  clients[fd].setNick(token);

  sendString(se, fd);
}
