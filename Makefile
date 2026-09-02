# ssce - simple source code editor
# See LICENSE.txt for copyright and license details.

FLG = -Wall -Wextra -O3 -std=c++23 -mwindows -municode
SRC = ssce.cpp
HIT = ssce

all: $(HIT)

$(HIT): $(SRC)
		$(CXX) $(SRC) -o $(HIT) $(FLG)

clean:
	rm -f $(HIT)

.PHONY: $(HIT) clean
