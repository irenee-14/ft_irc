#include "Server.hpp"

// join 시 채널에 있는 user들의 nick을 string으로 반환
std::string Server::userList(Channel& channel) {
  std::string se = "";

  std::vector<int> users = channel.getUserFds();
  std::vector<int> ops = channel.getOperators();

  std::cout << "\nusers : " << users.size() << " ops : " << ops.size() << "\n"
            << std::endl;

  for (unsigned int i = 0; i < users.size(); ++i) {
    std::vector<int>::iterator it = std::find(ops.begin(), ops.end(), users[i]);

    if (it != ops.end()) se += "@";
    se += clients[users[i]].getNick();

    if (i != users.size() - 1) se += " ";
  }
  return (se);
}

void Server::join(int fd, std::string channel) {
  std::string channelNoHash = channel.substr(1, channel.size() - 1);

  int channel_idx = isChannel(channelNoHash);
  if (channel_idx >= 0) {
    channels[channel_idx].addUser(fd, clients[fd].getNick());
  } else {
    channel_idx = channels.size();
    // 채널 없으면 새로 만들기
    channels.push_back(Channel(channelNoHash));
    channels.back().addUser(fd, clients[fd].getNick());
    channels.back().addOperator(fd);
  }

  // 채널에 속한 모든 user에게 join 메시지 보내기
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " JOIN :" + channel +
                   "\r\n";
  // ??? 채널 들어온 사람한테는 ** has joined #channel 메시지 안 띄우지 않았나??
  sendString(se, channels[channel_idx].getUserFds());

  // 채널에 들어온 user에게 채널 정보 보내기
  std::string se2 = ":127.0.0.1 353 " + clients[fd].getNick() + " = " +
                    channel + " :" + userList(channels[channel_idx]) + "\r\n";

  se2 += ":127.0.0.1 366 " + clients[fd].getNick() + " " + channel +
         " :End of /NAMES list.\r\n";
  sendString(se2, fd);
}
