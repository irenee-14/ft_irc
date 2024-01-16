#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <netinet/in.h>

#include <string>
class Client {
 private:
  int clnt_fd;
  //  socklen_t addr_len;

  std::string nick;
  std::string user;
  std::string real_name;
  std::string ip;

  Client(void);
  Client& operator=(Client const& rhs);

 public:
  Client(int fd);
  Client(const Client& src);
  ~Client(void);

  void setFd(const int fd);
  int getFd(void) const;
};

#endif
