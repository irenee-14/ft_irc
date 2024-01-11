#include "Client.hpp"

Client::Client(void) {}

Client::Client(const Client& src) { *this = src; }

Client::~Client(void) {}

Client& Client::operator=(Client const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}
