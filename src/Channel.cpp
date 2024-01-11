#include "Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(const Channel& src) { *this = src; }

Channel::~Channel(void) {}

Channel& Channel::operator=(Channel const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}
