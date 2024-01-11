#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
class Channel {
 private:
  std::string channel_name;

  // mode
  // operator
  // users
  // topic

  Channel(const Channel& src);
  Channel& operator=(Channel const& rhs);

 public:
  Channel(void);
  ~Channel(void);
};

#endif
