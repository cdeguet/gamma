#
# Copyright (C) 2017 Cyril Deguet <cyril.deguet@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

CPP_SRCS = $(wildcard src/*.cpp)
GM_SRCS = $(wildcard src/*.gm)
GEN_SRCS = $(GM_SRCS:%.gm=out/%.gm.cpp)
CPP_OBJS = $(CPP_SRCS:src/%.cpp=out/obj/%.o)
GEN_OBJS = $(GEN_SRCS:out/src/%.cpp=out/obj/%.o)
ALL_INCS = $(wildcard src/*.hpp) $(wildcard out/src/*.hpp)

CPPFLAGS = -Iout -Isrc

all: $(CPP_OBJS) $(GEN_OBJS)
	@mkdir -p out/bin
	g++ $(CPP_OBJS) $(GEN_OBJS) -o out/bin/gammac

clean:
	rm -rf out/obj/* out/bin/*

test:
	cd tests && $(MAKE) clean test

out/src/%.gm.cpp: src/%.gm
	if [ -f out/bin/gammac ]; then out/bin/gammac $< ; fi

out/obj/%.o: src/%.cpp $(ALL_INCS)
	@mkdir -p out/obj
	g++ -c $(CPPFLAGS) -std=c++11 -o $@ $<

out/obj/%.o: out/src/%.cpp $(ALL_INCS)
	@mkdir -p out/obj
	g++ -c $(CPPFLAGS) -std=c++11 -o $@ $<
