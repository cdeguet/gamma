#ifndef src_kind_gm__
#define src_kind_gm__

#include <ostream>

enum class Kind {
  Eof, Id, Comma, Colon, String, LParen, RParen, LBrack, RBrack, LBrace, RBrace, EnumDecl, EnumBody, SourceFile, TraitList, UnionDecl, UnionBody, 
};

std::ostream &operator<<(std::ostream &os, const Kind &obj);


#endif
