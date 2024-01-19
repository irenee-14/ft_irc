#include <unistd.h>

#include "Server.hpp"

void Server::nick(int fd, std::string token) {
  std::string se = ":" + clients[fd].getNick() + "!" + clients[fd].getUser() +
                   "@" + clients[fd].getServerName() + " NICK :" + token +
                   "\r\n";

  clients[fd].setNick(token);

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

// 채널 목록과 채널에 속한 user_num, 채널 옵션, 채널 주제 출력
// **************************
// 채널 옵션 받아와서 출력 할 것
// 채널 토픽 받아와서 출력 할 것

void Server::list(int fd, std::string rawToken) {
  std::string se = ":" + clients[fd].getServerName() + " 321 " +
                   clients[fd].getNick() + " Channel :Users Name\r\n";
  // LIST #channel
  // # 제거하고 채널 이름과 비교해서 채널 정보 출력
  if (rawToken.size() > 0) {
    std::string token = rawToken.substr(1, rawToken.size() - 1);

    for (unsigned int i = 0; i < channels.size(); ++i) {
      if (channels[i].getChannelName() == token) {
        se += ":" + clients[fd].getServerName() + " 322 " +
              clients[fd].getNick() + " #" + channels[i].getChannelName() +
              " " + std::to_string(channels[i].getUsers().size()) + " :[+" +
              channels[i].getModes() + "]\r\n";
      }
    }
  }
  // LIST
  // 모든 채널 정보 출력
  else {
    for (unsigned int i = 0; i < channels.size(); ++i) {
      se += ":" + clients[fd].getServerName() + " 322 " +
            clients[fd].getNick() + " #" + channels[i].getChannelName() + " " +
            std::to_string(channels[i].getUsers().size()) + " :[+" +
            channels[i].getModes() + "]\r\n";
    }
  }
  se += ":" + clients[fd].getServerName() + " 323 " + clients[fd].getNick() +
        " :End of channel list.\r\n";
  sendString(se, fd);
}
// ------------------------------------------------------------------------------

void Server::whois(int fd, std::string token) {
  std::string se = ":" + clients[fd].getServerName() + " 311 " +
                   clients[fd].getNick() + " " + token + " " +
                   clients[fd].getUser() + " " + clients[fd].getServerName() +
                   " * :" + clients[fd].getRealName() + "\r\n";
  se += ":" + clients[fd].getServerName() + " 312 " + clients[fd].getNick() +
        " " + token + " " + clients[fd].getServerName() +
        " :Local IRC Server\r\n";

  se += ":" + clients[fd].getServerName() + " 317 " + clients[fd].getNick() +
        " " + token + " " +
        std::to_string(time(0) - clients[fd].getTimestamp()) + " " +
        std::to_string(clients[fd].getTimestamp()) +
        " :seconds idle, signon "
        "time\r\n";

  se += ":" + clients[fd].getServerName() + " 318 " + clients[fd].getNick() +
        " " + token + " :End of /WHOIS list.\r\n";

  sendString(se, fd);
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
