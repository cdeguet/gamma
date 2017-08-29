#include "src/struct.gm.hpp"

std::ostream &operator<<(std::ostream &os, const Player &obj) {
  os << "{ ";
  os << "life" << ": " << obj.life << ", ";
  os << "bombs" << ": " << obj.bombs;
  os << " }";
  return os;
}

