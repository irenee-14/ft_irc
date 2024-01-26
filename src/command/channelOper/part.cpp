#include "Server.hpp"

void Server::part(int fd, std::string channel) {
  // 채널 아무것도 없으면 무시
  if (channels.size() < 1) return;
  const std::string channelNoHash = channel.substr(1, channel.size() - 1);
  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();

  int channel_idx = isChannel(channelNoHash);

  if (channel[0] != '#' || channel_idx < 0) {
    std::string se = ":" + SERVER_NAME + " 403 " + nickname + " " + channel +
                     " :No such channel\r\n";
    sendString(se, fd);
    return;
  } else {
    std::string se =
        ":" + nickname + "!" + username + " PART :" + channel + "\r\n";
    sendString(se, channels[channel_idx].getUserFds());

    // users에서 지우기
    channels[channel_idx].removeUser(fd);

    // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
    if (channels[channel_idx].getUserFds().empty()) {
      channels.erase(channels.begin() + channel_idx);
    }
  }
}
