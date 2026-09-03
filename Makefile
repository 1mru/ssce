# ssce - simple source code editor
# See LICENSE.txt for copyright and license details.

CXX = g++

CXXFLAGS = -DNDEBUG \
           -Wall \
           -Wextra \
           -O3 \
           -std=c++23 \
           -Iscintilla/include \
           -Ilexilla/include

LDFLAGS = -mwindows \
          -municode

LIBS = -Lscintilla/bin \
       -Llexilla/bin \
       -lscintilla \
       -llexilla \
       -limm32 \
       -lole32 \
       -loleaut32 \
       -luuid

SRC = $(wildcard src/*.cpp)
OUT = bin/ssce.exe

$(OUT): $(SRC)
	mkdir -p bin
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(OUT)

clean:
	rm -f $(OUT)
