#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <unistd.h>

#include <string>

void sendString(std::string str, int fd);
void sendString(std::string str, std::vector<int> fds);

template <typename T>
void printArg(char const *str, T arg) {
  write(1, str, std::strlen(str));
  if (arg != 0) {
    std::string arg_str = std::to_string(arg);
    write(1, arg_str.c_str(), arg_str.length());
  }
  write(1, "\n", 1);
}

#endif
