#include "Server.hpp"

// TOPIC #hi :hi
// :joyoo!root@127.0.0.1 TOPIC #hi :hi

void Server::topic(int fd, std::vector<std::string> tokens) {
  const std::string channel = tokens[1];
  if (tokens.size() < 2) {
    const std::string se = ":" + SERVER_NAME + " 461 " + clients[fd].getNick() +
                           " TOPIC :Not enough parameters.\r\n";
    sendString(se, fd);
    return;
  }
  const std::string channelNoHash = tokens[1].substr(1, tokens[1].size() - 1);
  const std::string topic = tokens[2];

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // 채널 이름으로 채널 찾기
  int channel_idx = isChannel(channelNoHash);
  // 채널이 없는 경우
  if (channel_idx < 0) {
    const std::string se = ":" + SERVER_NAME + " 403 " + nickname + " " +
                           channel + " :No such channel\r\n";
    sendString(se, fd);
    return;
  }

  // TOPIC #hi
  // 채널에 속하지 않은 user가 topic 확인하려고 할 때
  // 토픽이 없는 경우
  // :irc.local 331 root_ #hi :No topic is set.
  // 토픽이 설정되어 있는 경우
  // :irc.local 332 root_ #hi :hello
  if (channels[channel_idx].isUser(fd) < 0) {
    // topic을 변경하려고 할 경우 에러
    // :irc.local 442 root #asdf :You're not on that channel!
    if (tokens.size() > 2) {
      const std::string se = ":" + SERVER_NAME + " 442 " + nickname + " " +
                             channel + " :You're not on that channel!\r\n";
      sendString(se, fd);
      return;
    }
    const std::string get_topic = channels[channel_idx].getTopic();
    if (get_topic == "") {
      const std::string se = ":" + SERVER_NAME + " 331 " + nickname + " " +
                             channel + " :No topic is set.\r\n";
      sendString(se, fd);
    } else {
      const std::string se = ":" + SERVER_NAME + " 332 " + nickname + " " +
                             channel + " :" + get_topic + "\r\n";
      sendString(se, fd);
    }
    return;
  }

  // operator인지 확인
  if (!channels[channel_idx].getTopicMode() ||
      (channels[channel_idx].getTopicMode() &&
       channels[channel_idx].isOperator(fd) >= 0)) {
    channels[channel_idx].setTopic(topic);

    const std::string se = ":" + nickname + "!" + username + "@" + servername +
                           " TOPIC " + channel + " :" + topic + "\r\n";
    sendString(se, channels[channel_idx].getUserFds());
  } else {
    const std::string se =
        ":" + SERVER_NAME + " 482 " + nickname + " " + channel +
        " :You do not have access to change the topic on this channel\r\n";
    sendString(se, fd);
  }
}
