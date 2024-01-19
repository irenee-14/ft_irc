#include "Server.hpp"

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
  unsigned int i = 0;

  std::string name = token.substr(1, token.size() - 1);

  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      channels[i].addUser(fd, clients[fd].getNick());
      break;
    }
  }

  // 채널 없으면 새로 만들기
  if (i == channels.size()) {
    channels.push_back(Channel(name));
    channels[i].addUser(fd, clients[fd].getNick());
    channels[i].addOperator(fd);
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
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   " PART :#" + name + "\r\n";

  sendString(se, channels[i].getUserFds());
  // users에서 지우기
  channels[i].removeUser(fd, channels[i].getUserFds());
  // users에서 빠지면서 채널에 아무도 없으면 채널 없애기
  if (channels[i].getUserFds().empty()) {
    channels.erase(channels.begin() + i);
  }
}

// ----------------------------------------------------------------------------

void Server::msg(int fd, std::vector<std::string> token, std::string cmd) {
  // PRIVMSG target :babo
  // target이 채널이 아니면 target에게 메시지 보내기
  if (token[1][0] != '#') {
    std::string se2 = ":" + clients[fd].getNick() + "!" +
                      clients[fd].getUserFd() + "@" +
                      clients[fd].getServerName() + " " + cmd + " " + token[1] +
                      " :" + token[2] + "\r\n";
    // users에서 user 찾기
    for (unsigned int i = 0; i < clients.size(); i++) {
      if (clients[i].getNick() == token[1]) {
        sendString(se2, i);
        return;
      }
    }
    // 보내려는 유저가 없으면 에러
    // :irc.local 401 root hi :No such nick
    std::string se = ":127.0.0.1 401 " + clients[fd].getNick() + " " +
                     token[1] + " :No such nick\r\n";
    sendString(se, fd);
    return;
  }

  // PRIVMSG #channelname :hello
  // 채널에 속해있는 모든 유저에게 메시지 보내기
  std::string name = token[1].substr(1, token[1].size() - 1);
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " " + cmd + " " +
                   token[1] + " :" + token[2] + "\r\n";
  // 채널 이름으로 채널 찾기
  unsigned int i = 0;
  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      break;
    }
  }
  std::vector<int> users = channels[i].getUserFds();
  users.erase(std::remove(users.begin(), users.end(), fd), users.end());
  sendString(se, users);
}

void Server::privateMsg(int fd, std::vector<std::string> token) {
  msg(fd, token, "PRIVMSG");
}

void Server::notice(int fd, std::vector<std::string> token) {
  msg(fd, token, "NOTICE");
}

void Server::kick(int fd, std::vector<std::string> token) {
  std::string name = token[1].substr(1, token[1].size() - 1);

  // 채널 이름으로 채널 찾기
  unsigned int i = 0;
  for (; i < channels.size(); i++) {
    if (channels[i].getChannelName() == name) {
      break;
    }
  }

  std::string se = "";

  // operator인지 확인
  if (channels[i].isOperator(fd)) {
    for (unsigned int j = 0; j < channels[i].getUserFds().size(); j++) {
      if (channels[i].getUserNicks()[j] == token[2]) {
        se += ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
              "@" + clients[fd].getServerName() + " KICK " + token[1] + " " +
              token[2] + " :" + token[3] + "\r\n";
        sendString(se, channels[i].getUserFds());
        channels[i].removeUser(token[2], channels[i].getUserNicks());
        return;
      }
    }
    // 보내려는 유저가 없으면 에러
    se += ":" + clients[fd].getServerName() + " 401 " + clients[fd].getNick() +
          " " + token[2] + " :No such nick\r\n";

  } else {
    se += ":" + clients[fd].getServerName() + " 482 " + clients[fd].getNick() +
          " " + token[1] + " :You must be a channel operator\r\n";
  }
  sendString(se, fd);
}