#include "Server.hpp"

std::string Server::userList(Channel& channel) {
  std::string se = "";

  std::vector<int> users = channel.getUsers();
  std::vector<int> ops = channel.getOperators();
  std::cout << "users: " << users.size() << "ops : " << ops.size() << std::endl;

  for (unsigned int i = 0; i < users.size(); ++i) {
    std::vector<int>::iterator it = std::find(ops.begin(), ops.end(), users[i]);
    if (it != ops.end()) se += "@";
    se += clients[users[i]].getNick();
    if (i != users.size() - 1) se += " ";
  }
  return (se);
}

void Server::join(int fd, std::string token) {
  unsigned int i = 0;

  std::string name = token.substr(1, token.size() - 1);

  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      channels[i].addUser(fd);
      break;
    }
  }

  // 채널 없으면 새로 만들기
  if (i == channels.size()) {
    channels.push_back(Channel(name));
    channels[i].addUser(fd);
    channels[i].addOperator(fd);
  }

  // 채널에 속한 모든 user에게 join 메시지 보내기
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " JOIN :#" + name +
                   "\r\n";
  sendString(se, channels[i].getUsers());

  // 채널에 들어온 user에게 채널 정보 보내기
  std::string se2 = ":127.0.0.1 353 " + clients[fd].getNick() + " = #" + name +
                    " :" + userList(channels[i]) + "\r\n";

  se2 += ":127.0.0.1 366 " + clients[fd].getNick() + " #" + name +
         " :End of /NAMES list.\r\n";
  sendString(se2, fd);
}

// ------------------------------------------------------------------------------

void Server::part(int fd, std::string token) {
  // 채널 나가는데 마지막이면 채널 없애기
  std::string name = token.substr(1, token.size() - 1);
  unsigned int i = 0;
  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      break;
    }
  }
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   " PART :#" + name + "\r\n";

  sendString(se, channels[i].getUsers());
  // users에서 지우기
  channels[i].removeUser(fd);
  // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
  if (channels[i].getUsers().empty()) {
    channels.erase(channels.begin() + i);
  }
}

// ----------------------------------------------------------------------------

//
// void Server::msg(int fd, std::vector<std::string> token) {
//   //
//   (void)fd;
//   (void)token;
// }