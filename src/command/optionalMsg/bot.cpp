#include "Server.hpp"

void bot_msg(int fd, std::string nickname, std::string msg) {
  std::string se =
      ":BOT!BOT@" + SERVER_NAME + " PRIVMSG " + nickname + " :" + msg + "\r\n";
  sendString(se, fd);
}

void Server::bot(int fd, std::string nickname, std::string msg) {
  // hi/hello
  // 사용 방법 소개, 원하는 숫자를 입력하면 저녁 메뉴를 추천

  // help/HELP 모르는 명령어. help로 사용할 수 있는 명령어

  // 저메추
  // 가위바위보

  // if (msg == )
  // 숫자 입력
  // 랜덤하게 메뉴 출력
  std::stringstream ss;
  ss << msg;

  // std::string se;
  // 저메추,

  srand(time(NULL));

  int random = rand() % 3;
  std::cout << random << std::endl;
  switch (random + 1) {
    case 1:
      bot_msg(fd, nickname, "떡볶이");
      break;
    case 2:
      bot_msg(fd, nickname, "피자");
      break;
    default:
      bot_msg(fd, nickname, "엽떡");
      break;
  }

  // bot_msg(fd, nickname, se);
}

// hi -> hello. 저녁메뉴 추천 가능
// 랜덤한 숫자를 넣어봐
// 출력
