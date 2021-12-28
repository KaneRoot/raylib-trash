CC=zigcc

SRC ?= main

PKGS=raylib
CFLAGS=-Wall -Wextra -ggdb -pedantic -std=c11 `pkg-config --cflags --static $(PKGS)`
LIBS=`pkg-config --libs --static $(PKGS)`

$(SRC): $(SRC).c
	$(CC) $(CFLAGS) -o $(SRC) $(SRC).c $(LIBS)
