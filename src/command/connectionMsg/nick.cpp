#include "Server.hpp"

void Server::nick(int fd, std::string nickname) {
  // nick 중복 확인
  // :irc.local 433 * root :Nickname is already in use.

  // 기존 닉네임과 동일 → 무시
  if (clients[fd].getNick() == nickname) return;

  int nick_idx = isUser(nickname);
  if (nick_idx >= 0) {
    std::string se = ":" + SERVER_NAME + " 433 * " + nickname +
                     " :Nickname is already in use.\r\n";
    sendString(se, fd);
    return;
  }

  // nickname 설정
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " NICK :" + nickname +
                   "\r\n";
  clients[fd].setNick(nickname);
  sendString(se, fd);
}
