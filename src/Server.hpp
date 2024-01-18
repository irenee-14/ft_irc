#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <netinet/in.h>  //struct sockaddr_in
#include <sys/poll.h>    // struct pollfd

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

class Server {
 private:
  int serv_fd;
  struct sockaddr_in serv_adr;
  std::vector<struct pollfd> fds;
  unsigned int password;

  std::map<int, Client> clients;
  std::vector<Channel> channels;

  Server(void);
  Server(const Server& src);
  Server& operator=(Server const& rhs);

 public:
  Server(char** argv);
  ~Server(void);

  int getServFd() const;
  const std::vector<struct pollfd> getPollFds() const;

  void acceptLoop();
  void checkCommand(struct pollfd fds, char* buf);

  // ------------------------ cmd ------------------------ //

  void nick(int fd, std::string token);
  void user(int fd, std::vector<std::string> tokens);
  void userhost(int fd, std::vector<std::string> tokens);
  void pong(int fd);
  void list(void);
  void quit(int fd);

  // ------------------- cmdInChannel -------------------- //

  void join(int fd, std::string token);
  std::string userList(Channel& channel);
  void part(int fd, std::string token);
  void msg(int fd, std::vector<std::string> token);
};

#endif
