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

#include "lexer.hpp"
#include "parser.hpp"

#include <iostream>
#include <sstream>

Parser::Parser(Lexer &lexer) : lexer(lexer)
{
    for (int i = 0; i < k; i++)
        consume();
}

std::shared_ptr<SourceFile> Parser::parseSourceFile()
{
    auto source = std::make_shared<SourceFile>();
    while (nextKind() != Kind::Eof)
    {
        auto decl = parseTypeDecl();
        source->typeDecls.push_back(decl);
    }
    return source;
}

std::shared_ptr<TypeDecl> Parser::parseTypeDecl()
{
    switch (nextKind())
    {
    case Kind::EnumDecl:
        return parseEnumDecl();
    case Kind::UnionDecl:
        return parseUnionDecl();
    default:
        std::stringstream error;
        error << "Expected type declaration, found " << nextToken();
        throw std::runtime_error(error.str());
    }
}

std::shared_ptr<EnumDecl> Parser::parseEnumDecl()
{
    Token tok = nextToken();
    match(Kind::EnumDecl);
    auto name = parseId();
    auto traitList = std::make_shared<TraitList>();
    if (nextKind() == Kind::LBrack)
    {
        match(Kind::LBrack);
        for (auto id : parseIdList())
        {
            traitList->traits.push_back(id);
        }
        match(Kind::RBrack);
    }
    match(Kind::LBrace);
    auto body = parseEnumBody();
    match(Kind::RBrace);
    return std::make_shared<EnumDecl>(tok, name, traitList, body);
}

std::shared_ptr<EnumBody> Parser::parseEnumBody()
{
    auto body = std::make_shared<EnumBody>();
    while (nextKind() == Kind::Id)
    {
        Token tok = nextToken();
        match(Kind::Id);
        auto field = std::make_shared<EnumFieldDecl>(tok);
        if (nextKind() == Kind::String)
        {
            Token stringTok = nextToken();
            match(Kind::String);
            field->format = std::make_shared<StringLiteral>(stringTok);
        }
        body->fields.push_back(field);
        if (nextKind() == Kind::Comma)
        {
            match(Kind::Comma);
        }
        else
        {
            break;
        }
    }
    return body;
}

std::shared_ptr<UnionDecl> Parser::parseUnionDecl()
{
    Token tok = nextToken();
    match(Kind::UnionDecl);
    auto name = parseId();
    auto traitList = std::make_shared<TraitList>();
    if (nextKind() == Kind::LBrack)
    {
        match(Kind::LBrack);
        for (auto id : parseIdList())
        {
            traitList->traits.push_back(id);
        }
        match(Kind::RBrack);
    }
    match(Kind::LBrace);
    auto body = parseUnionBody();
    match(Kind::RBrace);
    return std::make_shared<UnionDecl>(tok, name, traitList, body);
}

std::shared_ptr<UnionBody> Parser::parseUnionBody()
{
    auto body = std::make_shared<UnionBody>();
    while (nextKind() == Kind::Id)
    {
        auto field = parseUnionField();
        body->fields.push_back(field);
        if (nextKind() == Kind::Comma)
        {
            match(Kind::Comma);
        }
        else
        {
            break;
        }
    }
    return body;
}

std::shared_ptr<UnionFieldDecl> Parser::parseUnionField()
{
    Token fieldId = nextToken();
    match(Kind::Id);
    auto field = std::make_shared<UnionFieldDecl>(fieldId);
    if (nextKind() == Kind::LParen)
    {
        match(Kind::LParen);
        while (nextKind() == Kind::Id)
        {
            Token argId = nextToken();
            match(Kind::Id);
            match(Kind::Colon);
            Token typeId = nextToken();
            match(Kind::Id);
            auto typeRef = std::make_shared<TypeRef>(typeId);
            auto arg = std::make_shared<Arg>(argId, typeRef);
            field->args.push_back(arg);
            if (nextKind() == Kind::Comma)
            {
                match(Kind::Comma);
            }
            else
            {
                break;
            }
        }
        match(Kind::RParen);
    }
    if (nextKind() == Kind::String)
    {
        Token stringTok = nextToken();
        match(Kind::String);
        field->format = std::make_shared<StringLiteral>(stringTok);
    }
    return field;
}

std::shared_ptr<Id> Parser::parseId()
{
    Token tok = nextToken();
    match(Kind::Id);
    return std::make_shared<Id>(tok);
};

std::vector<std::shared_ptr<Id>> Parser::parseIdList()
{
    std::vector<std::shared_ptr<Id>> list;
    if (nextKind() == Kind::Id)
    {
        while (true)
        {
            Token tok = nextToken();
            match(Kind::Id);
            auto item = std::make_shared<Id>(tok);
            list.push_back(item);
            if (nextKind() == Kind::Comma)
            {
                match(Kind::Comma);
            }
            else
            {
                break;
            }
        }
    }
    return list;
};

void Parser::match(Kind kind)
{
    if (nextKind() != kind)
    {
        std::stringstream ss;
        ss << "Unexpected token: " << nextToken();
        throw std::runtime_error(ss.str());
    }
    consume();
}

void Parser::consume()
{
    buffer[pos] = lexer.nextToken();
    pos = (pos + 1) % k;
}

const Token &Parser::nextToken(int lookAhead) const
{
    return buffer[(pos + lookAhead - 1) % k];
}

Kind Parser::nextKind(int lookAhead) const
{
    return nextToken(lookAhead).kind;
}
