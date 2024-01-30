#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <unistd.h>

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

void ft_write(int __fd, const void *__buf, size_t __nbyte);

void sendString(std::string str, int fd);
void sendString(std::string str, std::vector<int> fds);

template <typename T>
void printArg(char const *str, T arg) {
  ft_write(1, str, std::strlen(str));

  std::stringstream ss("");
  ss << arg;
  if (ss.tellp() != std::streampos(0)) {
    std::string arg_str = ss.str();
    ft_write(1, arg_str.c_str(), arg_str.length());
  }
  ft_write(1, "\n", 1);
}

// ---------------------------------------------------

std::string intToString(int value);
int stringToInt(std::string str);

// ---------------------------------------------------
size_t findCRLF(std::string buf);

bool isValidNick(std::string nickname);

std::vector<std::string> commaSplit(std::string str);
std::vector<std::string> spaceSplit(std::string str);

#endif
