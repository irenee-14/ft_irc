#include <Server.hpp>

// INVITE user #channel

// invite를 한 channel이 존재해야 함
// :irc.local 403 root #asdf :No such channel

// invite를 받은 user가 존재해야 함
// irc.local 401 root root_____ :No such nick

// invite를 한 user가 채널에 속해있어야 함
// :irc.local 442 root__ #hi :You're not on that channel!

// invite를 한 user가 operator여야 함
// :irc.local 482 root__ #hi :You must be a channel operator

// invite를 받은 user가 채널에 속해있으면 안됨
// :irc.local 443 root root_ #hi :is already on channel

void Server::invite(int fd, std::vector<std::string> tokens) {
  unsigned int i = 0;

  std::string name = tokens[2].substr(1, tokens[2].size() - 1);
  std::string user = tokens[1];

  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      break;
    }
  }

  // invite한 channel이 존재하는지 확인

  if (i == channels.size()) {
    std::string se = ":" + clients[fd].getServerName() + " 403 " +
                     clients[fd].getNick() + " " + tokens[2] +
                     " :No such channel\r\n";
    sendString(se, fd);
    return;
  }

  // invite 받은 user가 서버에 존재하는지 확인
  std::map<int, Client>::iterator it = clients.begin();

  for (; it != clients.end(); ++it) {
    if (it->second.getNick() == user) {
      break;
    }
  }

  if (it == clients.end()) {
    std::string se = ":" + clients[fd].getServerName() + " 401 " +
                     clients[fd].getNick() + " " + user + " :No such nick\r\n";
    sendString(se, fd);
    return;
  }

  // invite한 user가 채널에 속해있는지 확인
  std::vector<int> users = channels[i].getUserFds();

  std::vector<int>::iterator it2 = std::find(users.begin(), users.end(), fd);

  if (it2 == users.end()) {
    std::string se = ":" + clients[fd].getServerName() + " 442 " +
                     clients[fd].getNick() + " " + tokens[2] +
                     " :You're not on that channel!\r\n";
    sendString(se, fd);
    return;
  }

  // invite한 user가 operator인지 확인
  std::vector<int> ops = channels[i].getOperators();

  std::vector<int>::iterator it3 = std::find(ops.begin(), ops.end(), fd);

  if (it3 == ops.end()) {
    std::string se = ":" + clients[fd].getServerName() + " 482 " +
                     clients[fd].getNick() + " " + tokens[2] +
                     " :You must be a channel operator\r\n";
    sendString(se, fd);
    return;
  }

  // invite 받은 user가 채널에 속해있는지 확인
  std::vector<int>::iterator it4 =
      std::find(users.begin(), users.end(), it->first);

  if (it4 != users.end()) {
    std::string se = ":" + clients[fd].getServerName() + " 443 " +
                     clients[fd].getNick() + " " + user + " " + tokens[2] +
                     " :is already on channel\r\n";
    sendString(se, fd);
    return;
  }

  // 모든 조건 만족하면 invite 보내기

  // target에게 보내는 메시지
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " INVITE " + user +
                   " #" + name + "\r\n";

  sendString(se, it->first);

  // 보내는 user에게 보내는 메시지
  std::string se2 = ":" + clients[fd].getServerName() + " 341 " +
                    clients[fd].getNick() + " " + user + " " + tokens[2] +
                    "\r\n";

  sendString(se2, fd);

  // 채널에 속한 모든 user에게 보내는 메시지
  //   :irc.local NOTICE #hi :*** root invited root_ into the channel
  std::string se3 =
      ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() + "@" +
      clients[fd].getServerName() + " NOTICE " + tokens[2] + " :*** " +
      clients[fd].getNick() + " invited " + user + " into the channel\r\n";

  users.erase(std::remove(users.begin(), users.end(), fd), users.end());

  sendString(se3, users);
}