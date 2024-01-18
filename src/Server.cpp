#include "Server.hpp"

#include <arpa/inet.h>  // htons
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>  // close

#define BUF_SIZE 512

void check_password(int pass) { (void)pass; }

//*****************************

Server::Server(void) {}

Server::Server(char** argv) {
  // 서버 소켓 생성
  serv_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (this->serv_fd == -1) throw std::string("socket()");

  // 서버에 소켓 주소 생성
  memset(&serv_adr, 0, sizeof(this->serv_adr));
  this->serv_adr.sin_family = AF_INET;
  this->serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->serv_adr.sin_port = htons(atoi(argv[1]));

  int flag = 1;
  if (setsockopt(this->serv_fd, SOL_SOCKET, SO_REUSEADDR, &flag,
                 sizeof(flag)) == -1)
    throw std::string("setsockopt()");

  if (fcntl(this->serv_fd, F_SETFL, O_NONBLOCK) == -1)
    throw std::string("fcntl()");

  // 소켓과 서버 주소 연결
  if (bind(this->serv_fd, (struct sockaddr*)&this->serv_adr,
           sizeof(this->serv_adr)) == -1)
    throw std::string("bind()");

  if (listen(this->serv_fd, 5) == -1) throw std::string("listen()");

  fds.push_back(pollfd());
  fds[0].fd = this->serv_fd;
  fds[0].events = POLLIN;
}

Server::Server(const Server& src) { *this = src; }

Server::~Server(void) { close(this->serv_fd); }

Server& Server::operator=(Server const& rhs) {
  if (this != &rhs) {
    this->serv_fd = rhs.serv_fd;
    this->serv_adr = rhs.serv_adr;
    this->fds = rhs.fds;
    this->clients = rhs.clients;
    this->channels = rhs.channels;
  }
  return (*this);
}

// --------------------------------------------------------------- //

int Server::getServFd() const { return (this->serv_fd); }

const std::vector<struct pollfd> Server::getPollFds() const {
  return (this->fds);
}

// --------------------------------------------------------------- //

void Server::acceptLoop() {
  while (true) {
    int fd_num = poll(fds.data(), fds.size(), -1);

    if (fd_num == -1) throw std::string("poll()");

    // server 소켓에 연결요청
    if (fds[0].revents & POLLIN) {
      if (fds[0].fd == serv_fd) {
        struct sockaddr_in clnt_adr;

        socklen_t adr_sz = sizeof(clnt_adr);
        int clnt_sock = accept(serv_fd, (struct sockaddr*)&clnt_adr, &adr_sz);

        fds.push_back(pollfd());
        fds.back().fd = clnt_sock;
        fds.back().events = POLLIN;

        // 클라이언트 새로 생성 후 fd 할당
        clients[clnt_sock] = Client(clnt_sock);

        std::cout << "connected client: " << clnt_sock << std::endl;

        const char* connected_msg = "server와 연결\n";
        write(clnt_sock, connected_msg, strlen(connected_msg));

        check_password(password);
      }
      continue;
    }

    // client에서 온 요청
    for (unsigned int i = 1; i < fds.size(); ++i) {
      if (fds[i].revents & (POLLHUP | POLLERR))  // POLLHUP : 연결 끊어짐
      {
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
        std::cout << "closed client: " << fds[i].fd << std::endl;
      } else if (fds[i].revents & POLLIN) {
        char buf[BUF_SIZE];
        int str_len;
        str_len = recv(fds[i].fd, buf, BUF_SIZE, 0);
        //  ctrl + D
        //  if (str_len == 0) {
        //    close(fds[i].fd);
        //    fds.erase(fds.begin() + i);
        //    std::cout << "closed client: " << fds[i].fd << std::endl;
        //  } else
        buf[str_len++] = '\0';
        {
          write(1, buf, str_len);
          checkCommand(fds[i], buf);
          write(1, "\n--check--\n\n", 12);
        }
      }
    }
  }
}
