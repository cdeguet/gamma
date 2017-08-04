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

#include <iostream>
#include <stdexcept>

#include "lexer.hpp"

const std::map<std::string, Kind> Lexer::WORDS = {
    {"enum", Kind::EnumDecl},
    {"union", Kind::UnionDecl}};

Lexer::Lexer(std::istream &is) : is(is)
{
    nextPos.line = 1;
    nextPos.column = 1;
    consume();
}

Token Lexer::nextToken(int n)
{
    while (true)
    {
        if (isSpace())
        {
            consume();
        } 
        else if (peek == '#')
        {
            skipComment();
        }
        else {
            break;
        }
    }
    Pos startPos = curPos;
    while (peek != EOF)
    {
        switch (peek)
        {
        case ',':
            consume();
            return Token(Kind::Comma, startPos, ",");
        case ':':
            consume();
            return Token(Kind::Colon, startPos, ":");
        case '(':
            consume();
            return Token(Kind::LParen, startPos, "(");
        case ')':
            consume();
            return Token(Kind::RParen, startPos, ")");
        case '[':
            consume();
            return Token(Kind::LBrack, startPos, "[");
        case ']':
            consume();
            return Token(Kind::RBrack, startPos, "]");
        case '{':
            consume();
            return Token(Kind::LBrace, startPos, "{");
        case '}':
            consume();
            return Token(Kind::RBrace, startPos, "}");
        case '"':
            return getString(startPos);
        default:
            if (isLetter())
            {
                return getName(startPos);
            }
            else
            {
                std::string error = "Invalid character ";
                error += (char)peek;
                throw std::runtime_error(error);
            }
        }
    }
    return Token(Kind::Eof, startPos, "<EOF>");
}

Token Lexer::getName(const Pos &startPos)
{
    std::string buf;
    do
    {
        buf += peek;
        consume();
    } while (isLetter());
    auto word = WORDS.find(buf);
    if (word != WORDS.end())
    {
        return Token(word->second, startPos, buf);
    }
    return Token(Kind::Id, startPos, buf);
}

Token Lexer::getString(const Pos &startPos)
{
    consume();
    std::string buf;
    while (peek != '\"')
    {
        buf += peek;
        consume();
    };
    consume();
    return Token(Kind::String, startPos, buf);
}

void Lexer::consume()
{
    peek = is.get();
    curPos = nextPos;
    if (peek == '\n')
    {
        nextPos.line++;
        nextPos.column = 1;
    }
    else
    {
        nextPos.column++;
    }
}

void Lexer::skipComment()
{
    do {
        consume();
    }
    while (peek != '\n');
}

bool Lexer::isSpace() const
{
    return peek == ' ' || peek == '\t' || peek == '\r' || peek == '\n';
}

bool Lexer::isLetter() const
{
    return (peek >= 'a' && peek <= 'z') || (peek >= 'A' && peek <= 'Z');
}
