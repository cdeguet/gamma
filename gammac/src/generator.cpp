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

std::string getEnumFieldFormat(const EnumFieldDecl &field)
{
    return field.format ? field.format->getText() : field.getText();
}

CppBlock::CppBlock(const std::string &text) : text(text)
{
}

CppBlock &CppBlock::addBlock(const std::string &text)
{
    children.emplace_back(text);
    return children.back();
}

CppBlock &CppBlock::operator+=(const std::string &string)
{
    text += string;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const CppBlock &block)
{
    os << block.text;
    for (const auto &child : block.children)
    {
        os << child;
    }
    return os;
}

CppBlock &CppFile::addBlock(const std::string &text)
{
    CppBlock &child = block.addBlock(text);
    return child;
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
    os << block;
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
    case Kind::StructDecl:
        gen(static_cast<const StructDecl &>(node));
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

void CppGenerator::gen(const StructDecl &node)
{
    CppBlock &structBody = genStructBody(node);
    for (auto traitId : node.traitList->traits)
    {
        auto traitName = traitId->getText();
        if (traitName == "Eq")
        {
            genStructEqTrait(node, structBody);
        }
        else if (traitName == "Out")
        {
            genStructOutTrait(node);
        }
        else
        {
            throw std::runtime_error("Invalid trait " + traitName);
        }
    }
}

CppBlock &CppGenerator::genStructBody(const StructDecl &node)
{
    std::string structName = node.name->getText();
    CppBlock &block = header.addBlock();
    block.addBlock("struct " + structName + " {\n");
    CppBlock &structBody = block.addBlock();
    structBody += "  " + structName + "() = default;\n";
    const auto &fields = node.body->fields;
    if (!fields.empty())
    {
        structBody += "  " + structName + "(";
        int fieldCount = fields.size();
        for (auto field : fields)
        {
            structBody += field->type->getText() + " " + field->getText();
            if (--fieldCount > 0)
            {
                structBody += ", ";
            }
        }
        structBody += "): ";
        fieldCount = fields.size();
        for (auto field : fields)
        {
            structBody += field->getText() + "(" + field->getText() + ")";
            if (--fieldCount > 0)
            {
                structBody += ", ";
            }
        }
        structBody += " {}\n";
    }
    for (auto field : fields)
    {
        structBody += "  " + field->type->getText() + " " + field->getText() + ";\n";
    }
    block.addBlock("};\n\n");
    return structBody;
}

void CppGenerator::genStructEqTrait(const StructDecl &node, CppBlock &structBody)
{
    auto structName = node.name->getText();
    structBody += "  bool operator==(const " + structName + " &other) const;\n";
    std::string block;
    block += "bool " + structName + "::operator==(const " + structName + " &other) const {\n";
    block += "  return ";
    int fieldCount = node.body->fields.size();
    for (auto field : node.body->fields)
    {
        std::string fieldName = field->getText();
        block += fieldName + " == other." + fieldName;
        if (--fieldCount > 0)
        {
            block += "\n      && ";
        }
    }
    block += ";\n";
    block += "}\n\n";
    source.addBlock(block);
}

void CppGenerator::genStructOutTrait(const StructDecl &node)
{
    auto structName = node.name->getText();
    header.addInclude(STLHeader::ostream);
    header.addBlock("std::ostream &operator<<(std::ostream &os, const " + structName + " &obj);\n\n");
    std::string block;
    block += "std::ostream &operator<<(std::ostream &os, const " + structName + " &obj) {\n";
    block += "  os << \"{ \";\n";
    int fieldCount = node.body->fields.size();
    for (auto field : node.body->fields)
    {
        block += "  os << \"" + field->getText() + "\" << \": \" << obj." + field->getText();
        if (--fieldCount > 0)
        {
            block += " << \", \"";
        }
        block += ";\n";
    }
    block += "  os << \" }\";\n";
    block += "  return os;\n";
    block += "}\n\n";
    source.addBlock(block);
}

void CppGenerator::gen(const UnionDecl &node)
{
    CppBlock &unionBody = genUnionBody(node);
    for (auto traitId : node.traitList->traits)
    {
        auto traitName = traitId->getText();
        if (traitName == "Eq")
        {
            genUnionEqTrait(node, unionBody);
        }
        else if (traitName == "Out")
        {
            genUnionOutTrait(node);
        }
        else
        {
            throw std::runtime_error("Invalid trait " + traitName);
        }
    }
}

CppBlock &CppGenerator::genUnionBody(const UnionDecl &node)
{
    std::string unionName = node.name->getText();
    CppBlock &block = header.addBlock();
    block.addBlock("struct " + unionName + " {\n");
    CppBlock &unionBody = block.addBlock();
    unionBody += "  enum Type {\n";
    unionBody += "    Undef,\n";
    for (auto field : node.body->fields)
    {
        unionBody += "    " + field->getText() + "_t,\n";
    }
    unionBody += "  } type;\n";
    for (auto field : node.body->fields)
    {
        if (!field->args.empty())
        {
            unionBody += "  struct " + field->getText() + "_d {\n";
            for (auto arg : field->args)
            {
                unionBody += "    " + arg->type->getText() + " " + arg->getText() + ";\n";
            }
            unionBody += "  };\n";
        }
    }
    unionBody += "  union {\n";
    for (auto field : node.body->fields)
    {
        if (!field->args.empty())
        {
            std::string fieldName = field->getText();
            unionBody += "    " + fieldName + "_d " + fieldName + ";\n";
        }
    }
    unionBody += "  } data;\n";
    unionBody += "  " + unionName + "(Type type = Undef): type(type) {}\n";
    for (auto field : node.body->fields)
    {
        std::string fieldName = field->getText();
        unionBody += "  static " + unionName + " " + fieldName + "(";
        int argCount = field->args.size();
        for (auto arg : field->args)
        {
            unionBody += arg->type->getText() + " " + arg->getText();
            if (--argCount > 0)
            {
                unionBody += ", ";
            }
        }
        unionBody += ") {\n";
        unionBody += "    " + unionName + " obj(" + fieldName + "_t);\n";
        for (auto arg : field->args)
        {
            unionBody += "    obj.data." + fieldName + "." + arg->getText() + " = " + arg->getText() + ";\n";
        }
        unionBody += "    return obj;\n";
        unionBody += "  }\n";
    }
    block.addBlock("};\n\n");
    return unionBody;
}

void CppGenerator::genUnionEqTrait(const UnionDecl &node, CppBlock &unionBody)
{
    auto unionName = node.name->getText();
    unionBody += "  bool operator==(const " + unionName + " &other) const;\n";
    std::string block;
    block += "bool " + unionName + "::operator==(const " + unionName + " &other) const {\n";
    block += "  if (type != other.type) return false;\n";
    block += "  switch (type) {\n";
    for (auto field : node.body->fields)
    {
        if (!field->args.empty())
        {
            std::string fieldName = field->getText();
            block += "  case " + unionName + "::" + field->getText() + "_t:\n";
            block += "    return ";
            int argCount = field->args.size();
            for (auto arg : field->args)
            {
                std::string argName = fieldName + "." + arg->getText();
                block += "data." + argName + " == other.data." + argName;
                if (--argCount > 0)
                {
                    block += "\n      && ";
                }
            }
            block += ";\n";
            block += "  break;\n";
        }
    }
    block += "  default:\n";
    block += "    return true;\n";
    block += "  }\n";
    block += "}\n\n";
    source.addBlock(block);
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
    block += "  default:\n";
    block += "    break;";
    block += "  }\n";
    block += "  return os;\n";
    block += "}\n\n";
    source.addBlock(block);
}

std::string CppGenerator::expandFormat(const UnionFieldDecl &scope, const std::string &format)
{
    std::string out = "\"";
    for (char c : format)
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
