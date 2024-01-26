#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <vector>

// mode enum
enum { i = 0, k, l, t };

class Channel {
 private:
  std::string _channel_name;
  std::vector<int> _user_fds;
  std::vector<std::string> _user_nicks;
  time_t _timestamp;

  std::vector<int> _operator;
  // -------------------------------------------------------------

  int _mode[5];

  std::string _key;
  int _limit;

  bool _isTopicMode;
  std::string _topic;

  bool _isInviteOnlyMode;
  std::vector<int> _invite_list;

  // -------------------------------------------------------------

  Channel(void);

 public:
  Channel(const Channel& src);
  Channel(std::string channel_name);
  Channel& operator=(Channel const& rhs);
  ~Channel(void);

  // ----------------------------------------------------

  std::string getChannelName() const;
  std::vector<int> getUserFds() const;
  std::vector<std::string> getUserNicks() const;
  time_t getTimestamp(void) const;

  std::vector<int> getOperators() const;

  // ----------------------------------------------------

  std::string getModes(int fd) const;

  std::string getKey() const;
  int getLimit() const;

  bool getTopicMode() const;
  std::string getTopic() const;

  bool getInviteOnlyMode() const;
  std::vector<int> getInviteList() const;

  // ----------------------------------------------------

  void setChannelName(std::string channel_name);
  void setOperator(std::vector<int> op);

  void setMode(char mode, int isAddMode);

  void setKey(std::string key);
  void setLimit(int limit);

  void setTopicMode(bool isTopicMode);
  void setTopic(std::string topic);

  void setInviteOnlyMode(bool isInviteOnlyMode);
  void setInviteList(std::vector<int> invite_list);

  // ----------------------------------------------------
  void addUser(int user_fd, std::string user_nick);
  void removeUser(int find);
  void removeUser(std::string find);
  // fd로 찾는 경우 T : 찾을 fd, V : getUserFds
  // nick으로 찾는 경우 T ; 찾을 nickname, V : getUserNicks
  //  template <typename T, typename V>
  //  void removeUser(T find, V type) {
  //    for (unsigned int i = 0; i < type.size(); i++) {
  //      if (type[i] == find) {
  //        this->_user_fds.erase(this->_user_fds.begin() + i);
  //        this->_user_nicks.erase(this->_user_nicks.begin() + i);
  //        break;
  //      }
  //    }
  //  }

  int isUser(int fd) const;
  int isUser(std::string nickname) const;
  // ----------------------------------------------------

  void addOperator(int user);
  void removeOperator(int user);
  int isOperator(int fd) const;

  // ----------------------------------------------------

  void addInvite(int user);
  void removeInvite(int user);
  int isInvite(int fd) const;
};

#endif
