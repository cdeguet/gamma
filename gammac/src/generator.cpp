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

#include <stdexcept>

#include "generator.hpp"
#include "writer.hpp"

std::string normalizeName(const std::string &name)
{
    std::string result;
    for (char c : name)
    {
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
        {
            c = '_';
        }
        result += c;
    }
    return result;
}

std::string getEnumFieldFormat(const EnumFieldDecl& field)
{
    return field.format ? field.format->getText() : field.getText();
}

void CppFile::addBlock(const std::string &block)
{
    blocks.push_back(block);
}

void CppFile::addInclude(STLHeader header)
{
    includes.insert(header);
}

void CppFile::setIncludeGuard(const std::string &name)
{
    includeGuard = name;
}

void CppFile::emit()
{
    writer.startFile();
    std::ostream &os = writer.stream();
    if (!includeGuard.empty())
    {
        os << "#ifndef " + includeGuard + "__\n";
        os << "#define " + includeGuard + "__\n\n";
    }
    for (STLHeader include : includes)
    {
        os << "#include <" << include << ">\n";
    }
    if (!includes.empty())
    {
        os << std::endl;
    }
    for (const auto block : blocks)
    {
        os << block;
    }
    if (!includeGuard.empty())
    {
        os << "\n#endif\n";
    }
}

CppGenerator::CppGenerator(const std::string &fileName,
                           StreamWriter &sourceWriter,
                           StreamWriter &headerWriter) : fileName(fileName), source(sourceWriter), header(headerWriter)
{
}

void CppGenerator::gen(const AST &node)
{
    switch (node.token.kind)
    {
    case Kind::SourceFile:
        gen(static_cast<const SourceFile &>(node));
        break;
    case Kind::EnumDecl:
        gen(static_cast<const EnumDecl &>(node));
        break;
    case Kind::UnionDecl:
        gen(static_cast<const UnionDecl &>(node));
        break;
    default:
        break;
    }
}

void CppGenerator::gen(const SourceFile &node)
{
    source.addBlock("#include \"" + fileName + ".hpp\"\n\n");
    for (auto typeDecl : node.typeDecls)
    {
        gen(*typeDecl);
    }
    source.emit();
    header.setIncludeGuard(normalizeName(fileName));
    header.emit();
}

void CppGenerator::gen(const EnumDecl &node)
{
    std::string block;
    block += "enum class ";
    block += node.name->getText();
    block += " {\n  ";
    for (auto field : node.body->fields)
    {
        block += field->getText();
        block += ", ";
    }
    block += "\n};\n\n";
    header.addBlock(block);
    for (auto traitId : node.traitList->traits)
    {
        auto traitName = traitId->getText();
        if (traitName == "In")
        {
            genEnumInTrait(node);
        }
        else if (traitName == "Out")
        {
            genEnumOutTrait(node);
        }
        else
        {
            throw std::runtime_error("Invalid trait " + traitName);
        }
    }
}

void CppGenerator::genEnumInTrait(const EnumDecl &node)
{
    auto enumName = node.name->getText();
    header.addInclude(STLHeader::istream);
    header.addBlock("std::istream &operator>>(std::istream &is, " + enumName + " &obj);\n\n");
    source.addInclude(STLHeader::map);
    source.addInclude(STLHeader::string);
    std::string block;
    auto strToEnum = "kStrTo" + enumName;
    block += "static const std::map<std::string, " + enumName + "> " + strToEnum + " {\n";
    for (auto field : node.body->fields)
    {
        block += "  {\"" + getEnumFieldFormat(*field) + "\", " + enumName + "::" + field->getText() + "},\n";
    }
    block += "};\n\n";
    block += "std::istream &operator>>(std::istream &is, " + enumName + " &obj) {\n";
    block += "  std::string str;\n";
    block += "  is >> str;\n";
    block += "  obj = " + strToEnum + ".at(str);\n";
    block += "  return is;\n";
    block += "}\n\n";
    source.addBlock(block);
}

void CppGenerator::genEnumOutTrait(const EnumDecl &node)
{
    auto enumName = node.name->getText();
    header.addInclude(STLHeader::ostream);
    header.addBlock("std::ostream &operator<<(std::ostream &os, const " + enumName + " &obj);\n\n");
    source.addInclude(STLHeader::map);
    source.addInclude(STLHeader::string);
    std::string block;
    auto enumToStr = "k" + enumName + "ToStr";
    block += "static const std::string " + enumToStr + "[] = {\n  ";
    for (auto field : node.body->fields)
    {
        block += "\"" + getEnumFieldFormat(*field) + "\", ";
    }
    block += "\n};\n\n";
    block += "std::ostream &operator<<(std::ostream &os, const " + enumName + " &obj) {\n";
    block += "  os << " + enumToStr + "[static_cast<size_t>(obj)];\n";
    block += "  return os;\n";
    block += "}\n\n";
    source.addBlock(block);
}

void CppGenerator::gen(const UnionDecl &node)
{
    genUnionDecl(node);
    for (auto traitId : node.traitList->traits)
    {
        auto traitName = traitId->getText();
        if (traitName == "Out")
        {
            genUnionOutTrait(node);
        }
        else
        {
            throw std::runtime_error("Invalid trait " + traitName);
        }
    }
}

void CppGenerator::genUnionDecl(const UnionDecl &node)
{
    std::string unionName = node.name->getText();
    std::string block;
    block += "struct " + unionName + " {\n";
    block += "  enum Type {\n";
    block += "    Undef,\n";
    for (auto field : node.body->fields)
    {
        block += "    " + field->getText() + "_t,\n";
    }
    block += "  } type;\n";
    for (auto field : node.body->fields)
    {
        if (!field->args.empty())
        {
            block += "  struct " + field->getText() + "_d {\n";
            for (auto arg : field->args)
            {
                block += "    " + arg->type->getText() + " " + arg->getText() + ";\n";
            }
            block += "  };\n";
        }
    }
    block += "  union {\n";
    for (auto field : node.body->fields)
    {
        if (!field->args.empty())
        {
            std::string fieldName = field->getText();
            block += "    " + fieldName + "_d " + fieldName + ";\n";
        }
    }
    block += "  } data;\n";
    block += "  " + unionName + "(Type type = Undef): type(type) {}\n";
    for (auto field : node.body->fields)
    {
        std::string fieldName = field->getText();
        block += "  static " + unionName + " " + fieldName + "(";
        int argCount = field->args.size();
        for (auto arg : field->args)
        {
            block += arg->type->getText() + " " + arg->getText();
            if (--argCount > 0)
            {
                block += ", ";
            }
        }
        block += ") {\n";
        block += "    " + unionName + " obj(" + fieldName + "_t);\n";
        for (auto arg : field->args)
        {
            block += "    obj.data." + fieldName + "." + arg->getText() + " = " + arg->getText() + ";\n";
        }
        block += "    return obj;\n";
        block += "  }\n";
    }
    block += "};\n\n";
    header.addBlock(block);
}

void CppGenerator::genUnionOutTrait(const UnionDecl &node)
{
    auto unionName = node.name->getText();
    header.addInclude(STLHeader::ostream);
    header.addBlock("std::ostream &operator<<(std::ostream &os, const " + unionName + " &obj);\n\n");
    std::string block;
    block += "std::ostream &operator<<(std::ostream &os, const " + unionName + " &obj) {\n";
    block += "  switch (obj.type) {\n";
    for (auto field : node.body->fields)
    {
        block += "  case " + unionName + "::" + field->getText() + "_t:\n";
        block += "    os << " + expandFormat(*field, field->format->getText()) + ";\n";
        block += "  break;\n";
    }
    block += "  }\n";
    block += "  return os;\n";
    block += "}\n\n";
    source.addBlock(block);
}

std::string CppGenerator::expandFormat(const UnionFieldDecl& scope, const std::string& format)
{
    std::string out = "\"";
    for (char c: format)
    {
        switch (c)
        {
        case '{':
            out += "\" << ";
            out += "obj.data." + scope.getText() + ".";
        break;
        case '}':
            out += " << \"";
        break;
        default:
            out += c;
        }
    }
    out += "\"";
    return out;
}

