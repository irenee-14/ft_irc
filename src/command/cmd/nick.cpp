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
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " NICK :" + nickname +
                   "\r\n";

  sendString(se, fd);

  // nick, user 다 들어왔을 때 welcome, daily message 보내는 함수 빼기
  if (clients[fd].getNick() != "" && clients[fd].getUser() != "") {
    clients[fd].setTimestamp(time(0));
    std::string se = ":" + clients[fd].getServerName() + " 001 " +
                     clients[fd].getNick() + " :Welcome\r\n";
    sendString(se, fd);
  }
}
