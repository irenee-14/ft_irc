#include "Server.hpp"

void Server::msg(int fd, std::vector<std::string> tokens, std::string cmd) {
  // PRIVMSG target :babo
  // target이 채널이 아니면 target에게 메시지 보내기
  if (tokens[1][0] != '#') {
    std::string se2 = ":" + clients[fd].getNick() + "!" +
                      clients[fd].getUserFd() + "@" +
                      clients[fd].getServerName() + " " + cmd + " " +
                      tokens[1] + " :" + tokens[2] + "\r\n";
    // users에서 user 찾기
    for (unsigned int i = 0; i < clients.size(); i++) {
      if (clients[i].getNick() == tokens[1]) {
        sendString(se2, i);
        return;
      }
    }
    // 보내려는 유저가 없으면 에러
    // :irc.local 401 root hi :No such nick
    std::string se = ":127.0.0.1 401 " + clients[fd].getNick() + " " +
                     tokens[1] + " :No such nick\r\n";
    sendString(se, fd);
    return;
  }

  // PRIVMSG #channelname :hello
  // 채널에 속해있는 모든 유저에게 메시지 보내기
  std::string name = tokens[1].substr(1, tokens[1].size() - 1);
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUserFd() +
                   "@" + clients[fd].getServerName() + " " + cmd + " " +
                   tokens[1] + " :" + tokens[2] + "\r\n";
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

void Server::privateMsg(int fd, std::vector<std::string> tokens) {
  msg(fd, tokens, "PRIVMSG");
}

void Server::notice(int fd, std::vector<std::string> tokens) {
  msg(fd, tokens, "NOTICE");
}
