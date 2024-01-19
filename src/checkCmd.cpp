#include <sstream>

#include "Server.hpp"

std::vector<std::string> splitCommand(const std::string& str) {
  std::vector<std::string> result;
  std::string temp;
  bool hasColon = false;

  std::stringstream ss(str);
  char c;

  while (ss.get(c)) {
    if (c == ':') {
      hasColon = true;
      if (!temp.empty()) {
        result.push_back(temp);
        temp.clear();
      }
    } else if (c == ' ' && !hasColon) {
      result.push_back(temp);
      temp.clear();
    } else
      temp += c;
  }
  if (!temp.empty()) result.push_back(temp);

  return (result);
}

void Server::checkCommand(struct pollfd fds, char* buf) {
  std::stringstream ss(buf);
  std::string line;

  while (std::getline(ss, line)) {
    if (line.c_str()[line.length() - 1] == '\r') {
      std::string str = line.substr(0, line.find("\r"));
      if (str.find("CAP LS") == 0) {
        const char* se = "CAP * LS\r\n";
        send(fds.fd, se, strlen(se), 0);
      } else if (str.find("CAP END") == 0 || str.find("JOIN :") == 0) {
        std::cout << "cap end or join" << std::endl;

      } else {
        std::vector<std::string> tokens = splitCommand(str);
        if (tokens[0] == "PASS")
          pass(fds.fd, tokens[1]);
        else if (!clients[fds.fd].getPassFlag()) {
          throw std::string("not pass");
        }

        // token[0]을 command_list와 비교해서 switch문으로 처리
        switch (Server::command_list[tokens[0]]) {
          case NICK:
            nick(fds.fd, tokens[1]);
            break;
          case USER:
            user(fds.fd, tokens);
            break;
          case USERHOST:
            userhost(fds.fd, tokens);
            break;
          case PING:
            pong(fds.fd);
            break;
          case LIST:
            list(fds.fd, tokens[1]);
            break;
          case WHOIS:
            whois(fds.fd, tokens[1]);
            break;
          case JOIN:
            join(fds.fd, tokens[1]);
            break;
          case PART:
            part(fds.fd, tokens[1]);
            break;
          case PRIVMSG:
            privateMsg(fds.fd, tokens);
            break;
          case NOTICE:
            notice(fds.fd, tokens);
            break;
          case KICK:
            kick(fds.fd, tokens);
            break;
          case INVITE:
            break;
          case TOPIC:
            topic(fds.fd, tokens);
            break;
          case MODE:
            break;
          case QUIT:
            quit(fds.fd);
            break;
          default:
            break;
        }
      }
    }
  }
}
