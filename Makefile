# ssce - simple source code editor
# See LICENSE.txt for copyright and license details.

CXX = g++
WINDRES = windres

FLG = -Wall -Wextra -O3 -std=c++23 -mwindows -municode
SRC = ssce.cpp
RES = ssce.rc
OBJ = ssce_res.o
HIT = ssce.exe

all: $(HIT)

$(HIT): $(SRC) $(OBJ)
		$(CXX) $(SRC) $(OBJ) -o $(HIT) $(FLG)

$(OBJ): $(RES)
		$(WINDRES) $(RES) -o $(OBJ)

clean:
		rm -f $(HIT) $(OBJ)

.PHONY: all clean
