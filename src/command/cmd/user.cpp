#include "Server.hpp"

void Server::user(int fd, std::vector<std::string> tokens) {
  clients[fd].setUser(tokens[1]);
  clients[fd].setServerName(tokens[3]);
  clients[fd].setRealName(tokens[4]);

  std::cout << "\n결과 : " << clients[fd].getNick() << " "
            << clients[fd].getUserFd() << " " << clients[fd].getServerName()
            << " " << clients[fd].getRealName() << std::endl
            << std::endl;

  // 다 받은거 확인되면 welcome
  // 아니면 에러 띄우고 종료?
  // nick, user 둘 다 받았는지 확인
  if (clients[fd].getNick() != "" && clients[fd].getUser() != "") {
    clients[fd].setTimestamp(time(0));
    std::string se = ":" + clients[fd].getServerName() + " 001 " +
                     clients[fd].getNick() + " :Welcome\r\n";
    sendString(se, fd);
  }
}
