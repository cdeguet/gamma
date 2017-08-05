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

#include <cassert>
#include <iostream>
#include <sstream>

#include "src/enum_and_union.gm.hpp"

int main(int argc, char **argv)
{
    std::stringstream input("E");
    Direction dir;
    input >> dir;
    Action a1 = Action::Move(Direction::N);
    Action a2 = a1;
    Action a3 = Action::Wait();
    std::stringstream output;
    output << a2 << ", " << a3;
    assert(output.str() == "MOVE N, WAIT");  
}
