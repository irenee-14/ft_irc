#include <cstdlib>
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

void recommand(int fd, std::string nickname) {
  srand(time(NULL));
  int random = rand() % 15;

  std::string se = "오늘 당신의 저녁은?!?! : ";
  std::string food[15] = {
      "뿌링클",          "푸라닭",        "도미노피자",      "보쌈",
      "마라탕",          "주먹고기",      "치마오",          "모모유부",
      "정통집",          "광수육회",      "감탄계 숯불치킨", "이모네 전",
      "스파게티 스토리", "주박사 닭갈비", "뼈다귀 해장국"};

  bot_msg(fd, nickname, se + food[random]);
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
  std::vector<std::string> tokens = spaceSplit(msg);

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
        game(fd, nickname, stringToInt(tokens[1]));
      break;
    default:
      bot_msg(fd, nickname,
              "Invalid input. Please enter 'HELP' for instructions on how to "
              "use it.");
  }
}
