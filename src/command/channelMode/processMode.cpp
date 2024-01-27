#include "Server.hpp"

bool Server::processModeO(int fd, std::vector<std::string>::iterator& it,
                          Channel& channel, const std::string nickname,
                          bool isAddMode) {
  // server에 user가 존재하는지 확인
  int user_fd = isUser(*it);
  if (user_fd < 0) {
    // 401 ERR_NOSUCHNICK
    // :irc.local 401 root nick :No such nick
    std::string se = ":" + SERVER_NAME + " 401 " + nickname + " " + *it +
                     " :No such nick\r\n";
    sendString(se, fd);
    it++;
    return (false);
  }
  // channel에 user가 존재하지 않으면 continue
  // isAddMode : 이미 operator인 경우 continue
  // !isAddMode : operator가 아닌 경우 continue
  if (channel.isUser(user_fd) < 0 ||
      (channel.isOperator(user_fd) >= 0 && isAddMode) ||
      (channel.isOperator(user_fd) < 0 && !isAddMode)) {
    it++;
    return (false);
  }
  // channel에 user가 존재하고 operator가 아닌 경우 operator 추가
  if (isAddMode)
    channel.addOperator(user_fd);
  else
    channel.removeOperator(user_fd);
  return (true);
}

// ---------------------------------------------------------------

bool Server::processModeK(std::vector<std::string>::iterator& it,
                          Channel& channel, bool isAddMode) {
  // channel에 key가 이미 존재하는지 확인
  if (channel.getKey() != "" && isAddMode) {
    it++;
    return (false);
  }
  // channel에 key 추가
  if (isAddMode)
    channel.setKey(*it);
  else if (channel.getKey() == *it)
    channel.setKey("");
  else {
    it++;
    return (false);
  }
  return (true);
}

// ---------------------------------------------------------------

std::string Server::processModeL(std::vector<std::string>::iterator& it,
                                 Channel& channel, bool isAddMode) {
  // channel에 limit 추가
  if (isAddMode) {
    int retLimit = std::atoi((*it).c_str());
    channel.setLimit(retLimit);
    return (intToString(retLimit));
    it++;
  } else if (channel.getLimit() != -1) {
    channel.setLimit(-1);
    return ("");
  }
  throw 1;
}
