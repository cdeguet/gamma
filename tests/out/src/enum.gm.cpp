#include <map>
#include <string>

#include "src/enum.gm.hpp"

static const std::map<std::string, Owner> kStrToOwner {
  {"-1", Owner::OTHER},
  {"0", Owner::NONE},
  {"1", Owner::SELF},
};

std::istream &operator>>(std::istream &is, Owner &obj) {
  std::string str;
  is >> str;
  obj = kStrToOwner.at(str);
  return is;
}

static const std::string kOwnerToStr[] = {
  "-1", "0", "1", 
};

std::ostream &operator<<(std::ostream &os, const Owner &obj) {
  os << kOwnerToStr[static_cast<size_t>(obj)];
  return os;
}

