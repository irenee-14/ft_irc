#include "Channel.hpp"

#include <iostream>

Channel::Channel(void) {}

Channel::Channel(std::string channel_name) : _channel_name(channel_name) {
  _mode[n] = 1;
  _mode[t] = 1;
  _mode[i] = 0;
  _mode[k] = 0;
  _mode[l] = 0;
  _topic = "";
  _key = "";
}

Channel::Channel(const Channel& src) { *this = src; }

Channel::~Channel(void) {}

Channel& Channel::operator=(Channel const& rhs) {
  if (this != &rhs) {
    this->_channel_name = rhs._channel_name;
    this->_user_fds = rhs._user_fds;
    this->_user_nicks = rhs._user_nicks;
    for (int i = 0; i < 5; i++) this->_mode[i] = rhs._mode[i];
    this->_topic = rhs._topic;
    this->_operator = rhs._operator;
    this->_ban_list = rhs._ban_list;
    this->_key = rhs._key;
    this->_invite_list = rhs._invite_list;
  }
  return (*this);
}

// ------------------------------------------------------------------------------
std::string Channel::getChannelName() const { return (this->_channel_name); }

std::vector<int> Channel::getUserFds() const { return (this->_user_fds); }

std::vector<std::string> Channel::getUserNicks() const {
  return (this->_user_nicks);
}

int Channel::getMode(int index) const {
  if (index < 0 || index > 4) return (-1);
  return (this->_mode[index]);
}

std::string Channel::getTopic() const {
  if (this->_topic.empty()) return ("");
  return (this->_topic);
}
std::vector<int> Channel::getOperators() const {
  if (this->_operator.empty()) return (std::vector<int>());
  return (this->_operator);
}
std::vector<int> Channel::getBanList() const {
  if (this->_ban_list.empty()) return (std::vector<int>());
  return (this->_ban_list);
}
std::string Channel::getKey() const {
  if (this->_key.empty()) return ("");
  return (this->_key);
}
std::vector<int> Channel::getInviteList() const {
  if (this->_invite_list.empty()) return (std::vector<int>());
  return (this->_invite_list);
}

// ------------------------------------------------------------------------------

void Channel::setChannelName(std::string channel_name) {
  this->_channel_name = channel_name;
}

void Channel::setMode(int index, int mode) {
  if (index < 0 || index > 4) return;
  this->_mode[index] = mode;
}
void Channel::setTopic(std::string topic) { this->_topic = topic; }
void Channel::setOperator(std::vector<int> op) { this->_operator = op; }
void Channel::setBanList(std::vector<int> ban_list) {
  this->_ban_list = ban_list;
}
void Channel::setKey(std::string key) { this->_key = key; }
void Channel::setInviteList(std::vector<int> invite_list) {
  this->_invite_list = invite_list;
}
// ------------------------------------------------------------------------------

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

// ------------------------------------------------------------------------------
void Channel::addOperator(int user) { this->_operator.push_back(user); }

void Channel::removeOperator(int user) {
  std::vector<int>::iterator it =
      std::find(this->_operator.begin(), this->_operator.end(), user);
  if (it != this->_operator.end()) this->_operator.erase(it);
}

// ------------------------------------------------------------------------------

std::string Channel::getModes(void) {
  std::string modes = "";
  if (this->_mode[i]) modes += "i";
  if (this->_mode[k]) modes += "k";
  if (this->_mode[l]) modes += "l";
  if (this->_mode[n]) modes += "n";
  if (this->_mode[t]) modes += "t";

  return (modes);
}

// ------------------------------------------------------------------------------

// isOperator
// 인자로 어느 채널인지 받아서 그 채널의 operator인지 확인
// operator이면 true, 아니면 false

int Channel::isOperator(int fd) {
  for (unsigned int i = 0; i < this->_operator.size(); i++) {
    if (this->_operator[i] == fd) return (i);
  }
  return (-1);
}

int Channel::isUser(int fd) {
  for (unsigned int i = 0; i < this->_user_fds.size(); i++) {
    if (this->_user_fds[i] == fd) return (i);
  }
  return (-1);
}

int Channel::isUser(std::string nickname) {
  for (unsigned int i = 0; i < this->_user_nicks.size(); i++) {
    if (this->_user_nicks[i] == nickname) return (i);
  }
  return (-1);
}
