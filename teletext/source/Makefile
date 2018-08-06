CFLAGS = `sdl2-config --cflags` -O2 -Wall -Werror -Wextra -pedantic -ansi -Wfloat-equal
INCS = headersdl2.h
TARGET = teletext
SOURCES =  $(TARGET).c neillsdl2.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

memchk: $(TARGET)

	$(CC) -g $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)
		$ valgrind --leak-check=yes ./teletext
clean:
	rm -f $(TARGET)
