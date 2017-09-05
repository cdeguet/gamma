#ifndef src_enum_and_union_gm__
#define src_enum_and_union_gm__

#include <istream>
#include <ostream>

enum class Direction {
  N, E, S, W, 
};

std::istream &operator>>(std::istream &is, Direction &obj);

std::ostream &operator<<(std::ostream &os, const Direction &obj);

struct Action {
  enum Type {
    Undef,
    Move_t,
    Shoot_t,
    Wait_t,
  } type;
  struct Move_d {
    Direction dir;
  };
  struct Shoot_d {
    Direction dir;
    int strength;
  };
  union {
    Move_d Move;
    Shoot_d Shoot;
  } data;
  Action(Type type = Undef): type(type) {}
  static Action Move(Direction dir) {
    Action obj(Move_t);
    obj.data.Move.dir = dir;
    return obj;
  }
  static Action Shoot(Direction dir, int strength) {
    Action obj(Shoot_t);
    obj.data.Shoot.dir = dir;
    obj.data.Shoot.strength = strength;
    return obj;
  }
  static Action Wait() {
    Action obj(Wait_t);
    return obj;
  }
  bool operator==(const Action &other) const;
};

std::ostream &operator<<(std::ostream &os, const Action &obj);


#endif
