#include "Server.hpp"

void Server::kick(int fd, std::vector<std::string> tokens) {
  const std::string channel = tokens[1];
  const std::string channelNoHash = channel.substr(1, channel.size() - 1);
  const std::string target = tokens[2];
  const std::string message = tokens.size() > 3 ? tokens[3] : "";

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // 채널 이름으로 채널 찾기
  int channel_idx = isChannel(channelNoHash);

  if (channel_idx >= 0) {
    std::string se = "";

    // 채널에 속해있지 않으면 에러
    //  :irc.local 442 root_ #hi :You're not on that channel!
    if (channels[channel_idx].isUser(fd) < 0) {
      se = ":" + SERVER_NAME + " 442 " + nickname + " " + channel +
           " :You're not on that channel!\r\n";
      sendString(se, fd);
      return;
    }
    // operator인지 확인
    if (channels[channel_idx].isOperator(fd) >= 0) {
      // kick하려는 target이 channel에 존재하면 메시지 보내기
      int target_idx = channels[channel_idx].isUser(target);
      if (target_idx >= 0) {
        se = ":" + nickname + "!" + username + "@" + servername + " KICK " +
             channel + " " + target + " :" + message + "\r\n";

        sendString(se, channels[channel_idx].getUserFds());

        channels[channel_idx].removeUser(target);
        return;
      }

      // kick하려는 target이 channel에 없으면 에러
      if (isUser(target) >= 0) {
        se = ":" + SERVER_NAME + " 441 " + nickname + " " + target + " " +
             channel + " :They aren't on that channel\r\n";

        sendString(se, fd);
        return;
      }

      // kick하려는 target이 존재하지 않으면 에러
      se = ":" + SERVER_NAME + " 401 " + nickname + " " + target +
           " :No such nick\r\n";
    } else {
      se = ":" + SERVER_NAME + " 482 " + nickname + " " + channel +
           " :You must be a channel operator\r\n";
    }
    sendString(se, fd);
  } else {
    // :irc.local 403 root #asdfasdf :No such channel
    const std::string se = ":" + SERVER_NAME + " 403 " + nickname + " " +
                           channel + " :No such channel\r\n";

    sendString(se, fd);
  }
}
