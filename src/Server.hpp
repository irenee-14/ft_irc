#ifndef Server_HPP
#define Server_HPP

#include <netinet/in.h>  //struct sockaddr_in
#include <sys/poll.h>    // struct pollfd

#include <vector>

class Server {
 private:
  int serv_fd;
  struct sockaddr_in serv_adr;
  std::vector<struct pollfd> fds;

  Server(void);
  Server(const Server& src);
  Server& operator=(Server const& rhs);

 public:
  Server(char** argv);
  ~Server(void);

  int getServFd() const;
  const std::vector<struct pollfd> getPollFds() const;
};

void error_handling(const std::string& message);
#endif
