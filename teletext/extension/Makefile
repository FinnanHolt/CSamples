CFLAGS = `sdl2-config --cflags` -O2 -Wall -Werror -Wextra -pedantic -ansi -Wfloat-equal
TARGET = image
SOURCES =  $(TARGET).c
LIBS =  `sdl2-config --libs` -lm
CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	cc -o image image.c `pkg-config --cflags --libs MagickWand`
clean:

	rm -f $(TARGET)

run: all
	./$(TARGET)
