#include "Utils.hpp"

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
