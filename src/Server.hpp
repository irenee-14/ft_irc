#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <netinet/in.h>  //struct sockaddr_in
#include <sys/poll.h>    // struct pollfd

#include <map>
#include <vector>

class Client;
class Server {
 private:
  int serv_fd;
  struct sockaddr_in serv_adr;
  std::vector<struct pollfd> fds;
  unsigned int password;

  // clients
  std::map<int, Client> clients;
  // channels

  Server(void);
  Server(const Server& src);
  Server& operator=(Server const& rhs);

 public:
  Server(char** argv);
  ~Server(void);

  int getServFd() const;
  const std::vector<struct pollfd> getPollFds() const;

  void acceptLoop();
  void check_command(struct pollfd fds, char* buf, int str_len);
};

#endif
