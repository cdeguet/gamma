#ifndef src_struct_gm__
#define src_struct_gm__

#include <ostream>

struct Unit {
  Unit() = default;
};

struct Coord {
  Coord() = default;
  Coord(int x, int y): x(x), y(y) {}
  int x;
  int y;
};

struct Player {
  Player() = default;
  Player(int life, int bombs): life(life), bombs(bombs) {}
  int life;
  int bombs;
  bool operator==(const Player &other) const;
};

std::ostream &operator<<(std::ostream &os, const Player &obj);


#endif
