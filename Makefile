ifdef WIN
CC=x86_64-w64-mingw32-gcc
LFLAGS=-g -Wall -Wextra -lmingw32
SDLFLAGS=-lSDL2main -lSDL2 -I vendor/SDL2/x86_64-w64-mingw32/include/ -L vendor/SDL2/x86_64-w64-mingw32/lib
else
CC=gcc
LFLAGS=-g -Wall -Wextra
SDLFLAGS=-lSDL2main -lSDL2
endif

EXEC=sokoban.exe
SRC=main.c pc_renderer.c level.c pc_input.c lvllut.c
DEP=renderer.h level.h input.h

$(EXEC): $(SRC) $(DEP)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LFLAGS) $(SDLFLAGS) -D PCVER=1

.PHONY: clean
clean:
	rm $(EXEC)
