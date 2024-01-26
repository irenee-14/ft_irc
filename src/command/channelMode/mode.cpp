#include <cstdlib>  // atoi
#include <sstream>  // l : stringstream

#include "Server.hpp"

const std::string ERR_INVALIDMODEPARAM(const std::string nickname,
                                       const std::string channel,
                                       const char mode,
                                       const std::string rawMode) {
  return (":" + SERVER_NAME + " 696 " + nickname + " " + channel + " " + mode +
          " * :You must specify a parameter for the " + rawMode +
          " mode. Syntax: <" + rawMode + ">.\r\n");
}

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
  const std::string nickname = clients[fd].getNick();

  const std::string channel = tokens[1];
  const std::string channelNoHash = channel.substr(1, channel.size() - 1);

  int channel_idx = isChannel(channelNoHash);
  bool isAddMode = true;

  if (channel_idx < 0) return;

  // tokens[2] 에 값이 없을 때 채널의 현재 모드 정보 출력
  // :irc.local 324 root #hi +lnt :3
  // :irc.local 329 root #hi :1706258904
  if (tokens.size() < 3) {
    std::string se = ":" + SERVER_NAME + " 324 " + nickname + " " + channel +
                     " +" + channels[channel_idx].getModes(fd) + "\r\n";
    sendString(se, fd);

    std::string se2 = ":" + SERVER_NAME + " 329 " + nickname + " " + channel +
                      " " + intToString(channels[channel_idx].getTimestamp()) +
                      "\r\n";
    sendString(se2, fd);
    return;
  }

  std::string modeStr = tokens[2];

  // tokens에서 3번째부터 끝까지 모드 인자로 받기
  std::vector<std::string> modeArgs =
      std::vector<std::string>(tokens.begin() + 3, tokens.end());

  std::vector<std::string>::iterator it = modeArgs.begin();

  std::vector<t_mode> modes;  // 모드 정보를 담는 vector, 모드 정보는
                              // isAddMode, option, arg로 구성됨

  for (unsigned int i = 0; i < modeStr.size(); ++i) {
    const char mode = modeStr[i];
    if (mode == '+')
      isAddMode = true;
    else if (mode == '-')
      isAddMode = false;
    // ----------------------------------------------------------------------
    // isOperator
    // 필요한 인자가 다 있고, operator가 아닐 경우
    // :irc.local 482 root_ #hi :You must have channel op access or above to set
    // channel mode o

    else if (channels[channel_idx].isOperator(fd) < 0) {
      std::string un = isAddMode ? "" : "un";
      if (((mode == 'o' || mode == 'k') && it == modeArgs.end()) ||
          (isAddMode && mode == 'l' && it == modeArgs.end()))
        ;
      else {
        std::string se = ":" + SERVER_NAME + " 482 " + nickname + " " +
                         channel +
                         " :You must have channel op access or above to " + un +
                         "set channel mode " + mode + "\r\n";
        sendString(se, fd);
        continue;
      }
    }
    // -------------------------------- "o" --------------------------------
    if (mode == 'o') {
      // *it에 값 없을 경우
      if (it == modeArgs.end()) {
        sendString(ERR_INVALIDMODEPARAM(nickname, channel, mode, "op"), fd);
        continue;
      }
      // server에 user가 존재하는지 확인
      int user_fd = isUser(*it);
      if (user_fd < 0) {
        // 401 ERR_NOSUCHNICK
        // :irc.local 401 root nick :No such nick
        std::string se = ":" + SERVER_NAME + " 401 " + nickname + " " + *it +
                         " :No such nick\r\n";
        sendString(se, fd);
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
      modes.push_back(t_mode(isAddMode, mode, *it));
      it++;
    }
    // -------------------------------- "k" --------------------------------
    // MODE +k [key]
    else if (mode == 'k') {
      // *it에 값 없을 경우
      // :irc.local 696 root #hi k * :You must specify a parameter for the
      // key mode. Syntax: <key>.

      if (it == modeArgs.end()) {
        sendString(ERR_INVALIDMODEPARAM(nickname, channel, mode, "key"), fd);
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

      modes.push_back(t_mode(isAddMode, mode, *it));
      it++;
    }
    // -------------------------------- "l" --------------------------------
    else if (mode == 'l') {
      //*it에 값 없을 경우
      // :irc.local 696 root #hi l * :You must specify a parameter for the limit
      // mode. Syntax: <limit>.

      if (isAddMode && it == modeArgs.end()) {
        sendString(ERR_INVALIDMODEPARAM(nickname, channel, mode, "limit"), fd);
        continue;
      }

      // channel에 limit 추가
      if (isAddMode) {
        int retLimit = std::atoi((*it).c_str());
        channels[channel_idx].setLimit(retLimit);

        modes.push_back(t_mode(isAddMode, mode, intToString(retLimit)));
        it++;
      } else if (channels[channel_idx].getLimit() != -1) {
        channels[channel_idx].setLimit(-1);
        modes.push_back(t_mode(isAddMode, mode, ""));
      }
    }

    // -------------------------------- "i" --------------------------------
    else if (mode == 'i') {
      if (isAddMode == channels[channel_idx].getInviteOnlyMode()) {
        continue;
      }

      if (isAddMode)
        channels[channel_idx].setInviteOnlyMode(true);
      else
        channels[channel_idx].setInviteOnlyMode(false);
      modes.push_back(t_mode(isAddMode, 'i', ""));
    }
    // -------------------------------- "t" --------------------------------
    else if (mode == 't') {
      if (isAddMode == channels[channel_idx].getTopicMode()) {
        continue;
      }

      if (isAddMode)
        channels[channel_idx].setTopicMode(true);
      else
        channels[channel_idx].setTopicMode(false);
      modes.push_back(t_mode(isAddMode, 't', ""));
    }
    // 없는 모드일 경우 에러 처리
    // :irc.local 472 root q :is an unknown mode character
    else if (mode != '+' && mode != '-') {
      std::string se = ":" + SERVER_NAME + " 472 " + nickname + " " + mode +
                       " :is an unknown mode character\r\n";
      sendString(se, fd);
      continue;
    }

    if (mode != 'o') channels[channel_idx].setMode(mode, isAddMode);
  }

  // string 만들어서 출력
  if (modes.size() == 0) return;
  std::string se = makeModeReply(clients[fd], channel, modes);
  sendString(se, channels[channel_idx].getUserFds());
  // :root!root@127.0.0.1 MODE #hi +o :root_
  // :root!root@127.0.0.1 MODE #hi -i+o-t :root_

  (void)fd;
}
