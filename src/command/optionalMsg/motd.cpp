#include <fstream>

#include "Server.hpp"

std::string convertTimeToString(time_t unixTime) {
  struct tm* timeInfo;
  char buffer[80];

  timeInfo = localtime(&unixTime);
  strftime(buffer, sizeof(buffer), "%H:%M:%S %b %d %Y", timeInfo);

  return (buffer);
}

void sendMotd(int fd, Client client, int port, int client_num) {
  const std::string nickname = client.getNick();
  const std::string username = client.getUser();
  const std::string servername = client.getServerName();

  {
    std::string se;
    se += ":" + SERVER_NAME + " 001 " + nickname +
          " :Welcome to the Internet Relay Network" + nickname + "!" +
          username + "@" + servername + "\r\n";
    se += ":" + SERVER_NAME + " 002 " + nickname + " :Your host is " +
          SERVER_NAME + " (localhost), running version FT_IRC-1\r\n";
    se += ":" + SERVER_NAME + " 003 " + nickname +
          " :This server was created " +
          convertTimeToString(client.getTimestamp()) + "\r\n";

    // "<servername> <version> <available user modes> <available channel modes>"
    se += ":" + SERVER_NAME + " 004 " + nickname + " " + servername +
          " FT_IRC-1 i :oklit\r\n";
    //"Try server <server name>, port <port number>"
    se += ":" + SERVER_NAME + " 005 " + nickname + " Try server " +
          SERVER_NAME + ", port " + intToString(port) + "\r\n";

    //  :There are <integer> users on <integer> servers
    se += ":" + SERVER_NAME + " 251 " + nickname + " :There are " +
          intToString(client_num) + " users on 1 servers\r\n";
    sendString(se, fd);
  }
  // -----------------------------------------------------------------------

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

    se += ":" + SERVER_NAME + " 375 " + nickname + " :- " + SERVER_NAME +
          " Message of the day - \r\n";
    while (getline(data, line)) {
      se += ":" + SERVER_NAME + " 372 " + nickname + " :- " + line + "\r\n";
    }
    se +=
        ":" + SERVER_NAME + " 376 " + nickname + " :End of /MOTD command.\r\n";
    sendString(se, fd);
  }
  data.close();
}
