#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <vector>
class Channel {
 private:
  std::string _channel_name;
  std::vector<int> _users;
  int _mode[5];
  std::string _topic;
  std::vector<int> _operator;
  std::vector<int> _ban_list;
  std::string _key;
  std::vector<int> _invite_list;
  int _num_users;

  Channel(void);

 public:
  Channel(const Channel& src);
  Channel(std::string channel_name);
  Channel& operator=(Channel const& rhs);
  ~Channel(void);

  std::string getChannelName() const;
  std::vector<int> getUsers() const;
  int getMode(int index) const;
  std::string getTopic() const;
  std::vector<int> getOperators() const;
  std::vector<int> getBanList() const;
  std::string getKey() const;
  std::vector<int> getInviteList() const;
  int getNumUsers() const;

  void setChannelName(std::string channel_name);

  void setMode(int index, int mode);
  void setTopic(std::string topic);
  void setOperator(std::vector<int> op);
  void setBanList(std::vector<int> ban_list);
  void setKey(std::string key);
  void setInviteList(std::vector<int> invite_list);
  void setNumUsers(int num_users);

  void addUser(int user);
  void removeUser(int user);
  void addOperator(int user);
  void removeOperator(int user);
};

#endif
