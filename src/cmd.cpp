#include <unistd.h>

#include "Server.hpp"

void Server::nick(int fd, std::string token) {
  clients[fd].setNick(token);

  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " NICK :" + token +
                   "\r\n";

  sendString(se, fd);
}

// ------------------------------------------------------------------------------

void Server::user(int fd, std::vector<std::string> tokens) {
  clients[fd].setUser(tokens[1]);
  clients[fd].setServerName(tokens[3]);
  clients[fd].setRealName(tokens[4]);

  std::cout << "결과 : " << clients[fd].getNick() << " "
            << clients[fd].getUser() << " " << clients[fd].getServerName()
            << " " << clients[fd].getRealName() << std::endl;

  // 다 받은거 확인되면 welcome
  // 아니면 에러 띄우고 종료?
  // const char* se = ":127.0.0.1 001 jihylim :Welcome\r\n";
  std::string se = ":" + clients[fd].getServerName() + " 001 " +
                   clients[fd].getNick() + " :Welcome\r\n";
  sendString(se, fd);
}

// ------------------------------------------------------------------------------

void Server::userhost(int fd, std::vector<std::string> tokens) {
  std::string se = ":" + clients[fd].getServerName() + " 302 " +
                   clients[fd].getNick() + " :";
  for (unsigned int i = 1; i < tokens.size(); ++i) {
    se += clients[fd].getNick() + "=+" + clients[fd].getUser() + "@" +
          clients[fd].getServerName() + " ";
  }
  se += "\r\n";

  sendString(se, fd);
}

void Server::pong(int fd) {
  const char* se = ":127.0.0.1 PONG 127.0.0.1 :127.0.0.1\r\n";
  sendString(se, fd);
}

// ------------------------------------------------------------------------------

void Server::list(void) {
  for (unsigned int j = 0; j < channels.size(); ++j) {
    std::cout << channels[j].getChannelName() << " ";
    for (unsigned int i = 0; i < channels[j].getUsers().size(); ++i) {
      std::cout << channels[j].getUsers()[i];
    }
    std::cout << std::endl;
  }
}

// ------------------------------------------------------------------------------

void Server::quit(int fd) {
  // 채널에서 나가기
  for (unsigned int i = 0; i < channels.size(); ++i) {
    std::vector<int> users = channels[i].getUsers();
    std::vector<int>::iterator it = std::find(users.begin(), users.end(), fd);

    if (it != users.end()) {
      // 속해있는 채널에서 나가기
      channels[i].removeUser(fd);

      // operator인 경우 operator 목록에서 제거
      std::vector<int> ops = channels[i].getOperators();
      std::vector<int>::iterator it2 = std::find(ops.begin(), ops.end(), fd);
      if (it2 != ops.end()) {
        channels[i].removeOperator(fd);
      }

      // 채널에 속한 모든 user에게 quit 메시지 보내기
      std::string se = ":" + clients[fd].getNick() + "!" +
                       clients[fd].getUser() + "@" +
                       clients[fd].getServerName() + " QUIT :leaving\r\n";
      sendString(se, channels[i].getUsers());
    }
  }
  // server에 quit 메시지 보내기
  std::string se = "ERROR :Closing link: (" + clients[fd].getUser() + "@" +
                   clients[fd].getServerName() + ") [Quit: leaving]\r\n";

  sendString(se, fd);
}

// ------------------------------------------------------------------------------
