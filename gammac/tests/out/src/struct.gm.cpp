#include "src/struct.gm.hpp"

bool Player::operator==(const Player &other) const {
  return life == other.life
      && bombs == other.bombs;
}

std::ostream &operator<<(std::ostream &os, const Player &obj) {
  os << "{ ";
  os << "life" << ": " << obj.life << ", ";
  os << "bombs" << ": " << obj.bombs;
  os << " }";
  return os;
}

