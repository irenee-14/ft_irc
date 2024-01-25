#include "Server.hpp"

void Server::pong(int fd) {
  const std::string se =
      ":" + SERVER_NAME + " PONG " + SERVER_NAME + " :" + SERVER_NAME + "\r\n";
  sendString(se, fd);
}
