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

#include <array>

#include "ast.hpp"

class Lexer;

// LL(2) recursive descent parser
class Parser
{
  public:
    Parser(Lexer &lexer);
    std::shared_ptr<SourceFile> parseSourceFile();
    std::shared_ptr<TypeDecl> parseTypeDecl();
    std::shared_ptr<EnumDecl> parseEnumDecl();
    std::shared_ptr<EnumBody> parseEnumBody();
    std::shared_ptr<UnionDecl> parseUnionDecl();
    std::shared_ptr<UnionBody> parseUnionBody();
    std::shared_ptr<UnionFieldDecl> parseUnionField();
    std::shared_ptr<Id> parseId();
    std::vector<std::shared_ptr<Id>> parseIdList();

  private:
    void match(Kind kind);
    void consume();
    const Token& nextToken(int lookAhead = 1) const;
    Kind nextKind(int lookAhead = 1) const;

    Lexer &lexer;
    static const int k = 2;
    std::array<Token, k> buffer;
    int pos = 0;
};
