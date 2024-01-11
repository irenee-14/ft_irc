#include <iostream>

#include "Server.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage : " << argv[0] << " <port> <password>" << std::endl;
    return (0);
  }

  try {
    Server serv(argv);
    serv.acceptLoop();
  } catch (std::string exception) {
    std::cout << "Error : " << exception << std::endl;
  }
  return (0);
}
