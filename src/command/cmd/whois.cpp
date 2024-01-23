#include "Server.hpp"

// root_ 에서  /whois root
// :irc.local 319 root_ root :@#hello @#hi
// :irc.local 312 root_ root irc.local :Local IRC Server
// :irc.local 317 root_ root 1144 1706001403 :seconds idle, signon time
// :irc.local 318 root_ root :End of /WHOIS list.

void Server::whois(int fd, std::string nickname) {
  // WHOIS asdfasdf
  // :irc.local 401 root asdfasdf :No such nick
  // :irc.local 318 root asdfasdf :End of /WHOIS list.
  if (isUser(nickname) < 0) {
    std::string se = ":" + clients[fd].getServerName() + " 401 " +
                     clients[fd].getNick() + " " + nickname +
                     " :No such nick\r\n";
    se += ":" + clients[fd].getServerName() + " 318 " + clients[fd].getNick() +
          " " + nickname + " :End of /WHOIS list.\r\n";

    sendString(se, fd);
    return;
  }

  std::string se = ":" + clients[fd].getServerName() + " 311 " +
                   clients[fd].getNick() + " " + nickname + " " +
                   clients[fd].getUserFd() + " " + clients[fd].getServerName() +
                   " * :" + clients[fd].getRealName() + "\r\n";

  // :irc.local 378 root root :is connecting from root@127.0.0.1 127.0.0.1 -
  // 자기 자신에 대해서만 출력
  if (nickname == clients[fd].getNick()) {
    se += ":" + clients[fd].getServerName() + " 378 " + clients[fd].getNick() +
          " " + nickname + " :is connecting from " + clients[fd].getUserFd() +
          " " + clients[fd].getServerName() + "\r\n";
  }

  // :irc.local 319 root root :@#hello @#hi
  // 속한 채널이 있으면 출력, operator면 @ 붙여서 출력
  bool hasChannel = false;
  for (unsigned int i = 0; i < channels.size(); ++i) {
    int user_idx = channels[i].isUser(fd);

    if (user_idx >= 0) {
      if (!hasChannel) {
        se += ":" + clients[fd].getServerName() + " 319 " +
              clients[fd].getNick() + " " + nickname + " :";
        hasChannel = true;
      }
      int op_idx = channels[i].isOperator(fd);

      if (op_idx > 0) se += "@";

      se += "#" + channels[i].getChannelName() + " ";
    }
  }
  if (hasChannel) se += "\r\n";

  // :irc.local 312 root root irc.local :Local IRC Server
  se += ":" + clients[fd].getServerName() + " 312 " + clients[fd].getNick() +
        " " + nickname + " " + clients[fd].getServerName() +
        " :Local IRC Server\r\n";

  // :irc.local 379 root root :is using modes +i - 자기 자신에 대해서만 출력
  if (nickname == clients[fd].getNick()) {
    se += ":" + clients[fd].getServerName() + " 379 " + clients[fd].getNick() +
          " " + nickname + " :is using modes +i" + "\r\n";
  }
  // ?????????????????//

  // :irc.local 317 root root 940 1706001403 :seconds idle, signon time
  se += ":" + clients[fd].getServerName() + " 317 " + clients[fd].getNick() +
        " " + nickname + " " +
        std::to_string(time(0) - clients[isUser(nickname)].getTimestamp()) +
        " " + std::to_string(clients[isUser(nickname)].getTimestamp()) +
        " :seconds idle, signon " + "time\r\n";

  // :irc.local 318 root root :End of /WHOIS list.
  se += ":" + clients[fd].getServerName() + " 318 " + clients[fd].getNick() +
        " " + nickname + " :End of /WHOIS list.\r\n";

  sendString(se, fd);
}
