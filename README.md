[![Build Status](https://travis-ci.org/cdeguet/gamma.svg?branch=master)](https://travis-ci.org/cdeguet/gamma)

# The Gamma programming language

Gamma is a modern language inspired by Rust, generating C++ code.

With Gamma, you get all the power of C++, without the boilerplate!

## Example

The following Gamma code (`sample.gm`):

```
enum Direction [In, Out] {
    N, E, S, W
}
```

is translated into the C++ code below:

Header file (`sample.gm.hpp`):

```
#ifndef sample_gm__
#define sample_gm__

#include <istream>
#include <ostream>

enum class Direction {
  N, E, S, W, 
};

std::istream &operator>>(std::istream &is, Direction &obj);

std::ostream &operator<<(std::ostream &os, const Direction &obj);

#endif
```

Source file (`sample.gm.cpp`):
```
#include <map>
#include <string>

#include "sample.gm.hpp"

static const std::map<std::string, Direction> kStrToDirection {
  {"N", Direction::N},
  {"E", Direction::E},
  {"S", Direction::S},
  {"W", Direction::W},
};

std::istream &operator>>(std::istream &is, Direction &obj) {
  std::string str;generates
  is >> str;
  obj = kStrToDirection.at(str);
  return is;
}

static const std::string kDirectionToStr[] = {
  "N", "E", "S", "W", 
};

std::ostream &operator<<(std::ostream &os, const Direction &obj) {
  os << kDirectionToStr[static_cast<size_t>(obj)];
  return os;
}
```

## Build from source

To build the `gammac` compiler, run:

```
cd gammac
make
```

The compiler binary is generated in `out/bin/gammac`.

Some parts of the Gamma compiler are written in Gamma itself; the corresponding generated files can be found in `out/src`.

## Run the tests

To build and run the test suite:

```
cd gammac
make test
```
