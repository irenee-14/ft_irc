#include <sstream>

#include "Server.hpp"

std::vector<std::string> splitCommand(const std::string str) {
  std::vector<std::string> result;
  std::string temp;
  bool hasColon = false;

  std::stringstream ss;
  ss.str("");
  ss << str;

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
  ss.str("");
  return (result);
}

void Server::executeCommand(int fd, std::vector<std::string> tokens) {
  // token[0]을 command_list와 비교해서 switch문으로 처리
  switch (command_list[tokens[0]]) {
    case NICK:
      nick(fd, tokens[1]);
      break;
    case USER:
      user(fd, tokens);
      break;
    case USERHOST:
      userhost(fd, tokens);
      break;
    case PING:
      pong(fd);
      break;
    case LIST:
      list(fd, tokens);
      break;
    case WHOIS:
      whois(fd, tokens[1]);
      break;
    case JOIN:
      join(fd, tokens);
      break;
    case PART:
      part(fd, tokens[1]);
      break;
    case PRIVMSG:
      privateMsg(fd, tokens);
      break;
    case NOTICE:
      notice(fd, tokens);
      break;
    case KICK:
      kick(fd, tokens);
      break;
    case INVITE:
      invite(fd, tokens);
      break;
    case TOPIC:
      topic(fd, tokens);
      break;
    case MODE:
      mode(fd, tokens);
      break;
    case QUIT:
      quit(fd, tokens[1]);
      break;
    default:
      break;
  }
}

void Server::checkCommand(struct pollfd fds, char* buf) {
  std::stringstream ss;
  ss.str("");
  ss << buf;

  std::string line;

  while (std::getline(ss, line)) {
    if (line.c_str()[line.length() - 1] != '\r') break;

    std::string str = line.substr(0, line.find("\r"));

    if (str.find("CAP LS") == 0) {
      const char* se = "CAP * LS\r\n";
      send(fds.fd, se, strlen(se), 0);
    } else if (str.find("CAP END") == 0 || str.find("JOIN :") == 0)
      ;
    else {
      std::vector<std::string> tokens = splitCommand(str);

      if (tokens[0] == "PASS")
        pass(fds.fd, tokens[1]);
      else if (tokens[0] != "" && !clients[fds.fd].getPassFlag())
        throw std::string("password does not exist");
      else
        executeCommand(fds.fd, tokens);

      if (!clients[fds.fd].getNickFlag() && clients[fds.fd].getNick() != "" &&
          clients[fds.fd].getUser() != "") {
        clients[fds.fd].setTimestamp(time(0));
        std::string se = ":" + SERVER_NAME + " 001 " +
                         clients[fds.fd].getNick() + " :Welcome\r\n";
        sendString(se, fds.fd);
        clients[fds.fd].setNickFlag(true);
      }
    }
    line.clear();
  }
  ss.str("");
}
