#include "Server.hpp"

void Server::pong(int fd) {
  const char* se = ":127.0.0.1 PONG 127.0.0.1 :127.0.0.1\r\n";
  sendString(se, fd);
}
