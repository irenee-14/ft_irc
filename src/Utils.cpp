#include "Utils.hpp"

#include <algorithm>  // find

void ft_write(int __fd, const void *__buf, size_t __nbyte) {
  size_t len = write(__fd, __buf, __nbyte);
  if (len < 0) throw std::string("write faild");
}

void sendString(std::string str, int fd) {
  ft_write(1, str.c_str(), str.length());
  ft_write(fd, str.c_str(), str.length());
}

void sendString(std::string str, std::vector<int> fds) {
  for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); ++it) {
    ft_write(1, str.c_str(), str.length());
    ft_write(*it, str.c_str(), str.length());
  }
}

std::string intToString(int value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

int stringToInt(std::string str) { return (std::atoi(str.c_str())); }

// -------------------------------------------------------------
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

std::vector<std::string> commaSplit(std::string str) {
  std::vector<std::string> result;
  std::string temp;

  std::stringstream ss;
  ss.str("");
  ss << str;

  char c;

  while (ss.get(c)) {
    if (c == ',') {
      if (!temp.empty() &&
          (std::find(result.begin(), result.end(), temp) == result.end())) {
        result.push_back(temp);
        temp.clear();
      }
    } else
      temp += c;
  }
  if (!temp.empty() &&
      (std::find(result.begin(), result.end(), temp) == result.end()))
    result.push_back(temp);
  ss.str("");
  return (result);
}

std::vector<std::string> spaceSplit(std::string str) {
  std::stringstream ss(str);
  std::string token;
  std::vector<std::string> tokens;

  while (getline(ss, token, ' ')) {
    tokens.push_back(token);
  }
  return (tokens);
}
