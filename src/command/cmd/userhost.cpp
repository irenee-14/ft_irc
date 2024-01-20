#include "Server.hpp"

void Server::userhost(int fd, std::vector<std::string> tokens) {
  std::string se = ":" + clients[fd].getServerName() + " 302 " +
                   clients[fd].getNick() + " :";
  for (unsigned int i = 1; i < tokens.size(); ++i) {
    se += clients[fd].getNick() + "=+" + clients[fd].getUserFd() + "@" +
          clients[fd].getServerName() + " ";
  }
  se += "\r\n";

  sendString(se, fd);
}
