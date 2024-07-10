CFLAGS := -Wall -Wextra -Wpedantic -ggdb `pkg-config ncursesw --cflags`
LIBS   := `pkg-config ncursesw --libs`

all: main

clean:
	rm main

.PHONY: all clean

main: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
