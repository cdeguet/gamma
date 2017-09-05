#include <map>
#include <string>

#include "src/enum_and_union.gm.hpp"

static const std::map<std::string, Direction> kStrToDirection {
  {"N", Direction::N},
  {"E", Direction::E},
  {"S", Direction::S},
  {"W", Direction::W},
};

std::istream &operator>>(std::istream &is, Direction &obj) {
  std::string str;
  is >> str;
  obj = kStrToDirection.at(str);
  return is;
}

static const std::string kDirectionToStr[] = {
  "N", "E", "S", "W", 
};

std::ostream &operator<<(std::ostream &os, const Direction &obj) {
  os << kDirectionToStr[static_cast<size_t>(obj)];
  return os;
}

bool Action::operator==(const Action &other) const {
  if (type != other.type) return false;
  switch (type) {
  case Action::Move_t:
    return data.Move.dir == other.data.Move.dir;
  break;
  case Action::Shoot_t:
    return data.Shoot.dir == other.data.Shoot.dir
      && data.Shoot.strength == other.data.Shoot.strength;
  break;
  default:
    return true;
  }
}

std::ostream &operator<<(std::ostream &os, const Action &obj) {
  switch (obj.type) {
  case Action::Move_t:
    os << "MOVE " << obj.data.Move.dir << "";
  break;
  case Action::Shoot_t:
    os << "SHOOT " << obj.data.Shoot.dir << " " << obj.data.Shoot.strength << "";
  break;
  case Action::Wait_t:
    os << "WAIT";
  break;
  default:
    break;  }
  return os;
}

