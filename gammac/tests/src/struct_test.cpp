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

#include <sstream>
#include "catch.hpp"
#include "src/struct.gm.hpp"

TEST_CASE("Struct default constructor", "[struct]")
{
    Coord coord;
    coord.x = 12;
    coord.y = 42;
    REQUIRE(coord.x == 12);
    REQUIRE(coord.y == 42);
}

TEST_CASE("Struct full constructor", "[struct]")
{
    Coord coord(12, 42);
    REQUIRE(coord.x == 12);
    REQUIRE(coord.y == 42);
}

TEST_CASE("Struct copy constructor", "[struct]")
{
    Coord coord1(12, 42);
    Coord coord2 = coord1;
    REQUIRE(coord2.x == 12);
    REQUIRE(coord2.y == 42);
}

TEST_CASE("Struct to ostream", "[struct]")
{
    Player player(10, 5);
    std::stringstream out;
    out << player;
    REQUIRE(out.str() == "{ life: 10, bombs: 5 }");
}

TEST_CASE("Struct equality", "[struct]")
{
    REQUIRE(Player(10, 5) == Player(10, 5));
    REQUIRE_FALSE(Player(10, 5) == Player(10, 0));
    REQUIRE_FALSE(Player(10, 5) == Player(0, 5));
    REQUIRE_FALSE(Player(10, 5) == Player(0, 0));
}

TEST_CASE("Empty struct", "[struct]")
{
    Unit unit;
}
