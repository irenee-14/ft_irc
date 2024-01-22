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

enum command_enum {
  NICK = 0,
  USER,
  USERHOST,
  PING,
  LIST,
  WHOIS,
  JOIN,
  PART,
  PRIVMSG,
  NOTICE,
  KICK,
  INVITE,
  TOPIC,
  MODE,
  QUIT
};
class Server {
 private:
  int serv_fd;
  struct sockaddr_in serv_adr;
  std::vector<struct pollfd> fds;

  // -------------------------------------------------------------

  std::string _password;
  std::map<std::string, int> command_list;

  // -------------------------------------------------------------
  std::map<int, Client> clients;
  std::vector<Channel> channels;

  // -------------------------------------------------------------

  Server(void);
  Server(const Server& src);
  Server& operator=(Server const& rhs);

 public:
  Server(char** argv);
  ~Server(void);

  // -------------------------------------------------------------

  void initializeCommandList();

  // -------------------------------------------------------------

  int getServFd() const;
  const std::vector<struct pollfd> getPollFds() const;

  // -------------------------------------------------------------

  void acceptLoop();
  void executeCommand(int fd, std::vector<std::string> tokens);
  void checkCommand(struct pollfd fds, char* buf);

  // ---------------------------- cmd ----------------------------

  void list(int fd, std::string channel);
  void nick(int fd, std::string nickname);
  void pass(int fd, std::string password);
  void pong(int fd);
  void quit(int fd);
  void user(int fd, std::vector<std::string> tokens);
  void userhost(int fd, std::vector<std::string> tokens);
  void whois(int fd, std::string nickname);

  // ----------------------- cmdInChannel -------------------------

  void invite(int fd, std::vector<std::string> tokens);
  std::string userList(Channel& channel);
  void join(int fd, std::string channel);
  void kick(int fd, std::vector<std::string> tokens);
  void part(int fd, std::string channel);

  void privateMsg(int fd, std::vector<std::string> tokens);
  void msg(int fd, std::vector<std::string> tokens, std::string cmd);
  void notice(int fd, std::vector<std::string> tokens);

  void topic(int fd, std::vector<std::string> tokens);

  // --------------------------------------------------------------

  int isChannel(std::string channel_name);
  int isUser(std::string nickname);
};

#endif
