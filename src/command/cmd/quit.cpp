#include "Server.hpp"

void Server::quit(int fd) {
  // 채널에서 나가기
  for (unsigned int i = 0; i < channels.size(); ++i) {
    std::vector<int> users = channels[i].getUserFds();

    std::vector<int>::iterator it = std::find(users.begin(), users.end(), fd);

    if (it != users.end()) {
      // 속해있는 채널에서 나가기
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // 유저가 한명일 때, 채널 두 개 들어갔다가 나가면 세그폴트
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      channels[i].removeUser(fd, users);

      // operator인 경우 operator 목록에서 제거
      std::vector<int> ops = channels[i].getOperators();
      std::vector<int>::iterator it2 = std::find(ops.begin(), ops.end(), fd);
      if (it2 != ops.end()) {
        channels[i].removeOperator(fd);
      }

      // 채널에 속한 모든 user에게 quit 메시지 보내기
      std::string se = ":" + clients[fd].getNick() + "!" +
                       clients[fd].getUserFd() + "@" +
                       clients[fd].getServerName() + " QUIT :leaving\r\n";
      sendString(se, channels[i].getUserFds());
    }
  }
  // server에 quit 메시지 보내기
  std::string se = "ERROR :Closing link: (" + clients[fd].getUserFd() + "@" +
                   clients[fd].getServerName() + ") [Quit: leaving]\r\n";

  sendString(se, fd);
}
