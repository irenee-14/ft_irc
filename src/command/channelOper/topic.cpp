#include "Server.hpp"

// TOPIC #hi :hi
// :joyoo!root@127.0.0.1 TOPIC #hi :hi

void Server::topic(int fd, std::vector<std::string> tokens) {
  const std::string channel = tokens[1];
  const std::string channelNoHash = tokens[1].substr(1, tokens[1].size() - 1);
  const std::string topic = tokens[2];

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // 채널 이름으로 채널 찾기
  int channel_idx = isChannel(channelNoHash);
  if (channel_idx >= 0) {
    std::string se = "";

    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      channels[channel_idx].setTopic(topic);

      se += ":" + nickname + "!" + username + "@" + servername + " TOPIC " +
            channel + " :" + topic + "\r\n";
      sendString(se, channels[channel_idx].getUserFds());
      return;
    } else {
      se += ":" + SERVER_NAME + " 482 " + nickname + " " + channel +
            " :You do not have access to change the topic on this channel\r\n";
    }
    sendString(se, fd);
    // ?????? sendstring 여기서 해야하나? 엔터쳐져서 그런가?
  }
  // !!!else
  // 채널이 없는 경우
  // mode +t 일때 운영자만 가능 추가하기
}
