#include "Server.hpp"

#include <arpa/inet.h>  // htons
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>  // close

#include <cstdlib>  // atoi

#define BUF_SIZE 512

// ===============================================================

Server::Server(void) {}

Server::Server(char** argv) {
  this->_port = std::atoi(argv[1]);
  this->_password = argv[2];
  // 서버 소켓 생성
  _serv_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (this->_serv_fd == -1) throw std::string("socket()");

  // 서버에 소켓 주소 생성
  memset(&_serv_adr, 0, sizeof(this->_serv_adr));
  this->_serv_adr.sin_family = AF_INET;
  this->_serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->_serv_adr.sin_port = htons(_port);

  int flag = 1;
  if (setsockopt(this->_serv_fd, SOL_SOCKET, SO_REUSEADDR, &flag,
                 sizeof(flag)) == -1)
    throw std::string("setsockopt()");

  if (fcntl(this->_serv_fd, F_SETFL, O_NONBLOCK) == -1)
    throw std::string("fcntl()");

  // 소켓과 서버 주소 연결
  if (bind(this->_serv_fd, (struct sockaddr*)&this->_serv_adr,
           sizeof(this->_serv_adr)) == -1)
    throw std::string("bind()");

  if (listen(this->_serv_fd, 5) == -1) throw std::string("listen()");

  _fds.push_back(pollfd());
  _fds[0].fd = this->_serv_fd;
  _fds[0].events = POLLIN;
  initializeCommandList();
}

Server::Server(const Server& src) { *this = src; }

Server::~Server(void) { close(this->_serv_fd); }

Server& Server::operator=(Server const& rhs) {
  if (this != &rhs) {
    this->_serv_fd = rhs._serv_fd;
    this->_serv_adr = rhs._serv_adr;
    this->_fds = rhs._fds;
    this->_read_buf = rhs._read_buf;
    this->_port = rhs._port;
    this->_password = rhs._password;
    this->_command_list = rhs._command_list;
    this->clients = rhs.clients;
    this->channels = rhs.channels;
  }
  return (*this);
}

// ===============================================================

void Server::initializeCommandList() {
  if (_command_list.empty()) {
    _command_list["NICK"] = 1;
    _command_list["USER"] = 2;
    _command_list["userhost"] = 3;
    _command_list["PING"] = 4;
    _command_list["LIST"] = 5;
    _command_list["WHOIS"] = 6;
    _command_list["JOIN"] = 7;
    _command_list["PART"] = 8;
    _command_list["PRIVMSG"] = 9;
    _command_list["NOTICE"] = 10;
    _command_list["KICK"] = 11;
    _command_list["INVITE"] = 12;
    _command_list["TOPIC"] = 13;
    _command_list["MODE"] = 14;
    _command_list["motd"] = 15;
    _command_list["QUIT"] = 16;
  }
}

// ---------------------------------------------------------------

int Server::getServFd() const { return (this->_serv_fd); }

const std::vector<struct pollfd> Server::getPollFds() const {
  return (this->_fds);
}

std::string Server::getReadBuf() const { return (this->_read_buf); }

void Server::setReadBuf(std::string const& buf) { this->_read_buf += buf; }

void Server::clearReadBuf() {
  this->_read_buf.clear();
  this->_read_buf = "";
}

// ----------------------------------------------------------------------

int Server::isChannel(std::string channel_name) {
  for (unsigned int i = 0; i < channels.size(); i++) {
    if (channels[i].getChannelName() == channel_name) {
      return (i);
    }
  }
  return (-1);
}

int Server::isUser(std::string nickname) {
  for (unsigned int i = 0; i < clients.size(); i++) {
    if (clients[i].getNick() == nickname) {
      return (i);
    }
  }
  return (-1);
}

// ---------------------------------------------------------------

void Server::acceptClient(void) {
  struct sockaddr_in clnt_adr;

  socklen_t adr_sz = sizeof(clnt_adr);
  int clnt_sock = accept(_fds[0].fd, (struct sockaddr*)&clnt_adr, &adr_sz);

  _fds.push_back(pollfd());
  _fds.back().fd = clnt_sock;
  _fds.back().events = POLLIN;

  // 클라이언트 새로 생성 후 fd 할당
  clients[clnt_sock] = Client(clnt_sock);
  printArg("\n===============================================\n", "");
  printArg("connected client: ", clnt_sock);
}

void Server::disconnectClient(int fd) {
  close(fd);

  for (size_t i = 0; i < _fds.size(); ++i) {
    if (_fds[i].fd == fd) {
      _fds.erase(_fds.begin() + i);
      break;
    }
  }
  printArg("closed client: ", fd);
}

// ---------------------------------------------------------------

void Server::acceptLoop(void) {
  while (true) {
    int fd_num = poll(_fds.data(), _fds.size(), -1);

    if (fd_num == -1) throw std::string("poll()");

    // server 소켓에 연결요청
    if (_fds[0].revents & POLLIN) {
      if (_fds[0].fd == _serv_fd) {
        acceptClient();
      }
      continue;
    }
    // client에서 온 요청
    for (unsigned int i = 1; i < _fds.size(); ++i) {
      if (_fds[i].revents & (POLLHUP | POLLERR))  // POLLHUP : 연결 끊어짐
      {
        disconnectClient(_fds[i].fd);
        continue;
      } else if (_fds[i].revents & POLLIN) {
        recvMsg(_fds[i].fd);
        continue;
      }
    }
  }
}

void Server::recvMsg(int fd) {
  char buf[BUF_SIZE + 1];
  memset(buf, 0, BUF_SIZE + 1);

  int recv_len;
  recv_len = recv(fd, buf, BUF_SIZE, 0);
  buf[recv_len] = '\0';
  // write(1, buf, recv_len);

  if (recv_len < 0) {
    printArg("recv failed", "");
    this->disconnectClient(fd);
    return;
  } else if (recv_len == 0) {
    this->disconnectClient(fd);
    return;
  } else {
    this->setReadBuf(buf);

    if (findCRLF(this->getReadBuf()) != std::string::npos) {
      write(1, buf, recv_len);
      printArg("\n-----------------------------------------------\n", "");
      try {
        checkCommand(fd, this->getReadBuf());
        this->clearReadBuf();
      } catch (std::string exception) {
        // client에게도 에러 메시지 보내줘야함, pass 불일치
        std::string se =
            "ERROR Closing link: [Access denied by configuration]\r\n";
        sendString(se, fd);
        disconnectClient(fd);
        this->clearReadBuf();
      }
      printArg("\n===============================================\n", "");
    }
  }
}
