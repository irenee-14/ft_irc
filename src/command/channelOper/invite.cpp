#include <Server.hpp>
#include <algorithm>  // remove

// INVITE user #channel

// invite를 한 channel이 존재해야 함
// :irc.local 403 root #asdf :No such channel

// invite를 받은 user가 존재해야 함
// irc.local 401 root root_____ :No such nick

// invite를 한 user가 채널에 속해있어야 함
// :irc.local 442 root__ #hi :You're not on that channel!

// invite를 한 user가 operator여야 함
// :irc.local 482 root__ #hi :You must be a channel operator

// invite를 받은 user가 채널에 속해있으면 안됨
// :irc.local 443 root root_ #hi :is already on channel

void Server::invite(int fd, std::vector<std::string> tokens) {
  const std::string user = tokens[1];
  const std::string channel = tokens[2];
  const std::string channelNoHash = channel.substr(1, channel.size() - 1);

  const std::string nickname = clients[fd].getNick();
  const std::string username = clients[fd].getUser();
  const std::string servername = clients[fd].getServerName();

  // channel 인덱스
  int channel_idx = isChannel(channelNoHash);

  // invite한 channel이 존재하지 않으면 에러
  if (channel_idx < 0) {
    std::string se = ":" + SERVER_NAME + " 403 " + nickname + " " + channel +
                     " :No such channel\r\n";
    sendString(se, fd);
    return;
  }

  // invite 받은 user가 서버에 존재하지 않으면 에러
  int target_fd = isUser(user);
  if (target_fd < 0) {
    std::string se = ":" + SERVER_NAME + " 401 " + nickname + " " + user +
                     " :No such nick\r\n";
    sendString(se, fd);
    return;
  }

  // invite한 user가 채널에 속해있는지 않으면 에러
  if (channels[channel_idx].isUser(fd) < 0) {
    std::string se = ":" + SERVER_NAME + " 442 " + nickname + " " + channel +
                     " :You're not on that channel!\r\n";
    sendString(se, fd);
    return;
  }

  // invite한 user가 operator가 아니면 에러
  if (channels[channel_idx].isOperator(fd) < 0) {
    std::string se = ":" + SERVER_NAME + " 482 " + nickname + " " + channel +
                     " :You must be a channel operator\r\n";
    sendString(se, fd);
    return;
  }

  // invite 받은 user가 이미 채널에 속해있으면 에러
  if (channels[channel_idx].isUser(user) >= 0) {
    std::string se = ":" + SERVER_NAME + " 443 " + nickname + " " + user + " " +
                     channel + " :is already on channel\r\n";
    sendString(se, fd);
    return;
  }

  // 모든 조건 만족하면 invite 보내기
  {
    // invite list에 추가
    channels[channel_idx].addInvite(target_fd);
    // target에게 보내는 메시지
    std::string se = ":" + nickname + "!" + username + "@" + servername +
                     " INVITE " + user + " #" + channelNoHash + "\r\n";
    sendString(se, target_fd);
  }

  {
    // invite를 보내는 user에게 보내는 메시지
    std::string se2 = ":" + SERVER_NAME + " 341 " + nickname + " " + user +
                      " " + channel + "\r\n";
    sendString(se2, fd);
  }

  {
    // 채널에 속한 모든 user에게 보내는 메시지
    //   :irc.local NOTICE #hi :*** root invited root_ into the channel
    std::string se3 = ":" + nickname + "!" + username + "@" + servername +
                      " NOTICE " + channel + " :*** " + nickname + " invited " +
                      user + " into the channel\r\n";

    std::vector<int> users = channels[channel_idx].getUserFds();
    users.erase(std::remove(users.begin(), users.end(), fd), users.end());

    sendString(se3, users);
  }
}

// no such nick/channel이 window goto 1에 뜨는게 맞는가?
// 입력한 채널에 떠야하는 것인가?

// invite 했다는 메시지 채널 안에서는 보낸 사람 제외하고 뜨는게
// 맞는가?(메인화면에서는 뜸)
//  users.erase(std::remove(users.begin(), users.end(), fd), users.end());
