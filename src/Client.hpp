#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <netinet/in.h>

#include <string>
class Client {
 private:
  int clnt_fd;
  socklen_t addr_len;

  std::string nick;
  std::string user;
  std::string real_name;

  Client(const Client& src);
  Client& operator=(Client const& rhs);

 public:
  Client(void);
  ~Client(void);
};

#endif
