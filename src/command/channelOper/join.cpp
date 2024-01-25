#include <algorithm>  // find

#include "Server.hpp"

// join 시 채널에 있는 user들의 nick을 string으로 반환
const std::string Server::userList(const Channel& channel) {
  std::string se = "";

  std::vector<int> users = channel.getUserFds();
  std::vector<int> ops = channel.getOperators();

  printArg("\nusers : ", users.size());
  printArg("operators : ", ops.size());

  for (unsigned int i = 0; i < users.size(); ++i) {
    std::vector<int>::iterator it = std::find(ops.begin(), ops.end(), users[i]);

    if (it != ops.end()) se += "@";
    se += clients[users[i]].getNick();

    if (i != users.size() - 1) se += " ";
  }
  return (se);
}

// -------------------------------------------------------------

void Server::join(int fd, std::string channel) {
  const std::string channelNoHash = channel.substr(1, channel.size() - 1);

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  int channel_idx = isChannel(channelNoHash);
  if (channel_idx >= 0) {
    channels[channel_idx].addUser(fd, nickname);
  } else {
    channel_idx = channels.size();
    // 채널 없으면 새로 만들기
    channels.push_back(Channel(channelNoHash));
    channels.back().addUser(fd, nickname);
    channels.back().addOperator(fd);
  }

  {
    // 채널에 속한 모든 user에게 join 메시지 보내기
    std::string se = ":" + nickname + "!" + username + "@" + servername +
                     " JOIN :" + channel + "\r\n";
    sendString(se, channels[channel_idx].getUserFds());
  }

  {
    // 채널에 들어온 user에게 채널 정보 보내기
    std::string se2 = ":" + SERVER_NAME + " 353 " + nickname + " = " + channel +
                      " :" + userList(channels[channel_idx]) + "\r\n";

    se2 += ":" + SERVER_NAME + " 366 " + nickname + " " + channel +
           " :End of /NAMES list.\r\n";
    sendString(se2, fd);
  }
}
