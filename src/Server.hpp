#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <netinet/in.h>  //struct sockaddr_in
#include <sys/poll.h>    // struct pollfd

#include <iostream>
#include <map>

#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

const std::string SERVER_NAME = "localhost";

enum command_enum {
  NICK = 1,
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

typedef struct s_mode {
  bool _isAddMode;
  char _option;
  std::string _arg;
  s_mode(bool isAddMode, char option, std::string arg)
      : _isAddMode(isAddMode), _option(option), _arg(arg) {}
} t_mode;

class Server {
 private:
  int _serv_fd;
  struct sockaddr_in _serv_adr;
  std::vector<struct pollfd> _fds;
  std::string _read_buf;

  // -------------------------------------------------------------

  std::string _password;
  std::map<std::string, int> _command_list;

  // -------------------------------------------------------------
  // ????????clients fd 그대로 쓸 수 있는거 확인해서 바꾸기
  std::map<int, Client> clients;
  std::vector<Channel> channels;

  // -------------------------------------------------------------

  Server(void);
  Server(const Server& src);
  Server& operator=(Server const& rhs);

  // =============================================================

 public:
  Server(char** argv);
  ~Server(void);

  // -------------------------------------------------------------

  void initializeCommandList();

  // -------------------------------------------------------------

  int getServFd() const;
  const std::vector<struct pollfd> getPollFds() const;

  std::string getReadBuf() const;
  void setReadBuf(std::string const& buf);
  void clearReadBuf();

  // --------------------------- is ------------------------------

  int isChannel(std::string channel_name);
  int isUser(std::string nickname);

  // -------------------------- loop -----------------------------
  void acceptClient(void);
  void disconnectClient(int fd);
  void acceptLoop(void);
  void recvMsg(int fd);

  // --------------------------

  void executeCommand(int fd, std::vector<std::string> tokens);
  void checkCommand(int fd, std::string buf);

  // =============================================================
  // ---------------------- channel Mode -------------------------

  void mode(int fd, std::vector<std::string> tokens);

  bool processModeO(int fd, std::vector<std::string>::iterator& it,
                    Channel& channel, const std::string nickname,
                    bool isAddMode);

  bool processModeK(std::vector<std::string>::iterator& it, Channel& channel,
                    bool isAddMode);

  std::string processModeL(std::vector<std::string>::iterator& it,
                           Channel& channel, bool isAddMode);

  // -------------------- channel Operator -----------------------

  void invite(int fd, std::vector<std::string> tokens);
  const std::string userList(const Channel& channel);
  void join(int fd, std::vector<std::string> tokens);
  void kick(int fd, std::vector<std::string> tokens);
  void list(int fd, std::vector<std::string> tokens);
  void part(int fd, std::string channel);

  void privateMsg(int fd, std::vector<std::string> tokens);
  void msg(int fd, std::vector<std::string> tokens, std::string cmd);
  void notice(int fd, std::vector<std::string> tokens);

  void topic(int fd, std::vector<std::string> tokens);

  // ------------------- connection Message ----------------------

  void nick(int fd, std::string nickname);
  void pass(int fd, std::vector<std::string> tokens);
  void pong(int fd);
  void quit(int fd);
  void user(int fd, std::vector<std::string> tokens);

  // ------------------- optional Message -----------------------

  void userhost(int fd, std::vector<std::string> tokens);
  void whois(int fd, std::string target);

  // =============================================================
};

#endif
