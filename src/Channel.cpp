#include "Channel.hpp"

#include <algorithm>  // find
#include <iostream>

#include "Utils.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string channel_name)
    : _channel_name(channel_name),
      _timestamp(time(0)),
      _key(""),
      _limit(-1),
      _isTopicMode(true),
      _topic(""),
      _isInviteOnlyMode(false) {
  _mode[t] = 1;
  _mode[i] = 0;
  _mode[k] = 0;
  _mode[l] = 0;
}

Channel::Channel(const Channel& src) { *this = src; }

Channel::~Channel(void) {}

Channel& Channel::operator=(Channel const& rhs) {
  if (this != &rhs) {
    this->_channel_name = rhs._channel_name;
    this->_user_fds = rhs._user_fds;
    this->_user_nicks = rhs._user_nicks;
    this->_timestamp = rhs._timestamp;
    this->_operator = rhs._operator;

    for (int i = 0; i < 4; i++) this->_mode[i] = rhs._mode[i];
    this->_key = rhs._key;
    this->_limit = rhs._limit;

    this->_isTopicMode = rhs._isTopicMode;
    this->_topic = rhs._topic;

    this->_isInviteOnlyMode = rhs._isInviteOnlyMode;
    this->_invite_list = rhs._invite_list;
  }
  return (*this);
}

// ================================== get ======================================
std::string Channel::getChannelName() const { return (this->_channel_name); }

std::vector<int> Channel::getUserFds() const { return (this->_user_fds); }

std::vector<std::string> Channel::getUserNicks() const {
  return (this->_user_nicks);
}

std::vector<int> Channel::getOperators() const {
  if (this->_operator.empty()) return (std::vector<int>());
  return (this->_operator);
}

time_t Channel::getTimestamp(void) const { return (this->_timestamp); }

// ----------------------------------------------------------------------------

std::string Channel::getModes(int fd) const {
  std::string modes = "";
  if (this->_mode[i]) modes += "i";
  if (this->_mode[k]) modes += "k";
  if (this->_mode[l]) modes += "l";
  if (this->_mode[t]) modes += "t";
  // 채널에 속해있는 사람에게만 key 보임
  if (this->_key != "") {
    if (this->isUser(fd) != -1)
      modes += " " + this->_key;
    else
      modes += " <key>";
  }
  if (this->_limit != -1) modes += " " + intToString(this->_limit);

  return (modes);
}

// 현재 채널 모드 정보 출력
// :irc.local 324 root #hi +lnt :3
// :irc.local 329 root #hi :1706258904
void Channel::printCurMode(int fd, std::string server_name,
                           std::string nickname) {
  std::string channel_name = this->getChannelName();

  std::string se = ":" + server_name + " 324 " + nickname + " " + channel_name +
                   " +" + this->getModes(fd) + "\r\n";
  sendString(se, fd);

  std::string se2 = ":" + server_name + " 329 " + nickname + " " +
                    channel_name + " " + intToString(this->getTimestamp()) +
                    "\r\n";
  sendString(se2, fd);
}

// ----------------------------------------------------------------------------

std::string Channel::getKey() const {
  if (this->_key.empty()) return ("");
  return (this->_key);
}

int Channel::getLimit() const { return (this->_limit); }

// ------------------------------------------------------------------------------

bool Channel::getTopicMode() const { return (this->_isTopicMode); }

std::string Channel::getTopic() const {
  if (this->_topic.empty()) return ("");
  return (this->_topic);
}

// ----------------------------------------------------------------------------

bool Channel::getInviteOnlyMode() const { return (this->_isInviteOnlyMode); }

std::vector<int> Channel::getInviteList() const {
  if (this->_invite_list.empty()) return (std::vector<int>());
  return (this->_invite_list);
}

// =============================================================================
// ================================== set ======================================

void Channel::setChannelName(std::string channel_name) {
  this->_channel_name = channel_name;
}

void Channel::setOperator(std::vector<int> op) { this->_operator = op; }

// ------------------------------------------------------------------------------
void Channel::setMode(char mode, int isAddMode) {
  if (mode == 'i')
    this->_mode[i] = isAddMode;
  else if (mode == 'k')
    this->_mode[k] = isAddMode;
  else if (mode == 'l')
    this->_mode[l] = isAddMode;
  else if (mode == 't')
    this->_mode[t] = isAddMode;
  else
    return;
}

// ------------------------------------------------------------------------------

void Channel::setKey(std::string key) { this->_key = key; }
void Channel::setLimit(int limit) { this->_limit = limit; }

// ------------------------------------------------------------------------------

void Channel::setTopicMode(bool isTopicMode) {
  this->_isTopicMode = isTopicMode;
}
void Channel::setTopic(std::string topic) { this->_topic = topic; }

// ------------------------------------------------------------------------------

void Channel::setInviteOnlyMode(bool isInviteOnlyMode) {
  this->_isInviteOnlyMode = isInviteOnlyMode;
}

void Channel::setInviteList(std::vector<int> invite_list) {
  this->_invite_list = invite_list;
}

// ================================= User =====================================

void Channel::addUser(int user_fd, std::string user_nick) {
  this->_user_fds.push_back(user_fd);
  this->_user_nicks.push_back(user_nick);
}

void Channel::removeUser(int find) {
  for (unsigned int i = 0; i < this->_user_fds.size(); i++) {
    if (this->_user_fds[i] == find) {
      this->_user_fds.erase(this->_user_fds.begin() + i);
      this->_user_nicks.erase(this->_user_nicks.begin() + i);
      break;
    }
  }
}

void Channel::removeUser(std::string find) {
  for (unsigned int i = 0; i < this->_user_nicks.size(); i++) {
    if (this->_user_nicks[i] == find) {
      this->_user_fds.erase(this->_user_fds.begin() + i);
      this->_user_nicks.erase(this->_user_nicks.begin() + i);
      break;
    }
  }
}

// ================================= is user ===================================

int Channel::isUser(int fd) const {
  for (unsigned int i = 0; i < this->_user_fds.size(); i++) {
    if (this->_user_fds[i] == fd) return (i);
  }
  return (-1);
}

int Channel::isUser(std::string nickname) const {
  for (unsigned int i = 0; i < this->_user_nicks.size(); i++) {
    if (this->_user_nicks[i] == nickname) return (i);
  }
  return (-1);
}

// =============================== Operator =================================
void Channel::addOperator(int user) { this->_operator.push_back(user); }

void Channel::removeOperator(int user) {
  std::vector<int>::iterator it =
      std::find(this->_operator.begin(), this->_operator.end(), user);
  if (it != this->_operator.end()) this->_operator.erase(it);
}

// isOperator
// 인자로 어느 채널인지 받아서 그 채널의 operator인지 확인
// operator이면 true, 아니면 false

int Channel::isOperator(int fd) const {
  for (unsigned int i = 0; i < this->_operator.size(); i++) {
    if (this->_operator[i] == fd) return (i);
  }
  return (-1);
}

// =============================== invite ====================================

void Channel::addInvite(int user) {
  if (this->isInvite(user) == -1) this->_invite_list.push_back(user);
}

void Channel::removeInvite(int user) {
  std::vector<int>::iterator it =
      std::find(this->_invite_list.begin(), this->_invite_list.end(), user);
  if (it != this->_invite_list.end()) this->_invite_list.erase(it);
}

int Channel::isInvite(int fd) const {
  for (unsigned int i = 0; i < this->_invite_list.size(); i++) {
    if (this->_invite_list[i] == fd) return (i);
  }
  return (-1);
}
