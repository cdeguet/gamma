#ifndef src_cpp_model_gm__
#define src_cpp_model_gm__

#include <ostream>

enum class STLHeader {
  map, istream, ostream, string, 
};

std::ostream &operator<<(std::ostream &os, const STLHeader &obj);


#endif
