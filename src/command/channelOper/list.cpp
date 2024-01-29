#include "Server.hpp"

const std::string RPL_LIST(const std::string nickname, Channel channel,
                           std::string modes) {
  return (":" + SERVER_NAME + " 322 " + nickname + " #" +
          channel.getChannelName() + " " +
          intToString(channel.getUserFds().size()) + " :[+" + modes + "]\r\n");
}

// -----------------------------------------------------------

void Server::list(int fd, std::vector<std::string> tokens) {
  const std::string channel = tokens.size() > 1 ? tokens[1] : "";
  const std::string nickname = clients[fd].getNick();

  unsigned int size = channels.size();

  printArg("channel size : ", size);

  std::string se =
      ":" + SERVER_NAME + " 321 " + nickname + " Channel :Users Name\r\n";

  if (size > 0) {
    // LIST #channel
    if (tokens.size() > 1) {
      std::string channelNoHash = channel.substr(1, channel.size() - 1);

      int channel_idx = isChannel(channelNoHash);
      if (channel_idx >= 0) {
        se += RPL_LIST(nickname, channels[channel_idx],
                       channels[channel_idx].getModes(fd));
      }
    }
    // LIST : 모든 채널 정보 출력
    else {
      for (unsigned int i = 0; i < size; ++i) {
        se += RPL_LIST(nickname, channels[i], channels[i].getModes(fd));
      }
    }
  }
  se += ":" + SERVER_NAME + " 323 " + nickname + " :End of channel list.\r\n";
  sendString(se, fd);
}

// 헷갈리는 부분 : list 여러개 확인 못 했었나?
