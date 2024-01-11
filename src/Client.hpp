#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

class Client {
 private:
  Client(const Client& src);
  Client& operator=(Client const& rhs);

 public:
  Client(void);
  ~Client(void);
};

#endif
