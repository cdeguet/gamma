#ifndef src_enum_gm__
#define src_enum_gm__

#include <istream>
#include <ostream>

enum class Owner {
  OTHER, NONE, SELF, 
};

std::istream &operator>>(std::istream &is, Owner &obj);

std::ostream &operator<<(std::ostream &os, const Owner &obj);


#endif
