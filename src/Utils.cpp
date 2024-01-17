#include "Utils.hpp"

#include <unistd.h>

void sendString(std::string str, int fd) {
  write(1, str.c_str(), str.length());
  write(fd, str.c_str(), str.length());
}