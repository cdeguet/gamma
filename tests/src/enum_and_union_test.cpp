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
#include <sstream>

#include "catch.hpp"
#include "src/enum_and_union.gm.hpp"

TEST_CASE("Enum from istream", "[enum]")
{
    std::stringstream input("E");
    Direction dir;
    input >> dir;
    REQUIRE(dir == Direction::E);
}

TEST_CASE("Union to ostream", "[union]")
{
    Action a1 = Action::Move(Direction::N);
    Action a2 = Action::Shoot(Direction::E, 42);
    Action a3 = Action::Wait();
    std::stringstream output;
    output << a1 << ", " << a2 << ", " << a3;
    REQUIRE(output.str() == "MOVE N, SHOOT E 42, WAIT");
}

TEST_CASE("Union copy", "[union]")
{
    Action a1 = Action::Move(Direction::E);
    Action a2 = a1;
    std::stringstream output;
    output << a2;
    REQUIRE(output.str() == "MOVE E");
}

TEST_CASE("Union default constructor", "[union]")
{
    Action a;
    REQUIRE(a.type == Action::Undef);
    a = Action::Move(Direction::S);
    REQUIRE(a.type == Action::Move_t);
}

TEST_CASE("Union equality unit variant", "[union]")
{
    REQUIRE(Action::Wait() == Action::Wait());
    REQUIRE_FALSE(Action::Wait() == Action());
}

TEST_CASE("Union equality different variants", "[union]")
{
    REQUIRE_FALSE(Action::Wait() == Action());
    REQUIRE_FALSE(Action::Move(Direction::N) == Action::Wait());
    REQUIRE_FALSE(Action::Shoot(Direction::N, 42) == Action::Move(Direction::N));
}

TEST_CASE("Union equality same variant", "[union]")
{
    REQUIRE(Action::Move(Direction::N) == Action::Move(Direction::N));
    REQUIRE_FALSE(Action::Move(Direction::N) == Action::Move(Direction::E));
}

TEST_CASE("Union equality multiple arguments", "[union]")
{
    REQUIRE(Action::Shoot(Direction::N, 42) == Action::Shoot(Direction::N, 42));
    REQUIRE_FALSE(Action::Shoot(Direction::N, 42) == Action::Shoot(Direction::N, 12));
    REQUIRE_FALSE(Action::Shoot(Direction::N, 42) == Action::Shoot(Direction::E, 42));
    REQUIRE_FALSE(Action::Shoot(Direction::N, 42) == Action::Shoot(Direction::E, 12));
}
