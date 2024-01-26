#include "Server.hpp"

void Server::user(int fd, std::vector<std::string> tokens) {
  clients[fd].setUser(tokens[1]);
  clients[fd].setServerName(tokens[3]);
  clients[fd].setRealName(tokens[4]);

  printArg("\n-------- set user --------\n nick name   : ",
           clients[fd].getNick());
  printArg(" user name   : ", clients[fd].getUser());
  printArg(" server name : ", clients[fd].getServerName());
  printArg(" real name   : ", clients[fd].getRealName());
  printArg("--------------------------\n", 0);
}
