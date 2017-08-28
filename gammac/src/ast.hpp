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

#include <memory>
#include <vector>

#include "token.hpp"

struct AST
{
  public:
    AST(const Token &token) : token(token) {}
    virtual ~AST() = default;

    std::string getText() const { return token.lexeme; }

    Token token;
};

struct Id : public AST
{
    Id(const Token &token) : AST(token) {}
};

struct TypeDecl : public AST
{
    TypeDecl(const Token &token,
             std::shared_ptr<Id> name) : AST(token), name(name) {}

    std::shared_ptr<Id> name;
};

struct StringLiteral : public AST
{
    StringLiteral(const Token& token) : AST(token) {}
};

struct EnumFieldDecl : public AST
{
    EnumFieldDecl(const Token &token) : AST(token) {}
    
    std::shared_ptr<StringLiteral> format;
};

struct EnumBody : public AST
{
    EnumBody() : AST(Token(Kind::EnumBody)) {}

    std::vector<std::shared_ptr<EnumFieldDecl>> fields;
};

struct TraitList : public AST
{
    TraitList() : AST(Token(Kind::TraitList)) {}

    std::vector<std::shared_ptr<Id>> traits;
};

struct EnumDecl : public TypeDecl
{
    EnumDecl(const Token &token,
             std::shared_ptr<Id> name,
             std::shared_ptr<TraitList> traitList,
             std::shared_ptr<EnumBody> body) : TypeDecl(token, name), traitList(traitList), body(body) {}

    std::shared_ptr<EnumBody> body;
    std::shared_ptr<TraitList> traitList;
};

struct TypeRef : public AST
{
    TypeRef(const Token &token) : AST(token) {}
};

struct Arg : public AST
{
    Arg(const Token &token, std::shared_ptr<TypeRef> type) : AST(token), type(type) {}

    std::shared_ptr<TypeRef> type;
};

struct UnionFieldDecl : public AST
{
    UnionFieldDecl(const Token &token) : AST(token) {}

    std::vector<std::shared_ptr<Arg>> args;
    std::shared_ptr<StringLiteral> format;
};

struct UnionBody : public AST
{
    UnionBody() : AST(Token(Kind::UnionBody)) {}

    std::vector<std::shared_ptr<UnionFieldDecl>> fields;
};

struct UnionDecl : public TypeDecl
{
    UnionDecl(const Token &token,
              std::shared_ptr<Id> name,
              std::shared_ptr<TraitList> traitList,
              std::shared_ptr<UnionBody> body) : TypeDecl(token, name), traitList(traitList), body(body) {}

    std::shared_ptr<UnionBody> body;
    std::shared_ptr<TraitList> traitList;
};

struct SourceFile : public AST
{
    SourceFile() : AST(Token(Kind::SourceFile)) {}

    std::vector<std::shared_ptr<TypeDecl>> typeDecls;
};
