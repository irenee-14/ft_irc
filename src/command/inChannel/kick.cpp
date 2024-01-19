#include "Server.hpp"

void Server::kick(int fd, std::vector<std::string> tokens) {
  std::string name = tokens[1].substr(1, tokens[1].size() - 1);

  // 채널 이름으로 채널 찾기
  unsigned int i = 0;
  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      break;
    }
  }

  std::string se = "";

  // operator인지 확인
  if (channels[i].isOperator(fd)) {
    for (unsigned int j = 0; j < channels[i].getUserFds().size(); j++) {
      if (channels[i].getUserNicks()[j] == tokens[2]) {
        se += ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
              "@" + clients[fd].getServerName() + " KICK " + tokens[1] + " " +
              tokens[2] + " :" + tokens[3] + "\r\n";
        sendString(se, channels[i].getUserFds());
        channels[i].removeUser(tokens[2], channels[i].getUserNicks());
        return;
      }
    }
    // 보내려는 유저가 없으면 에러
    se += ":" + clients[fd].getServerName() + " 401 " + clients[fd].getNick() +
          " " + tokens[2] + " :No such nick\r\n";

  } else {
    se += ":" + clients[fd].getServerName() + " 482 " + clients[fd].getNick() +
          " " + tokens[1] + " :You must be a channel operator\r\n";
  }
  sendString(se, fd);
}
