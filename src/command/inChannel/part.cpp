#include "Server.hpp"

void Server::part(int fd, std::string token) {
  // 채널 나가는데 마지막이면 채널 없애기
  if (channels.size() < 1) return;

  std::string name = token.substr(1, token.size() - 1);

  // unsigned int i = 0;
  // for (; i < channels.size(); i++) {
  //   if (channels[i].getChannelName() == name) {
  //     break;
  //   }
  // }
  unsigned int channel_idx = isChannel(name);

  if (token[0] != '#' || channel_idx == channels.size()) {
    std::string se = ":127.0.0.1 403 " + clients[fd].getNick() + " " + token +
                     " :No such channel\r\n";
    sendString(se, fd);

    return;
  } else {
    std::string se = ":" + clients[fd].getNick() + "!" +
                     clients[fd].getUserFd() + " PART :#" + name + "\r\n";

    sendString(se, channels[channel_idx].getUserFds());

    // users에서 지우기
    channels[channel_idx].removeUser(fd);

    // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
    if (channels[channel_idx].getUserFds().empty()) {
      channels.erase(channels.begin() + channel_idx);
    }
  }
}
