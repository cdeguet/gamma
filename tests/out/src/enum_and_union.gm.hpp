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
    Move_t, 
    Shoot_t, 
    Wait_t, 
  } type;
  struct Move_d {
    Direction dir;
  };
  struct Shoot_d {
    int strength;
  };
  union {
    Move_d Move;
    Shoot_d Shoot;
  } data;
  Action(Type type): type(type) {}
  static Action Move(Direction dir) {
    Action obj(Move_t);
    obj.data.Move.dir = dir;
    return obj;
  }
  static Action Shoot(int strength) {
    Action obj(Shoot_t);
    obj.data.Shoot.strength = strength;
    return obj;
  }
  static Action Wait() {
    Action obj(Wait_t);
    return obj;
  }
};

std::ostream &operator<<(std::ostream &os, const Action &obj);


#endif
