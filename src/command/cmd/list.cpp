#include "Server.hpp"

const std::string strList(Client client, Channel channel) {
  std::stringstream channel_size;
   channel_size << channel.getUserFds().size();

  return (":" + SERVER_NAME + " 322 " + client.getNick() + " #" +
          channel.getChannelName() + " " +
          intToString(channel.getUserFds().size()) + " :[+" +
          channel.getModes() + "]\r\n");
}

void Server::list(int fd, std::vector<std::string> tokens) {
  std::string channel = tokens[1];

  unsigned int size = channels.size();

  printArg("channel size : ", size);

  std::string se = ":" + SERVER_NAME + " 321 " + clients[fd].getNick() +
                   " Channel :Users Name\r\n";

  if (size > 0) {
    // LIST #channel
    if (tokens.size() > 1) {
      std::string channelNoHash = channel.substr(1, channel.size() - 1);

      int channel_idx = isChannel(channelNoHash);
      if (channel_idx >= 0) {
        se += strList(clients[fd], channels[channel_idx]);
      }
    }
    // LIST : 모든 채널 정보 출력
    else {
      for (unsigned int i = 0; i < size; ++i) {
        se += strList(clients[fd], channels[i]);
      }
    }
  }
  se += ":" + SERVER_NAME + " 323 " + clients[fd].getNick() +
        " :End of channel list.\r\n";
  sendString(se, fd);
}

// 헷갈리는 부분 : list 여러개 확인 못 했었나?
