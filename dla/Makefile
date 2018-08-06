CFLAGS = `sdl2-config --cflags` -O2 -Wall -Werror -Wextra -pedantic -ansi -Wfloat-equal
TARGET = dla
SOURCES =  $(TARGET).c
LIBS =  `sdl2-config --libs` -lm
CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	cc -o dla dla.c neillncurses.c `pkg-config --cflags --libs MagickWand`
clean:

	rm -f $(TARGET)
run: all
	./$(TARGET)
