CFLAGS = `sdl2-config --cflags` -O2 -Wall -Werror -Wextra -pedantic -ansi -Wfloat-equal
INCS = testheadersdl2.h
TARGET = testteletext
SOURCES =  $(TARGET).c teletextfuncs.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

testgraphics: testgraphics.c
	$(CC) testgraphics.c neillsdl2.c -o testgraphics $(CFLAGS) $(LIBS)

memchk: $(TARGET)
	$(CC) -g $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)
	$ valgrind --leak-check=yes ./testteletext

clean:
	rm -f $(TARGET) testgraphics
