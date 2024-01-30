#include <fstream>

#include "Server.hpp"
#include "Utils.hpp"

void bot_msg(int fd, std::string nickname, std::string msg) {
  std::string se =
      ":BOT!BOT@" + SERVER_NAME + " PRIVMSG " + nickname + " :" + msg + "\r\n";
  sendString(se, fd);
}

void botHelp(int fd, std::string nickname) {
  std::ifstream data;
  char filepath[24] = "src/config/bot.config";

  data.open(filepath);

  if (!data) {
    bot_msg(fd, nickname, "BOT File is missing");
    return;
  } else {
    std::string line;
    while (getline(data, line)) {
      bot_msg(fd, nickname, line);
    }
  }
  data.close();
}

std::vector<std::string> splitString(std::string str) {
  std::stringstream ss(str);
  std::string token;
  std::vector<std::string> tokens;

  while (getline(ss, token, ' ')) {
    tokens.push_back(token);
  }
  return (tokens);
}

void recommand(int fd, std::string nickname) {
  srand(time(NULL));
  int random = rand() % 10;

  std::string se = "오늘 당신의 저녁은?!?! : ";
  switch (random + 1) {
    case 1:
      se += "치킨";
      break;
    case 2:
      se += "피자";
      break;
    case 3:
      se += "보쌈";
      break;
    case 4:
      se += "마라탕 && 꿔바로우";
      break;
    case 5:
      se += "국밥";
      break;
    case 6:
      se += "돈까스";
      break;
    case 7:
      se += "샤브샤브";
      break;
    case 8:
      se += "모모유부";
      break;
    case 9:
      se += "고추바사삭";
      break;
    case 10:
      se += "프랭크버거";
      break;
    default:
      se += "김치찌개";
      break;
  }
  bot_msg(fd, nickname, se);
}

void game(int fd, std::string nickname, int choice) {
  srand(time(NULL));
  int com = rand() % 3 + 1;

  std::string se;
  std::string choices[] = {"가위", "바위", "보"};

  bot_msg(
      fd, nickname,
      "Bot's choice: " + choices[com - 1] + " (" + intToString(com) + ")\r\n");

  if (choice == com) {
    bot_msg(fd, nickname, "비겼습니다");
  } else if ((choice == 1 && com == 3) || (choice == 2 && com == 1) ||
             (choice == 3 && com == 2)) {
    bot_msg(fd, nickname, "이겼습니다!! 축하합니다:)");
  } else {
    bot_msg(fd, nickname, "졌습니다... 다시 도전해보세요..!");
  }
}

void Server::bot(int fd, std::string nickname, std::string msg) {
  std::string command[4] = {"HI", "HELP", "REC", "GAME"};
  std::vector<std::string> tokens = splitString(msg);

  for (size_t i = 0; i < tokens[0].length(); i++) {
    tokens[0][i] = std::toupper(tokens[0][i]);
  }

  std::cout << "input : " << tokens[0] << std::endl;

  int index = 0;
  while (index < 4) {
    if (tokens[0] == command[index]) break;
    index++;
  }

  std::string se2;
  switch (index + 1) {
    case 1:
      se2 = ":" + SERVER_NAME +
            " [BOT] Hello I am a bot. Please move on to the next page\r\n";
      sendString(se2, fd);
      bot_msg(fd, nickname, "Hello I am a bot.");
      bot_msg(fd, nickname,
              "If you need help, enter HELP, REC for "
              "recommendations, and GAME for games.");
      break;
    case 2:
      botHelp(fd, nickname);
      break;
    case 3:
      recommand(fd, nickname);
      break;
    case 4:
      if (tokens.size() < 2) {
        bot_msg(fd, nickname, "Please enter in the form : GAME [choice]");
        bot_msg(fd, nickname, "가위(1), 바위(2), and 보(3)");
      } else
        game(fd, nickname, std::atoi(tokens[1].c_str()));
      break;
    default:
      bot_msg(fd, nickname,
              "Invalid input. Please enter 'HELP' for instructions on how to "
              "use it.");
  }
}

// hi/hello
// 사용 방법 소개, 원하는 숫자를 입력하면 저녁 메뉴를 추천

// help/HELP 모르는 명령어. help로 사용할 수 있는 명령어

// 저메추
// 가위바위보

// if (msg == )
// 숫자 입력
// 랜덤하게 메뉴 출력
// std::stringstream ss;
// ss << msg;

// std::string se;
// 저메추,

// }

// bot_msg(fd, nickname, se);
