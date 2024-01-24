#include "Server.hpp"

void Server::nick(int fd, std::string nickname) {
  // nick 중복 확인
  // :irc.local 433 * root :Nickname is already in use.

  int nick_idx = isUser(nickname);
  if (nick_idx >= 0) {
    std::string se =
        ":127.0.0.1 433 * " + nickname + " :Nickname is already in use.\r\n";

    sendString(se, fd);
    return;
  }

  // nickname 설정
  clients[fd].setNick(nickname);
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " NICK :" + nickname +
                   "\r\n";

  clients[fd].setNick(nickname);
  sendString(se, fd);
}
