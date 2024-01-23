#include "Server.hpp"

void Server::part(int fd, std::string channel) {
  // 채널 나가는데 마지막이면 채널 없애기
  if (channels.size() < 1) return;

  std::string channelNoHash = channel.substr(1, channel.size() - 1);

  int channel_idx = isChannel(channelNoHash);

  if (channel[0] != '#' || channel_idx < 0) {
    std::string se = ":127.0.0.1 403 " + clients[fd].getNick() + " " + channel +
                     " :No such channel\r\n";
    sendString(se, fd);

    return;
  } else {
    std::string se = ":" + clients[fd].getNick() + "!" +
                     clients[fd].getUserFd() + " PART :" + channel + "\r\n";

    sendString(se, channels[channel_idx].getUserFds());

    // users에서 지우기
    channels[channel_idx].removeUser(fd);

    // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
    if (channels[channel_idx].getUserFds().empty()) {
      channels.erase(channels.begin() + channel_idx);
    }
  }
}
