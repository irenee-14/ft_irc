#include "Server.hpp"

// root_ 에서  /whois root
// :irc.local 319 root_ root :@#hello @#hi
// :irc.local 312 root_ root irc.local :Local IRC Server
// :irc.local 317 root_ root 1144 1706001403 :seconds idle, signon time
// :irc.local 318 root_ root :End of /WHOIS list.

void Server::whois(int fd, std::string targets) {
  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string realname = clients[fd].getRealName();

  // WHOIS asdfasdf
  // :irc.local 401 root asdfasdf :No such nick
  // :irc.local 318 root asdfasdf :End of /WHOIS list.
  int comma_index = -1;
  targets = targets + ",";
  while (true) {
    int start_index = comma_index + 1;
    comma_index = targets.find(',', comma_index + 1);
    if (comma_index < 0) break;

    std::string target = targets.substr(start_index, comma_index - start_index);
    std::cout << "target: " << target << std::endl;

    if (isUser(target) < 0) {
      std::string se = ":" + SERVER_NAME + " 401 " + nickname + " " + target +
                       " :No such nick\r\n";
      se += ":" + SERVER_NAME + " 318 " + nickname + " " + target +
            " :End of /WHOIS list.\r\n";

      sendString(se, fd);
      return;
    }

    std::string se = ":" + SERVER_NAME + " 311 " + nickname + " " + target +
                     " " + username + " " + SERVER_NAME + " * :" + realname +
                     "\r\n";

    se += ":" + SERVER_NAME + " 312 " + nickname + " " + target + " " +
          SERVER_NAME + " :Local IRC Server\r\n";

    // :irc.local 379 root root :is using modes +i - 자기 자신에 대해서만 출력
    if (target == nickname) {
      se += ":" + SERVER_NAME + " 379 " + nickname + " " + target +
            " :is using modes +i" + "\r\n";
    }

    // :irc.local 317 root root 940 1706001403 :seconds idle, signon time
    se += ":" + SERVER_NAME + " 317 " + nickname + " " + target + " " +
          intToString(time(0) - clients[isUser(target)].getTimestamp()) + " " +
          intToString(clients[isUser(target)].getTimestamp()) +
          " :seconds idle, signon " + "time\r\n";

    // :irc.local 318 root root :End of /WHOIS list.
    se += ":" + SERVER_NAME + " 318 " + nickname + " " + target +
          " :End of /WHOIS list.\r\n";
    sendString(se, fd);
  }
}
// ????????/자기자신한테만 출력하는거 두개 아니었나?
