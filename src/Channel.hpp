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
  // -------------------------------------------------------------
  int _mode[5];

  std::string _topic;
  std::string _key;
  int _limit;
  // -------------------------------------------------------------
  std::vector<int> _operator;
  std::vector<int> _invite_list;
  // -------------------------------------------------------------

  Channel(void);

  // 127.000.000.001.04242-127.000.000.001.41124: :root!root@127.0.0.1 MODE #hi
  // +k :asdf 127.000.000.001.04242-127.000.000.001.41124: :root!root@127.0.0.1
  // MODE #hi +kl asdf :3

 public:
  Channel(const Channel& src);
  Channel(std::string channel_name);
  Channel& operator=(Channel const& rhs);
  ~Channel(void);

  // ----------------------------------------------------

  std::string getChannelName() const;
  std::vector<int> getUserFds() const;
  std::vector<std::string> getUserNicks() const;

  int getMode(int index) const;
  std::string getModes(void);

  std::string getTopic() const;
  std::string getKey() const;
  int getLimit() const;

  std::vector<int> getOperators() const;
  std::vector<int> getInviteList() const;

  // ----------------------------------------------------

  void setChannelName(std::string channel_name);

  void setMode(int index, int mode);

  void setTopic(std::string topic);
  void setKey(std::string key);
  void setLimit(int limit);

  void setOperator(std::vector<int> op);
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

  // ----------------------------------------------------
  void addOperator(int user);
  void removeOperator(int user);

  // ----------------------------------------------------
  int isUser(int fd);
  int isUser(std::string nickname);
  int isOperator(int fd);
};

#endif
