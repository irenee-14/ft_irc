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

void Server::join(int fd, std::string token) {
  // unsigned int i = 0;

  std::string name = token.substr(1, token.size() - 1);

  // for (; i < channels.size(); i++) {
  //   if (channels[i].getChannelName() == name) {
  //     channels[i].addUser(fd, clients[fd].getNick());
  //     break;
  //   }
  // }

  int channel_idx = isChannel(name);
  if (channel_idx >= 0) {
    channels[channel_idx].addUser(fd, clients[fd].getNick());
  }
  // 채널 없으면 새로 만들기
  else {
    channels.push_back(Channel(name));
    channels.back().addUser(fd, clients[fd].getNick());
    channels.back().addOperator(fd);
  }

  // 채널에 속한 모든 user에게 join 메시지 보내기
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " JOIN :#" + name +
                   "\r\n";
  sendString(se, channels[i].getUserFds());

  // 채널에 들어온 user에게 채널 정보 보내기
  std::string se2 = ":127.0.0.1 353 " + clients[fd].getNick() + " = #" + name +
                    " :" + userList(channels[i]) + "\r\n";

  se2 += ":127.0.0.1 366 " + clients[fd].getNick() + " #" + name +
         " :End of /NAMES list.\r\n";
  sendString(se2, fd);
}
