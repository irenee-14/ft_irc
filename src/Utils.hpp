#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <unistd.h>

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

void sendString(std::string str, int fd);
void sendString(std::string str, std::vector<int> fds);

template <typename T>
void printArg(char const *str, T arg) {
  write(1, str, std::strlen(str));
  if (arg != 0) {
    std::stringstream ss;
    ss << arg;
    std::string arg_str = ss.str();
    write(1, arg_str.c_str(), arg_str.length());
  }
  write(1, "\n", 1);
}

std::string intToString(int value);

#endif
