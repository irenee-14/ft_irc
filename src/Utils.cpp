#include "Utils.hpp"

#include <unistd.h>

#include <iostream>
#include <vector>

void sendString(std::string str, int fd) {
  write(1, str.c_str(), str.length());
  write(fd, str.c_str(), str.length());
}

void sendString(std::string str, std::vector<int> fds) {
  write(1, "??????\n", 7);
  std::cout << fds.size() << std::endl;
  for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); ++it) {
    write(1, str.c_str(), str.length());
    write(*it, str.c_str(), str.length());
  }
}