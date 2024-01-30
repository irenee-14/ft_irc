#include "Server.hpp"

void Server::part(int fd, std::vector<std::string> tokens) {
  const std::string channelString = tokens[1];
  const std::string partMessage = tokens.size() > 2 ? tokens[2] : "";

  const std::vector<std::string> channel_vec = splitComma(channelString, 0);
  for (unsigned int i = 0; i < channel_vec.size(); ++i) {
    const std::string channel = channel_vec[i];
    const std::string channelNoHash = channel.substr(1, channel.size() - 1);
    const std::string nickname = clients[fd].getNick();
    const std::string username = clients[fd].getUser();

    int channel_idx = isChannel(channelNoHash);

    if (channel[0] != '#' || channel_idx < 0) {
      std::string se = ":" + SERVER_NAME + " 403 " + nickname + " " + channel +
                       " :No such channel\r\n";
      sendString(se, fd);
      continue;
    }
    // 채널에 없을 경우
    // :irc.local 442 root #hi :You're not on that channel
    else if (channels[channel_idx].isUser(fd) < 0) {
      std::string se = ":" + SERVER_NAME + " 442 " + nickname + " " + channel +
                       " :You're not on that channel\r\n";
      sendString(se, fd);
      continue;
    } else {
      if (partMessage != "") {
        std::string se = ":" + nickname + "!" + username + " PART " + channel +
                         " :" + partMessage + "\r\n";
        sendString(se, channels[channel_idx].getUserFds());
      } else {
        std::string se =
            ":" + nickname + "!" + username + " PART :" + channel + "\r\n";
        sendString(se, channels[channel_idx].getUserFds());
      }

      // users에서 지우기
      channels[channel_idx].removeUser(fd);

      // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
      if (channels[channel_idx].getUserFds().empty()) {
        channels.erase(channels.begin() + channel_idx);
      }
    }
  }
}
