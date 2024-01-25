#include "Server.hpp"
#include <algorithm> // remove

void Server::msg(int fd, std::vector<std::string> tokens, std::string cmd) {
  std::string target = tokens[1];
  std::string message = tokens[2];

  // PRIVMSG target :babo
  // target이 채널이 아니면 target에게 메시지 보내기
  if (target[0] != '#') {
    std::string se2 = ":" + clients[fd].getNick() + "!" +
                      clients[fd].getUser() + "@" +
                      clients[fd].getServerName() + " " + cmd + " " + target +
                      " :" + message + "\r\n";

    // users에서 user 찾기
    int target_fd = isUser(target);

    // 보내는 타겟이 자신이면 보내지 않기
    if (fd == target_fd)
      ;
    else if (target_fd >= 0) {
      sendString(se2, target_fd);
    }
    // 보내려는 유저가 없으면 에러
    // :irc.local 401 root hi :No such nick
    else {
      std::string se = ":" + SERVER_NAME + " 401 " + clients[fd].getNick() +
                       " " + target + " :No such nick\r\n";
      sendString(se, fd);
    }
    return;
  }

  // PRIVMSG #channelname :hello
  // 채널에 속해있는 모든 유저에게 메시지 보내기
  std::string name = target.substr(1, target.size() - 1);

  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " " + cmd + " " +
                   target + " :" + message + "\r\n";

  int channel_idx = isChannel(name);
  if (channel_idx >= 0) {
    std::vector<int> users = channels[channel_idx].getUserFds();
    users.erase(std::remove(users.begin(), users.end(), fd), users.end());

    sendString(se, users);
  }
  // !!!!!!else
  // 채널이 없는 경우
}

void Server::privateMsg(int fd, std::vector<std::string> tokens) {
  msg(fd, tokens, "PRIVMSG");
}

void Server::notice(int fd, std::vector<std::string> tokens) {
  msg(fd, tokens, "NOTICE");
}
