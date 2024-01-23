#include "Server.hpp"

void Server::user(int fd, std::vector<std::string> tokens) {
  clients[fd].setUser(tokens[1]);
  clients[fd].setServerName(tokens[3]);
  clients[fd].setRealName(tokens[4]);

  std::cout << "\n결과 : " << clients[fd].getNick() << " "
            << clients[fd].getUserFd() << " " << clients[fd].getServerName()
            << " " << clients[fd].getRealName() << std::endl
            << std::endl;
}
