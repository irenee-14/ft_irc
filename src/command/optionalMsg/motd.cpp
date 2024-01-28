#include <fstream>

#include "Server.hpp"

void sendMotd(int fd, Client client) {
  const std::string nickname = client.getNick();
  const std::string servername = client.getServerName();
  {
    std::string se;
    se += ":" + SERVER_NAME + " 001 " + nickname + " :Welcome\r\n";
    se += ":" + SERVER_NAME + " 002 " + nickname + " :Your host is " +
          SERVER_NAME + " (localhost), running version 1.0\r\n";
    se += ":" + SERVER_NAME + " 003 " + nickname +
          " :This server was created " + intToString(client.getTimestamp()) +
          "\r\n";
    se +=
        ":" + SERVER_NAME + " 004 " + nickname + " " + servername + " 1.0\r\n";
    se += ":localhost 005 " + nickname +
          " jihylim, joyoo :are supported by this server\r\n";
    sendString(se, fd);
  }
  std::ifstream data;
  char filepath[24] = "src/config/motd.config";

  data.open(filepath);
  if (!data) {
    const std::string se =
        ":" + SERVER_NAME + " 422 " + nickname + " :MOTD File is missing\r\n";
    sendString(se, fd);
    return;
  } else {
    std::string line;
    std::string se = "";

    se += ":" + SERVER_NAME + " 375 " + nickname + " :- " + servername +
          " Message of the day - \r\n";
    while (getline(data, line)) {
      se += ":" + SERVER_NAME + " 372 " + nickname + " :" + line + "\r\n";
    }
    se +=
        ":" + SERVER_NAME + " 376 " + nickname + " :End of /MOTD command.\r\n";
    sendString(se, fd);
  }
  data.close();
}
