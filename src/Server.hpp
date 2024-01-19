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


enum command_enum{ NICK = 0, USER, USERHOST, PING, LIST, WHOIS, JOIN, PART, PRIVMSG, NOTICE, KICK, INVITE, TOPIC, MODE, QUIT};
class Server {
 private:
  int serv_fd;
  struct sockaddr_in serv_adr;
  std::vector<struct pollfd> fds;
  std::string password;

  std::map<int, Client> clients;
  std::vector<Channel> channels;

  Server(void);
  Server(const Server& src);
  Server& operator=(Server const& rhs);

 public:
  Server(char** argv);
  ~Server(void);

  // -------------------------------------------------------------
  static std::map<std::string, int> command_list;
  static void initializeCommandList();

  // ---------------------------------------------------------------

  int getServFd() const;
  const std::vector<struct pollfd> getPollFds() const;

  // -------------------------------------------------------------

  void acceptLoop();
  void checkCommand(struct pollfd fds, char* buf);

  // ---------------------------- cmd ----------------------------

  void pass(int fd, std::string token);
  void nick(int fd, std::string token);
  void user(int fd, std::vector<std::string> tokens);
  void userhost(int fd, std::vector<std::string> tokens);
  void pong(int fd);
  void list(int fd, std::string token);
  void whois(int fd, std::string token);
  void quit(int fd);

  // ----------------------- cmdInChannel -------------------------

  void join(int fd, std::string token);
  std::string userList(Channel& channel);
  void part(int fd, std::string token);

  void msg(int fd, std::vector<std::string> tokens, std::string cmd);
  void notice(int fd, std::vector<std::string> tokens);
  void privateMsg(int fd, std::vector<std::string> tokens);

  void kick(int fd, std::vector<std::string> tokens);
  void topic(int fd, std::vector<std::string> tokens);
};

#endif
