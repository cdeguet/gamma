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

#include <istream>
#include <map>
#include <string>

#include "token.hpp"

class Lexer
{
public:
  Lexer(std::istream &is);
  Token nextToken(int n = 1);

private:
  void consume();
  bool isSpace() const;
  bool isLetter() const;
  Token getName(const Pos& startPos);
  Token getString(const Pos& startPos);
  void skipComment();

  std::istream &is;
  int peek = EOF;
  Pos curPos;
  Pos nextPos;
  static const std::map<std::string, Kind> WORDS;
};
