#include <algorithm>  // find

#include "Server.hpp"
#include "Utils.hpp"

// join 시 채널에 있는 user들의 nick을 string으로 반환
const std::string Server::userList(const Channel& channel) {
  std::string se = "";

  std::vector<int> users = channel.getUserFds();
  std::vector<int> ops = channel.getOperators();

  printArg("\nusers : ", users.size());
  printArg("operators : ", ops.size());

  for (unsigned int i = 0; i < users.size(); ++i) {
    std::vector<int>::iterator it = std::find(ops.begin(), ops.end(), users[i]);

    if (it != ops.end()) se += "@";
    se += clients[users[i]].getNick();

    if (i != users.size() - 1) se += " ";
  }
  return (se);
}

// -------------------------------------------------------------

void Server::join(int fd, std::vector<std::string> tokens) {
  const std::vector<std::string> channel_vec = commaSplit(tokens[1], 0);
  const std::vector<std::string> password_vec =
      tokens.size() > 2 ? commaSplit(tokens[2], 1) : std::vector<std::string>();

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  for (unsigned int i = 0; i < channel_vec.size(); ++i) {
    const std::string channel = channel_vec[i];
    const std::string password = password_vec.size() > i ? password_vec[i] : "";
    const std::string channelNoHash = channel.substr(1, channel.size() - 1);

    int channel_idx = isChannel(channelNoHash);
    if (channel_idx >= 0) {
      // invited 확인
      if (channels[channel_idx].isInvite(fd) < 0) {
        // password mode 확인
        if (channels[channel_idx].getKeyMode() &&
            channels[channel_idx].getKey() != password) {
          const std::string se =
              ":" + SERVER_NAME + " 475 " + nickname + " " + channel +
              " :Cannot join channel (incorrect channel key)\r\n";
          sendString(se, fd);
          continue;
        };
        // invite mode 확인
        if (channels[channel_idx].getInviteOnlyMode()) {
          const std::string se = ":" + SERVER_NAME + " 473 " + nickname + " " +
                                 channel +
                                 " :Cannot join channel (invite only)\r\n";
          sendString(se, fd);
          continue;
        };
        // limit 확인
        if (channels[channel_idx].getLimit() > 0 &&
            channels[channel_idx].getUserFds().size() >=
                (unsigned int)channels[channel_idx].getLimit()) {
          const std::string se = ":" + SERVER_NAME + " 471 " + nickname + " " +
                                 channel +
                                 " :Cannot join channel (channel is full)\r\n";
          sendString(se, fd);
          continue;
        };
      } else
        channels[channel_idx].removeInvite(fd);
      channels[channel_idx].addUser(fd, nickname);
    } else {
      channel_idx = channels.size();
      // 채널 없으면 새로 만들기
      channels.push_back(Channel(channelNoHash));
      channels.back().addUser(fd, nickname);
      channels.back().addOperator(fd);
    }

    {
      // 채널에 속한 모든 user에게 join 메시지 보내기
      // :root_!root@127.0.0.1 JOIN :#hi
      std::string se = ":" + nickname + "!" + username + "@" + servername +
                       " JOIN :" + channel + "\r\n";
      sendString(se, channels[channel_idx].getUserFds());
    }

    // 채널 입장시 보내는 메시지
    {
      // topic 있을 때
      // :irc.local 332 root_ #hi :asdf
      // :irc.local 333 root_ #hi root!root@127.0.0.1 :1706264746
      std::string topic = channels[channel_idx].getTopic();
      if (topic != "") {
        std::string se = ":" + SERVER_NAME + " 332 " + nickname + " " +
                         channel + " :" + topic + "\r\n";
        sendString(se, fd);
      }
    }

    {
      // 채널에 들어온 user에게 채널 정보 보내기
      // :irc.local 353 root_ = #hi :@root root_
      // :irc.local 366 root_ #hi :End of /NAMES list.
      std::string se2 = ":" + SERVER_NAME + " 353 " + nickname + " = " +
                        channel + " :" + userList(channels[channel_idx]) +
                        "\r\n";

      se2 += ":" + SERVER_NAME + " 366 " + nickname + " " + channel +
             " :End of /NAMES list.\r\n";
      sendString(se2, fd);
    }
  }
}
