CC=x86_64-w64-mingw32-gcc
SRC=main.c pc_renderer.c level.c pc_input.c lvllut.c
DEP=renderer.h level.h input.h
LFLAGS=-g -Wall -Wextra -lmingw32 -lSDL2main -lSDL2
SDLFLAGS=-I vendor/SDL2/x86_64-w64-mingw32/include/ -L vendor/SDL2/x86_64-w64-mingw32/lib
EXEC=sokoban

$(EXEC): $(SRC) $(DEP)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LFLAGS) $(SDLFLAGS) -D PCVER=1

.PHONY: clean
clean:
	rm $(EXEC)

.PHONY: install
install:
	cp $(EXEC).exe /mnt/e/DOS_C/skb/
