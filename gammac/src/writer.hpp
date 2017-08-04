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

#include <fstream>
#include <sstream>
#include <string>

class StreamWriter
{
  public:
    virtual ~StreamWriter() = default;
    virtual void startFile() {}
    virtual std::ostream &stream() = 0;
};

class FileWriter : public StreamWriter
{
  public:
    FileWriter(const std::string &fileName);
    std::ostream &stream() override;

  private:
    std::fstream fstream;
};

class DebugWriter : public StreamWriter
{
  public:
    DebugWriter(const std::string &fileName);
    void startFile() override;
    std::ostream &stream() override;

  private:
    std::string fileName;
};
