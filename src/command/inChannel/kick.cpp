#include "Server.hpp"

void Server::kick(int fd, std::vector<std::string> tokens) {
  std::string channel = tokens[1];
  std::string channelNoHash = channel.substr(1, channel.size() - 1);
  std::string target = tokens[2];

  // 채널 이름으로 채널 찾기
  int channel_idx = isChannel(channelNoHash);

  if (channel_idx >= 0) {
    std::string se = "";

    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      // kick하려는 target이 channel에 존재하면 메시지 보내기
      if (channels[channel_idx].isUser(target) >= 0) {
        se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() + "@" +
             clients[fd].getServerName() + " KICK " + channel + " " + target +
             " :" + tokens[3] + "\r\n";

        sendString(se, channels[channel_idx].getUserFds());

        channels[channel_idx].removeUser(target);

        return;
      }

      // 보내려는 유저가 없으면 에러
      se = ":" + clients[fd].getServerName() + " 401 " + clients[fd].getNick() +
           " " + target + " :No such nick\r\n";
    } else {
      se = ":" + clients[fd].getServerName() + " 482 " + clients[fd].getNick() +
           " " + channel + " :You must be a channel operator\r\n";
    }
    sendString(se, fd);
  }
  // !!!!! else
  // 채널이 없는 경우???
}
