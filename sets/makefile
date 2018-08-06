CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
TARGETS = week9p2 week9p3

all: $(TARGETS)

week9p2: testarr.c arr.c set.c arr.h set.h
	$(CC) -O3 -D ARRINTS testarr.c arr.c set.c -o $@ $(CFLAGS)

week9p3: janeausten.c arr.c set.c arr.h set.h
	$(CC) -O3 -D ARRSTRINGS arr.c set.c janeausten.c -o $@ $(CFLAGS)

clean:
	rm -fr $(TARGETS)

run: all
	./testarr
	./week9p2
	./week9p3

.PHONY: clean run all
