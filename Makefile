CFLAGS := -Wall -Wextra -Wpedantic -ggdb
LIBS   := -lncurses

all: main

clean:
	rm main

.PHONY: all clean

main: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
