CXX = g++
FLG = -Wall -Wextra -O3 -std=c++23 -mwindows -municode
SRC = ssce.cpp edit.cpp
HIT = ssce

all: $(HIT)

$(HIT): $(SRC)
		$(CXX) $(SRC) -o $(HIT) $(FLG)

clean:
	rm -f $(HIT)

.PHONY: $(HIT) clean
