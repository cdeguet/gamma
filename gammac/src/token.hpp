/*
 * Copyright (C) 2017 Cyril Deguet <cyril.deguet@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <ostream>
#include <string>

#include "src/kind.gm.hpp"

struct Pos
{
  int line = 0;
  int column = 0;
};

struct Token
{
  Token() = default;
  Token(Kind kind, const Pos &start = Pos(), const std::string &lexeme = "") : kind(kind), start(start), lexeme(lexeme) {}

  Kind kind;
  Pos start;
  std::string lexeme;
};

std::ostream &operator<<(std::ostream &os, const Token &token);