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
        // write(1, se, strlen(se));
        //  break;
      } else if (str.find("CAP END") == 0 || str.find("JOIN :") == 0) {
        std::cout << "cap end or join" << std::endl;

      } else {
        std::vector<std::string> tokens = splitCommand(str);

        if (tokens[0] == "NICK")
          nick(fds.fd, tokens[1]);

        else if (tokens[0] == "USER")
          user(fds.fd, tokens);

        else if (str.find("userhost") == 0)
          userhost(fds.fd, tokens);

        else if (str.find("PING") == 0)
          pong(fds.fd);

        // LIST : 현재 서버에서 사용 가능한 채널 목록을 조회
        else if (str.find("LIST") == 0)
          list(fds.fd, tokens[1]);
        else if (str.find("WHOIS") == 0)
          whois(fds.fd, tokens[1]);
        // -------------------------------------------
        else if (str.find("JOIN") == 0)
          join(fds.fd, tokens[1]);

        else if (str.find("PART") == 0)
          part(fds.fd, tokens[1]);

        // PRIVMSG : 특정 사용자 또는 채널에 메시지를 보내기
        else if (str.find("PRIVMSG") == 0)
          privateMsg(fds.fd, tokens);

        // NOTICE : PRIVMSG와 비슷하지만, 서버가 보낸 메시지에 대한 응답을
        // 보낼 때 사용
        else if (str.find("NOTICE") == 0)
          notice(fds.fd, tokens);
        // KICK : 유저를 특정 채널에서 내보내기
        else if (str.find("KICK") == 0) {
        }
        // INVITE : 특정 채널로 유저 초대
        else if (str.find("INVITE") == 0) {
        }
        // TOPIC : 특정 채널의 주제 설정
        else if (str.find("TOPIC") == 0) {
        }
        // MODE : 채널의 모드 설정
        // - i : 초대 전용 채널 설정/제거
        // - t : 채널 운영자에 대한 TOPIC 명령 제한 설정/제거
        // - k : 채널키(비밀번호) 설정/제거
        // - o : 채널 운영자 권한 부여/받기
        // - l: 채널에 대한 사용자 제한을 설정/해제
        else if (str.find("MODE") == 0) {
        }
        // --------------------------------------------------
        else if (str.find("QUIT") == 0)
          quit(fds.fd);
      }
    }
  }
}
