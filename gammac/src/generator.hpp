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

#include <set>
#include <string>
#include <vector>

#include "ast.hpp"
#include "src/cpp_model.gm.hpp"

class StreamWriter;

class CppFile
{
public:
  CppFile(StreamWriter &writer) : writer(writer) {}
  void addBlock(const std::string &block);
  void addInclude(STLHeader header);
  void setIncludeGuard(const std::string& name);
  void emit();

private:
  StreamWriter &writer;
  std::string includeGuard;
  std::set<STLHeader> includes;
  std::vector<std::string> blocks;
};

class CppGenerator
{
public:
  CppGenerator(const std::string &fileName, StreamWriter &sourceWriter, StreamWriter &headerWriter);
  void gen(const SourceFile &node);

private:
  void gen(const AST &node);
  void gen(const EnumDecl &node);
  void genEnumInTrait(const EnumDecl &node);
  void genEnumOutTrait(const EnumDecl &node);
  void gen(const StructDecl &node);
  void genStructDecl(const StructDecl &node);
  void genStructOutTrait(const StructDecl &node);
  void gen(const UnionDecl &node);
  void genUnionDecl(const UnionDecl &node);
  void genUnionOutTrait(const UnionDecl &node);
  std::string expandFormat(const UnionFieldDecl& scope, const std::string& format);

  std::string fileName;
  CppFile source;
  CppFile header;
};
