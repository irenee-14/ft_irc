#include <sstream>  // l : stringstream

#include "Server.hpp"
std::string makeModeReply(const Client client, std::string channel,
                          std::vector<t_mode> modes) {
  std::string se = ":" + client.getNick() + "!" + client.getUser() + "@" +
                   client.getServerName() + " MODE " + channel + " ";

  if (modes[0]._isAddMode)
    se += "+";
  else
    se += "-";

  for (unsigned int i = 0; i < modes.size(); ++i) {
    if (i > 0 && modes[i]._isAddMode != modes[i - 1]._isAddMode) {
      if (modes[i]._isAddMode)
        se += "+";
      else
        se += "-";
    }
    se += modes[i]._option;
  }

  for (unsigned int i = 0; i < modes.size(); ++i) {
    if (modes[i]._arg != "") se += " " + modes[i]._arg;
    // !!!!! ':' 마지막에 붙여야하는지 고민해보기
  }
  se += "\r\n";
  return (se);
}

// !!!!!!!!!!!!!setMode 해두기

void Server::mode(int fd, std::vector<std::string> tokens) {
  std::string channel = tokens[1];
  std::string channelNoHash = channel.substr(1, channel.size() - 1);
  int channel_idx = isChannel(channelNoHash);
  bool isAddMode = true;

  if (channel_idx < 0) return;
  // tokens[2] 에 값이 없을 때 채널의 현재 모드 정보 출력
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (tokens.size() < 3) return;

  std::string modeStr = tokens[2];
  // tokens에서 3번째부터 끝까지 모드 인자로 받기
  std::vector<std::string> modeArgs =
      std::vector<std::string>(tokens.begin() + 3, tokens.end());

  std::vector<std::string>::iterator it = modeArgs.begin();

  std::vector<t_mode> modes;  // 모드 정보를 담는 vector, 모드 정보는
                              // isAddMode, option, arg로 구성됨

  std::string ret_mode;
  std::string ret_str;

  for (unsigned int i = 0; i < modeStr.size(); ++i) {
    if (modeStr[i] == '+')
      isAddMode = true;
    else if (modeStr[i] == '-')
      isAddMode = false;
    // -------------------------------- "o" --------------------------------
    else if (modeStr[i] == 'o') {
      // *it에 값 없을 경우
      if (it == modeArgs.end()) {
        std::string se = ":" + SERVER_NAME + " 696 " +
                         clients[fd].getNick() + " " + channel +
                         " o * :You must specify a parameter for the op mode. "
                         "Syntax: <nick>.\r\n";
        sendString(se, fd);
        continue;
      }
      // server에 user가 존재하는지 확인
      int user_fd = isUser(*it);
      if (user_fd < 0) {
        // 401 ERR_NOSUCHNICK
        // :irc.local 401 root nick :No such nick
        std::string se = ":" + SERVER_NAME + " 401 " +
                         clients[fd].getNick() + " " + *it +
                         " :No such nick\r\n";
        it++;
        continue;
      }
      // channel에 user가 존재하지 않으면 continue
      // isAddMode : 이미 operator인 경우 continue
      // !isAddMode : operator가 아닌 경우 continue
      if (channels[channel_idx].isUser(user_fd) < 0 ||
          (channels[channel_idx].isOperator(user_fd) >= 0 && isAddMode) ||
          (channels[channel_idx].isOperator(user_fd) < 0 && !isAddMode)) {
        it++;
        continue;
      }
      // channel에 user가 존재하고 operator가 아닌 경우 operator 추가
      if (isAddMode)
        channels[channel_idx].addOperator(user_fd);
      else
        channels[channel_idx].removeOperator(user_fd);
      modes.push_back(t_mode(isAddMode, 'o', *it));
      it++;
    }
    // -------------------------------- "k" --------------------------------
    // MODE +k [key]
    else if (modeStr[i] == 'k') {
      // *it에 값 없을 경우
      // :irc.local 696 root #hi k * :You must specify a parameter for the
      // key mode. Syntax: <key>.

      if (it == modeArgs.end()) {
        std::string se = ":" + SERVER_NAME + " 696 " +
                         clients[fd].getNick() + " " + channel +
                         " k * :You must specify a parameter for the key "
                         "mode. Syntax: <key>.\r\n";
        sendString(se, fd);
        continue;
      }
      // channel에 key가 이미 존재하는지 확인
      if (channels[channel_idx].getKey() != "" && isAddMode) {
        it++;
        continue;
      }
      // channel에 key 추가
      if (isAddMode)
        channels[channel_idx].setKey(*it);
      else if (channels[channel_idx].getKey() == *it)
        channels[channel_idx].setKey("");
      else {
        it++;
        continue;
      }

      modes.push_back(t_mode(isAddMode, 'k', *it));
      it++;
    }
    // -------------------------------- "l" --------------------------------
    else if (modeStr[i] == 'l') {
      //*it에 값 없을 경우
      // :irc.local 696 root #hi l * :You must specify a parameter for the limit
      // mode. Syntax: <limit>.å

      if (isAddMode && it == modeArgs.end()) {
        std::string se = ":" + SERVER_NAME + " 696 " +
                         clients[fd].getNick() + " " + channel +
                         " l * :You must specify a parameter for the limit "
                         "mode. Syntax: <limit>.\r\n";
        sendString(se, fd);
        continue;
      }

      // channel에 limit 추가
      if (isAddMode) {
        int retLimit = std::atoi((*it).c_str());
        channels[channel_idx].setLimit(retLimit);
        std::stringstream ss;
        ss << retLimit;

        modes.push_back(t_mode(isAddMode, 'l', ss.str()));
        it++;
      } else if (channels[channel_idx].getLimit() != -1) {
        channels[channel_idx].setLimit(-1);
        modes.push_back(t_mode(isAddMode, 'l', ""));
      }
    }

    // -------------------------------- "i" --------------------------------
    else if (modeStr[i] == 'i')
      ;
    // -------------------------------- "t" --------------------------------
    else if (modeStr[i] == 't')
      ;
    else
      ;
    // parsing
    // o, i, t, k, l 일 경우 +, - 따라서 함수 실행
    // std::map<char, std::string> option_map;
  }

  // string 만들어서 출력
  if (modes.size() == 0) return;
  std::string se = makeModeReply(clients[fd], channel, modes);
  sendString(se, channels[channel_idx].getUserFds());
  // :root!root@127.0.0.1 MODE #hi +o :root_
  // :root!root@127.0.0.1 MODE #hi -i+o-t :root_

  (void)fd;
}