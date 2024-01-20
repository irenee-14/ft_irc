#include "Server.hpp"

// TOPIC #hi :hi
// :joyoo!root@127.0.0.1 TOPIC #hi :hi

void Server::topic(int fd, std::vector<std::string> tokens) {
  std::string name = tokens[1].substr(1, tokens[1].size() - 1);

  // 채널 이름으로 채널 찾기
  // unsigned int i = 0;
  // for (; i < channels.size(); i++) {
  //   if (channels[i].getChannelName() == name) {
  //     break;
  //   }
  // }

  int channel_idx = isChannel(name);
  if (channel_idx >= 0) {
    std::string se = "";

    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      channels[channel_idx].setTopic(tokens[2]);

      se += ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() + "@" +
            clients[fd].getServerName() + " TOPIC " + tokens[1] + " :" +
            tokens[2] + "\r\n";

      sendString(se, channels[channel_idx].getUserFds());

      return;
    } else {
      se += ":" + clients[fd].getServerName() + " 482 " +
            clients[fd].getNick() + " " + tokens[1] +
            " :You do not have access to change the topic on this channel\r\n";
    }
    sendString(se, fd);
  }
  // !!!else
  // 채널이 없는 경우
  // mode +t 일때 운영자만 가능 추가하기
}
