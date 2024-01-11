#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

class Channel {
 private:
  Channel(const Channel& src);
  Channel& operator=(Channel const& rhs);

 public:
  Channel(void);
  ~Channel(void);
};

#endif
