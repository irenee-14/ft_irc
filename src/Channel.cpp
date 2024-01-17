#include "Channel.hpp"

#include <iostream>

Channel::Channel(void) {}

Channel::Channel(std::string channel_name) : _channel_name(channel_name) {}

Channel::Channel(const Channel& src) { *this = src; }

Channel::~Channel(void) {}

Channel& Channel::operator=(Channel const& rhs) {
  if (this != &rhs) {
    this->_channel_name = rhs._channel_name;
  }
  return (*this);
}

std::string Channel::getChannelName() const { return (this->_channel_name); }
std::vector<int> Channel::getUsers() const { return (this->_users); }
int Channel::getMode(int index) const {
  if (index < 0 || index > 4) return (-1);
  return (this->_mode[index]);
}
std::string Channel::getTopic() const {
  if (this->_topic.empty()) return ("");
  return (this->_topic);
}
std::vector<int> Channel::getOperator() const {
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
int Channel::getNumUsers() const {
  if (this->_num_users < 0) return (-1);
  return (this->_num_users);
}

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
void Channel::setNumUsers(int num_users) { this->_num_users = num_users; }

void Channel::addUser(int user) { this->_users.push_back(user); }

void Channel::removeUser(int user) {
  std::vector<int>::iterator it =
      std::find(this->_users.begin(), this->_users.end(), user);
  if (it != this->_users.end()) this->_users.erase(it);
}