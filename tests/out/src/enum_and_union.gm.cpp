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

static const std::map<Direction, std::string> kDirectionToStr {
  {Direction::N, "N"},
  {Direction::E, "E"},
  {Direction::S, "S"},
  {Direction::W, "W"},

};

std::ostream &operator<<(std::ostream &os, const Direction &obj) {
  os << kDirectionToStr.at(obj);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Action &obj) {
  switch (obj.type) {
  case Action::Move_t:
    os << "MOVE " << obj.data.Move.dir << "";
  break;
  case Action::Shoot_t:
    os << "SHOOT " << obj.data.Shoot.strength << "";
  break;
  case Action::Wait_t:
    os << "WAIT";
  break;
  }
  return os;
}

