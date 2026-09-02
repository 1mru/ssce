CXX = g++
FLG = -Wall -Wextra -O3 -std=c++23 -mwindows -municode
SRC = src/*.cpp
HIT = bin/ssce.exe

all: test

$(HIT): $(SRC)
		$(CXX) $(SRC) -o $(HIT) $(FLG)

test: $(HIT)
		$(HIT)

clean:
	rm -f $(HIT)

.PHONY: $(HIT) test clean
