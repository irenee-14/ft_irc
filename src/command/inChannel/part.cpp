#include "Server.hpp"

void Server::part(int fd, std::string token) {
  // 채널 나가는데 마지막이면 채널 없애기
  std::string name = token.substr(1, token.size() - 1);
  unsigned int i = 0;
  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      break;
    }
  }
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   " PART :#" + name + "\r\n";

  sendString(se, channels[i].getUserFds());
  // users에서 지우기
  channels[i].removeUser(fd, channels[i].getUserFds());
  // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
  if (channels[i].getUserFds().empty()) {
    channels.erase(channels.begin() + i);
  }
}
