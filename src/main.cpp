// #include <arpa/inet.h>
// #include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
// #include <vector>

#include "Server.hpp"

#define BUF_SIZE 1000

void error_handling(const std::string& message);
void check_command(struct pollfd fds, char* buf, int str_len);

int main(int argc, char* argv[]) {
  //  int serv_sock;
  int clnt_sock;
  //  struct sockaddr_in serv_adr;
  struct sockaddr_in clnt_adr;
  socklen_t adr_sz;
  int str_len, i;
  char buf[BUF_SIZE];

  if (argc != 2) {
    std::cout << "Usage : " << argv[0] << " <port>" << std::endl;
    exit(1);
  }

  //  // 서버 소켓 생성
  //  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  //  if (serv_sock == -1) {
  //    error_handling("socket() error");
  //  }

  //  // 서버에 소켓 주소 생성
  //  memset(&serv_adr, 0, sizeof(serv_adr));
  //  serv_adr.sin_family = AF_INET;
  //  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  //  serv_adr.sin_port = htons(atoi(argv[1]));
  //  int enable = 1;
  //  if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable,
  //                 sizeof(enable)) == -1)
  //    error_handling("setsockopt() error");
  //  if (fcntl(serv_sock, F_SETFL, O_NONBLOCK) == -1)
  //    error_handling("fcntl() error");
  //  // 소켓과 서버 주소 연결
  //  if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  //  {
  //    error_handling("bind() error");
  //  }

  //  if (listen(serv_sock, 5) == -1) {
  //    error_handling("listen() error");
  //  }

  //  std::vector<struct pollfd> fds(2);
  //  fds[0].fd = serv_sock;
  //  fds[0].events = POLLIN;

  /////////////////////////////////////////////
  //  Server serv(argv);
  //  while (true) {
  //    std::vector<struct pollfd> *pollFds = serv.getPollFds();
  //    int fd_num = poll(pollFds[0], serv.getPollFds().size(), -1);
  //    if (fd_num == -1) {
  //      error_handling("poll() error");
  //    }
  //    // server 소켓에 연결요청
  //    if (fds[0].revents & POLLIN) {
  //      if (fds[0].fd == serv_sock) {
  //        adr_sz = sizeof(clnt_adr);
  //        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

  //        fds.push_back(pollfd());
  //        fds.back().fd = clnt_sock;
  //        fds.back().events = POLLIN;

  //        std::cout << "connected client: " << clnt_sock << std::endl;

  //        const char* connected_msg = "server와 연결\n";
  //        write(clnt_sock, connected_msg, strlen(connected_msg));
  //      }
  //      continue;
  //    }
  //    // client에서 온 요청

  //    for (i = 1; i < (int)fds.size(); ++i) {
  //      if (fds[i].revents & POLLIN) {
  //        {
  //          str_len = recv(fds[i].fd, buf, BUF_SIZE, 0);
  //          // ctrl + D
  //          if (str_len == 0) {
  //            close(fds[i].fd);
  //            fds.erase(fds.begin() + i);  // fds 벡터에서 해당 pollfd 제거
  //            std::cout << "closed client: " << fds[i].fd << std::endl;
  //          } else {
  //            write(1, buf, str_len);
  //            check_command(fds[i], buf, str_len);
  //          }
  //        }
  //      }
  //    }
  //  }
  //  close(serv_sock);
  return 0;
}

void error_handling(const std::string& message) {
  std::cerr << message << std::endl;
  exit(1);
}

void check_command(struct pollfd fds, char* buf, int str_len) {
  if (buf[str_len - 1] == '\n' && buf[str_len - 2] == '\r') {
    std::string strr(buf);
    const char* se = ":127.0.0.1 001 jihylim :Welcome\r\n";
    if (strr.find("CAP") == 0 || strr.find("NICK") == 0)
      send(fds.fd, se, strlen(se), 0);
  }
  (void)str_len;
  (void)fds;
}
