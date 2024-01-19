#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <vector>

// mode enum
enum { i = 0, k, l, n, t };

class Channel {
 private:
  std::string _channel_name;
  std::vector<int> _user_fds;
  std::vector<std::string> _user_nicks;
  // -------------------------------------------------------------
  int _mode[5];
  std::string _topic;
  std::string _key;
  // -------------------------------------------------------------
  std::vector<int> _operator;
  std::vector<int> _ban_list;
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

  int getMode(int index) const;
  std::string getModes(void);

  std::string getTopic() const;
  std::string getKey() const;

  std::vector<int> getOperators() const;
  std::vector<int> getBanList() const;
  std::vector<int> getInviteList() const;

  // ----------------------------------------------------

  void setChannelName(std::string channel_name);

  void setMode(int index, int mode);

  void setTopic(std::string topic);
  void setKey(std::string key);

  void setOperator(std::vector<int> op);
  void setBanList(std::vector<int> ban_list);
  void setInviteList(std::vector<int> invite_list);

  // ----------------------------------------------------
  void addUser(int user_fd, std::string user_nick);

  template <typename T, typename V>
  void removeUser(T find, V type) {
    for (unsigned int i = 0; i < type.size(); i++) {
      if (type[i] == find) {
        this->_user_fds.erase(this->_user_fds.begin() + i);
        this->_user_nicks.erase(this->_user_nicks.begin() + i);
        break;
      }
    }
  }

  // ----------------------------------------------------
  void addOperator(int user);
  void removeOperator(int user);
  bool isOperator(int fd);
};

#endif
