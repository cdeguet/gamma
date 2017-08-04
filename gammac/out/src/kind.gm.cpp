#include <map>
#include <string>

#include "src/kind.gm.hpp"

static const std::string kKindToStr[] = {
  "Eof", "Id", "Comma", "Colon", "String", "LParen", "RParen", "LBrack", "RBrack", "LBrace", "RBrace", "EnumDecl", "EnumBody", "SourceFile", "TraitList", "UnionDecl", "UnionBody", 
};

std::ostream &operator<<(std::ostream &os, const Kind &obj) {
  os << kKindToStr[static_cast<size_t>(obj)];
  return os;
}

