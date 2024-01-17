#include "Server.hpp"

#include <arpa/inet.h>  // htons
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>  // close

#include <iostream>
#include <sstream>
#include <string>

#include "Client.hpp"

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
    *this = rhs;
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
        clients.push_back(Client(clnt_sock));

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
        {
          write(1, buf, str_len);
          check_command(fds[i], buf, str_len);
          write(1, "\n--check--\n\n", 12);
        }
      }
    }
  }
}

// 커맨드랑 인자 파싱하기
// 커맨드 분류
// 커맨드 따라서 필요한 인자 확인
// 커맨드 별 보내야 할 메시지 파악하기
// 처음 접속 시 ":127.0.0.1 001 jihyunlim :Welcome\r\n" 등의 연결 확인 코드
// 보내기

/*
--------------------------------------------
### 필수 command

PASS : 서버에 연결하기 위한 패스워드 확인
NICK : 닉네임 변경
USER : 유저 이름 변경
JOIN : 채널 입장
PART : 채널 나가기
PRIVMSG : 특정 사용자 또는 채널에 메시지를 보내기

운영자와 일반 사용자 존재
- KICK : 특정 채널에서 유저를 내보냄
- INVITE : 특정 채널로 유저 초대
- TOPIC : 특정 채널의 주제 설정
- MODE : 채널의 모드 설정
    - i : 초대 전용 채널 설정/제거
    - t : 채널 운영자에 대한 TOPIC 명령 제한 설정/제거
    - k : 채널키(비밀번호) 설정/제거
    - o : 채널 운영자 권한 부여/받기
    - l: 채널에 대한 사용자 제한을 설정/해제
QUIT : IRC 서버에서 나가기
--------------------------------------------
### 필요한 command
NOTICE
LIST : 현재 서버에서 사용 가능한 채널 목록을 조회
PING : 클라이언트-서버 간의 연결을 확인
OPER : 관리자 권한을 얻기
KICK : 유저를 특정 채널에서 내보내기
*/

std::vector<std::string> split_command(std::string str) {
  std::istringstream iss(str);
  std::vector<std::string> tokens;

  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return (tokens);
}

void Server::check_command(struct pollfd fds, char* buf, int str_len) {
  //  std::string buffer(buf, str_len);  // 버퍼 내용을 문자열로 저장

  //  if (!buffer.empty()) {
  //  std::stringstream ss(buffer);
  std::string str(buf);

  //  while (std::getline(ss, str)) {
  if (buf[str_len - 2] == '\r' && buf[str_len - 1] == '\n') {
    if (str.find("CAP LS") == 0) {
      const char* se = "CAP * LS\r\n";
      send(fds.fd, se, strlen(se), 0);
      // write(1, se, strlen(se));
      //  break;
    } else if (str.find("CAP END") == 0) {
      //  const char* se = ":127.0.0.1 001 jihyunlim :Welcome\r\n";
      //  send(fds.fd, se, strlen(se), 0);
      // 처음 들어왔을 때
      std::stringstream ss(str);
      std::string line;

      while (std::getline(ss, line)) {
        std::vector<std::string> user_token = split_command(line);
        if (user_token[0] == "CAP") {
          continue;
        } else if (user_token[0] == "NICK") {
          clients.back().setNick(user_token[1]);
        } else if (user_token[0] == "USER") {
          clients.back().setUser(user_token[1]);
          clients.back().setServerName(user_token[3]);
          clients.back().setRealName(user_token[4]);
          // write(1, "user in\n", 10);
        }
      }
      std::cout << "결과 : " << clients.back().getNick() << " "
                << clients.back().getUser() << " "
                << clients.back().getServerName() << " "
                << clients.back().getRealName() << std::endl;
      const char* se = ":127.0.0.1 001 jihyunlim :Welcome\r\n";
      send(fds.fd, se, strlen(se), 0);
      //  continue;
    } else {
      // command랑 인자로 나누기
      // 인자 받아서 인자에 맞는 함수 실행

      // std::vector<std::string> tokens = split_command(str);

      if (str.find("NICK") == 0) {
        // nickname 변경
        // std::getline(ss, str);

        // std::vector<std::string> user_token = split_command(str);

        // const char* se = ":127.0.0.1 001 jihyunlim :Welcome\r\n";
        // send(fds.fd, se, strlen(se), 0);
      } else if (str.find("JOIN") == 0) {
        const char* se = ":jihyunlim!jihyunlim@127.0.0.1 JOIN :#channel\r\n";
        send(fds.fd, se, strlen(se), 0);
      } else if (str.find("PART") == 0) {
        const char* se = ":jihyunlim!jihyunlim@127.0.0.1 PART :#channel\r\n";
        send(fds.fd, se, strlen(se), 0);
      }
      // if (str.find("NICK") == 0) {
      //   const char* se = ":jihyunlim!jihyunlim@127.0.0.1 JOIN :#channel\r\n";
      //   send(fds.fd, se, strlen(se), 0);
      // } else if (str.find("JOIN") == 0) {
      //   const char* se = ":jihyunlim!jihyunlim@127.0.0.1 JOIN :#channel\r\n";
      //   send(fds.fd, se, strlen(se), 0);
      // } else if (str.find("PART") == 0) {
      //   const char* se = ":jihyunlim!jihyunlim@127.0.0.1 PART :#channel\r\n";
      //   send(fds.fd, se, strlen(se), 0);
      // }
      //  }
    }
  }

  //  }
  (void)str_len;
}
