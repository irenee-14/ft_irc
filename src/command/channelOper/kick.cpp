#include "Server.hpp"

void Server::kick(int fd, std::vector<std::string> tokens) {
  const std::string channel = tokens[1];
  const std::string channelNoHash = channel.substr(1, channel.size() - 1);
  const std::string target = tokens[2];
  const std::string message = tokens[3];

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // 채널 이름으로 채널 찾기
  int channel_idx = isChannel(channelNoHash);

  if (channel_idx >= 0) {
    std::string se = "";

    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      // kick하려는 target이 channel에 존재하면 메시지 보내기
      if (channels[channel_idx].isUser(target) >= 0) {
        se = ":" + nickname + "!" + username + "@" + servername + " KICK " +
             channel + " " + target + " :" + message + "\r\n";

        sendString(se, channels[channel_idx].getUserFds());

        channels[channel_idx].removeUser(target);
        return;
      }

      // 보내려는 유저가 없으면 에러
      se = ":" + SERVER_NAME + " 401 " + nickname + " " + target +
           " :No such nick\r\n";
    } else {
      se = ":" + SERVER_NAME + " 482 " + nickname + " " + channel +
           " :You must be a channel operator\r\n";
    }
    sendString(se, fd);
  }
  // !!!!! else
  // 채널이 없는 경우???
}
