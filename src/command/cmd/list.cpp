#include "Server.hpp"

void Server::list(int fd, std::vector<std::string> tokens) {
  std::string se = ":" + clients[fd].getServerName() + " 321 " +
                   clients[fd].getNick() + " Channel :Users Name\r\n";
  std::cout << "channel size : " << channels.size() << std::endl;

  if (channels.size() > 0) {
    // LIST #channel
    // # 제거하고 채널 이름과 비교해서 채널 정보 출력
    if (tokens.size() > 1) {
      std::string name = tokens[1].substr(1, tokens[1].size() - 1);

      for (unsigned int i = 0; i < channels.size(); ++i) {
        if (channels[i].getChannelName() == name) {
          se += ":" + clients[fd].getServerName() + " 322 " +
                clients[fd].getNick() + " #" + channels[i].getChannelName() +
                " " + std::to_string(channels[i].getUserFds().size()) + " :[+" +
                channels[i].getModes() + "]\r\n";
        }
      }
    }
    // LIST
    // 모든 채널 정보 출력
    else {
      for (unsigned int i = 0; i < channels.size(); ++i) {
        se += ":" + clients[fd].getServerName() + " 322 " +
              clients[fd].getNick() + " #" + channels[i].getChannelName() +
              " " + std::to_string(channels[i].getUserFds().size()) + " :[+" +
              channels[i].getModes() + "]\r\n";
      }
    }
  }
  se += ":" + clients[fd].getServerName() + " 323 " + clients[fd].getNick() +
        " :End of channel list.\r\n";
  sendString(se, fd);
}
