#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>

void sendString(std::string str, int fd);
void sendString(std::string str, std::vector<int> fds);

#endif
