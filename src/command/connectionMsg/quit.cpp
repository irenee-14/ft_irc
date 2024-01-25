#include "Server.hpp"

void Server::quit(int fd) {
  for (unsigned int i = 0; i < channels.size(); ++i) {
    // 채널에 user가 포함되어 있는지 확인
    if (channels[i].isUser(fd) >= 0) {
      // operator인 경우 operator 목록에서 제거

      if (channels[i].isOperator(fd) >= 0) {
        channels[i].removeOperator(fd);
      }

      channels[i].removeUser(fd);

      // 채널에 속한 모든 user에게 quit메시지 보내기
      if (channels[i].getUserFds().size() > 0) {
        std::string se = ":" + clients[fd].getNick() + "!" +
                         clients[fd].getUser() + "@" +
                         clients[fd].getServerName() + " QUIT :leaving\r\n";
        sendString(se, channels[i].getUserFds());
      }
    }
  }
  // server에 quit 메시지 보내기
  std::string se = "ERROR :Closing link: (" + clients[fd].getUser() + "@" +
                   clients[fd].getServerName() + ") [Quit: leaving]\r\n";

  sendString(se, fd);
}