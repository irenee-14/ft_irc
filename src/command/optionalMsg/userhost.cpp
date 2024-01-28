#include "Server.hpp"

void Server::userhost(int fd, std::vector<std::string> tokens) {
  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // tokens[1] 에 값이 없을 때
  // :irc.local 461 root USERHOST :Not enough parameters.
  if (tokens.size() < 2) {
    std::string se = ":" + SERVER_NAME + " 461 " + nickname +
                     " USERHOST :Not enough parameters.\r\n";
    sendString(se, fd);
    return;
  }

  std::string se = ":" + SERVER_NAME + " 302 " + nickname + " :";

  // tokens 다 돌면서 nickname에 맞는 정보 출력
  // userhost root_ root
  // :irc.local 302 root :root_=+root@127.0.0.1 root=+root@127.0.0.1
  for (unsigned int i = 1; i < tokens.size(); ++i) {
    // nickname에 맞는 client 찾기
    int user_fd = isUser(tokens[i]);
    if (user_fd >= 0) {
      const std::string user_nickname = clients[user_fd].getNick();
      const std::string user_username = clients[user_fd].getUser();
      const std::string user_servername = clients[user_fd].getServerName();
      se += user_nickname + "=+" + user_username + "@" + user_servername + " ";
    }
  }
  // 마지막에 \r\n 붙여서 보내기, 찾는 nickname이 없으면 개행만 보냄
  se += "\r\n";
  sendString(se, fd);
}
