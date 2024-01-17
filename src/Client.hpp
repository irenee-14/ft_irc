#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <netinet/in.h>

#include <string>
class Client {
 private:
  int _clnt_fd;
  //  socklen_t addr_len;

  std::string _nick;
  std::string _user;
  std::string _real_name;
  std::string _server_name;

  Client(void);
  Client& operator=(Client const& rhs);

 public:
  Client(int fd);
  Client(const Client& src);
  ~Client(void);

  void setFd(const int fd);
  void setNick(const std::string nick);
  void setUser(const std::string user);
  void setRealName(const std::string real);
  void setServerName(const std::string server_name);

  int getFd(void) const;
  std::string getNick(void) const;
  std::string getUser(void) const;
  std::string getRealName(void) const;
  std::string getServerName(void) const;
};

#endif
