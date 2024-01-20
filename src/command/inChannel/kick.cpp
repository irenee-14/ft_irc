#include "Server.hpp"

void Server::kick(int fd, std::vector<std::string> tokens) {
  std::string name = tokens[1].substr(1, tokens[1].size() - 1);
  std::string target = tokens[2];

  // 채널 이름으로 채널 찾기
  // unsigned int i = 0;
  // for (; i < channels.size(); i++) {
  //   if (channels[i].getChannelName() == name) {
  //     break;
  //   }
  // }

  int channel_idx = isChannel(name);

  if (channel_idx >= 0) {
    std::string se = "";

    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      // for (unsigned int j = 0; j < user_fds.size(); j++) {
      //   if (channels[channel_idx].getUserNicks()[j] == tokens[2]) {
      if (channels[channel_idx].isUser(target) >= 0) {
        se += ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
              "@" + clients[fd].getServerName() + " KICK " + tokens[1] + " " +
              target + " :" + tokens[3] + "\r\n";

        sendString(se, channels[channel_idx].getUserFds());

        channels[channel_idx].removeUser(target);

        return;
      }

      // 보내려는 유저가 없으면 에러
      se += ":" + clients[fd].getServerName() + " 401 " +
            clients[fd].getNick() + " " + target + " :No such nick\r\n";
    } else {
      se += ":" + clients[fd].getServerName() + " 482 " +
            clients[fd].getNick() + " " + tokens[1] +
            " :You must be a channel operator\r\n";
    }
    sendString(se, fd);
  }
  // !!!!! else
  // 채널이 없는 경우
}
