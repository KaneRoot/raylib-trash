CC=zigcc

PKGS=raylib
CFLAGS=-Wall -Wextra -ggdb -pedantic -std=c11 `pkg-config --cflags --static $(PKGS)`
LIBS=`pkg-config --libs --static $(PKGS)`

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LIBS)
