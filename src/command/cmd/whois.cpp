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
    std::string se = ":" + SERVER_NAME + " 401 " + clients[fd].getNick() + " " +
                     nickname + " :No such nick\r\n";
    se += ":" + SERVER_NAME + " 318 " + clients[fd].getNick() + " " + nickname +
          " :End of /WHOIS list.\r\n";

    sendString(se, fd);
    return;
  }

  std::string se = ":" + SERVER_NAME + " 311 " + clients[fd].getNick() + " " +
                   nickname + " " + clients[fd].getUser() + " " + SERVER_NAME +
                   " * :" + clients[fd].getRealName() + "\r\n";

  se += ":" + SERVER_NAME + " 312 " + clients[fd].getNick() + " " + nickname +
        " " + SERVER_NAME + " :Local IRC Server\r\n";

  // :irc.local 379 root root :is using modes +i - 자기 자신에 대해서만 출력
  if (nickname == clients[fd].getNick()) {
    se += ":" + SERVER_NAME + " 379 " + clients[fd].getNick() + " " + nickname +
          " :is using modes +i" + "\r\n";
  }
  // ?????????????????//

  // :irc.local 317 root root 940 1706001403 :seconds idle, signon time
  se += ":" + SERVER_NAME + " 317 " + clients[fd].getNick() + " " + nickname +
        " " +
        std::to_string(time(0) - clients[isUser(nickname)].getTimestamp()) +
        " " + std::to_string(clients[isUser(nickname)].getTimestamp()) +
        " :seconds idle, signon " + "time\r\n";

  // :irc.local 318 root root :End of /WHOIS list.
  se += ":" + SERVER_NAME + " 318 " + clients[fd].getNick() + " " + nickname +
        " :End of /WHOIS list.\r\n";

  sendString(se, fd);
}
