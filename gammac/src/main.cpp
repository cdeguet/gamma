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

#include <fstream>
#include <iostream>

#include "generator.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "writer.hpp"

void dumpFile(const std::string &fileName)
{
    std::cout << "//<< [" + fileName + "]" << std::endl;
    std::fstream inStream;
    inStream.open(fileName);
    while (!inStream.eof())
    {
        std::string line;
        std::getline(inStream, line);
        std::cout << line << std::endl;
    }
}

void compileFile(const std::string &fileName, bool debug)
{
    try
    {
        std::fstream inStream;
        inStream.open(fileName);
        Lexer lexer(inStream);
        Parser parser(lexer);
        auto ast = parser.parseSourceFile();
        if (debug)
        {
            dumpFile(fileName);
            DebugWriter sourceWriter(fileName + ".cpp");
            DebugWriter headerWriter(fileName + ".hpp");
            CppGenerator generator(fileName, sourceWriter, headerWriter);
            generator.gen(*ast);
        }
        else
        {
            std::string outDir = "./out";
            FileWriter sourceWriter(outDir + "/" + fileName + ".cpp");
            FileWriter headerWriter(outDir + "/" + fileName + ".hpp");
            CppGenerator generator(fileName, sourceWriter, headerWriter);
            generator.gen(*ast);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
}

int main(int argc, char **argv)
{
    switch (argc)
    {
    case 2:
        compileFile(argv[1], false);
        break;
    case 3:
        compileFile(argv[2], true);
        break;
    default:
        std::cout << "Usage: gammac <fileName> [-d]" << std::endl;
    }
}
