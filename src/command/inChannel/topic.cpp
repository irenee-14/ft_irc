#include "Server.hpp"

// TOPIC #hi :hi
// :joyoo!root@127.0.0.1 TOPIC #hi :hi

void Server::topic(int fd, std::vector<std::string> tokens) {
  std::string channel = tokens[1];
  std::string channelNoHash = tokens[1].substr(1, tokens[1].size() - 1);
  std::string topic = tokens[2];

  // 채널 이름으로 채널 찾기
  int channel_idx = isChannel(channelNoHash);
  if (channel_idx >= 0) {
    std::string se = "";

    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      channels[channel_idx].setTopic(topic);

      se += ":" + clients[fd].getNick() + "!" + clients[fd].getUser() + "@" +
            clients[fd].getServerName() + " TOPIC " + channel + " :" + topic +
            "\r\n";

      sendString(se, channels[channel_idx].getUserFds());

      return;
    } else {
      se += ":" + clients[fd].getServerName() + " 482 " +
            clients[fd].getNick() + " " + channel +
            " :You do not have access to change the topic on this channel\r\n";
    }
    sendString(se, fd);
  }
  // !!!else
  // 채널이 없는 경우
  // mode +t 일때 운영자만 가능 추가하기
}
