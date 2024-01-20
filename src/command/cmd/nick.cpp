#include "Server.hpp"

void Server::nick(int fd, std::string token) {
  // // nick 중복 확인
  // // :irc.local 433 * root :Nickname is already in use.
  for (unsigned int i = 0; i < clients.size(); ++i) {
    if (clients[i].getNick() == token) {
      // 입력된 nick 뒤에 _붙이기
      std::string se =
          ":127.0.0.1 433 * " + token + " :Nickname is already in use.\r\n";

      sendString(se, fd);
      return;
    }
  }

  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " NICK :" + token +
                   "\r\n";

  clients[fd].setNick(token);
  sendString(se, fd);
}
