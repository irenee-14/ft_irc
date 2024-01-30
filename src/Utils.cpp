#include "Utils.hpp"

#include <algorithm>  // find

void sendString(std::string str, int fd) {
  write(1, str.c_str(), str.length());
  write(fd, str.c_str(), str.length());
}

void sendString(std::string str, std::vector<int> fds) {
  for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); ++it) {
    write(1, str.c_str(), str.length());
    write(*it, str.c_str(), str.length());
  }
}

std::string intToString(int value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

size_t findCRLF(std::string buf) {
  size_t crPos = buf.find("\r");
  size_t lfPos = buf.find("\n");
  if (lfPos == std::string::npos) return crPos;
  if (crPos == std::string::npos) return lfPos;
  return std::min(crPos, lfPos);
}
// -------------------------------------------------------------

bool isSpecialChar(char c) {
  // "[", "]", "\", "`", "_", "^", "{", "|", "}"
  if (c == '[' || c == ']' || c == '\\' || c == '`' || c == '_' || c == '^' ||
      c == '{' || c == '|' || c == '}' || c == '_') {
    return true;
  }
  return false;
}

bool isValidNick(std::string nickname) {
  if (nickname.length() > 9) return false;
  if (!isalpha(nickname[0]) && !isSpecialChar(nickname[0])) return false;
  for (unsigned int i = 1; i < nickname.length(); ++i) {
    if (!isalnum(nickname[i]) && nickname[i] != '-' &&
        !isSpecialChar(nickname[i]))
      return false;
  }
  return true;
}

// -------------------------------------------------------------
// flag == 0 : 중복 제거
// flag == 1 : 중복 허용

std::vector<std::string> commaSplit(std::string str, int flag) {
  std::vector<std::string> result;
  std::string temp;

  std::stringstream ss;
  ss.str("");
  ss << str;

  char c;

  while (ss.get(c)) {
    if (c == ',') {
      if (!temp.empty()) {
        if (flag == 0) {
          if (std::find(result.begin(), result.end(), temp) == result.end())
            result.push_back(temp);
        } else
          result.push_back(temp);
        temp.clear();
      }
    } else
      temp += c;
  }
  if (!temp.empty()) {
    if (flag == 0) {
      if (std::find(result.begin(), result.end(), temp) == result.end())
        result.push_back(temp);
    } else
      result.push_back(temp);
    temp.clear();
  }
  ss.str("");
  return (result);
}
