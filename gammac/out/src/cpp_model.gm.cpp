#include <map>
#include <string>

#include "src/cpp_model.gm.hpp"

static const std::string kSTLHeaderToStr[] = {
  "map", "istream", "ostream", "string", 
};

std::ostream &operator<<(std::ostream &os, const STLHeader &obj) {
  os << kSTLHeaderToStr[static_cast<size_t>(obj)];
  return os;
}

