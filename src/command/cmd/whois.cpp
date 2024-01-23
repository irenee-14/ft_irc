#include "Server.hpp"

void Server::whois(int fd, std::string nickname) {
  std::string se = ":" + clients[fd].getServerName() + " 311 " +
                   clients[fd].getNick() + " " + nickname + " " +
                   clients[fd].getUserFd() + " " + clients[fd].getServerName() +
                   " * :" + clients[fd].getRealName() + "\r\n";

  se += ":" + clients[fd].getServerName() + " 312 " + clients[fd].getNick() +
        " " + nickname + " " + clients[fd].getServerName() +
        " :Local IRC Server\r\n";

  se += ":" + clients[fd].getServerName() + " 317 " + clients[fd].getNick() +
        " " + nickname + " " +
        std::to_string(time(0) - clients[fd].getTimestamp()) + " " +
        std::to_string(clients[fd].getTimestamp()) +
        " :seconds idle, signon "
        "time\r\n";

  se += ":" + clients[fd].getServerName() + " 318 " + clients[fd].getNick() +
        " " + nickname + " :End of /WHOIS list.\r\n";

  sendString(se, fd);
}
