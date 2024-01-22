#include "Server.hpp"

const std::string strList(Client client, Channel channel) {
  return (":" + client.getServerName() + " 322 " + client.getNick() + " #" +
          channel.getChannelName() + " " +
          std::to_string(channel.getUserFds().size()) + " :[+" +
          channel.getModes() + "]\r\n");
}

void Server::list(int fd, std::string channel) {
  printArg("channel size : ", channels.size());

  std::string se = ":" + clients[fd].getServerName() + " 321 " +
                   clients[fd].getNick() + " Channel :Users Name\r\n";

  if (channels.size() > 0) {
    // LIST #channel
    if (channel != "") {
      std::string channelNoHash = channel.substr(1, channel.size() - 1);

      int channel_idx = isChannel(channelNoHash);
      if (channel_idx >= 0) {
        se += strList(clients[fd], channels[channel_idx]);
      }
    }
    // LIST : 모든 채널 정보 출력
    else {
      for (unsigned int i = 0; i < channels.size(); ++i) {
        se += strList(clients[fd], channels[i]);
      }
    }
  }
  se += ":" + clients[fd].getServerName() + " 323 " + clients[fd].getNick() +
        " :End of channel list.\r\n";
  sendString(se, fd);
}

// 헷갈리는 부분 : list 여러개 확인 못 했었나?
