#include <algorithm>  // remove

#include "Server.hpp"

void Server::msg(int fd, std::vector<std::string> tokens, std::string cmd) {
  const std::string target = tokens[1];
  const std::string message = tokens[2];

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // PRIVMSG target :babo
  // target이 채널이 아니면 target에게 메시지 보내기
  if (target[0] != '#') {
    // users에서 user 찾기
    int target_fd = isUser(target);

    if (target_fd >= 0) {
      std::string se = ":" + nickname + "!" + username + "@" + servername +
                       " " + cmd + " " + target + " :" + message + "\r\n";
      sendString(se, target_fd);
    }
    // 보내려는 유저가 없으면 에러
    // :irc.local 401 root hi :No such nick
    else {
      std::string se = ":" + SERVER_NAME + " 401 " + nickname + " " + target +
                       " :No such nick\r\n";
      sendString(se, fd);
    }
    return;
  }

  // PRIVMSG #channelname :hello
  // 채널에 속해있는 모든 유저에게 메시지 보내기
  std::string name = target.substr(1, target.size() - 1);

  int channel_idx = isChannel(name);
  if (channel_idx >= 0) {
    std::vector<int> users = channels[channel_idx].getUserFds();
    users.erase(std::remove(users.begin(), users.end(), fd), users.end());

    std::string se = ":" + nickname + "!" + username + "@" + servername + " " +
                     cmd + " " + target + " :" + message + "\r\n";
    sendString(se, users);
  } else {
    // :irc.local 403 root #asdfasdf :No such channel
    const std::string se = ":" + SERVER_NAME + " 403 " + nickname + " " +
                           target + " :No such channel\r\n";
    sendString(se, fd);
  }
}

void Server::privateMsg(int fd, std::vector<std::string> tokens) {
  msg(fd, tokens, "PRIVMSG");
}

void Server::notice(int fd, std::vector<std::string> tokens) {
  msg(fd, tokens, "NOTICE");
}
