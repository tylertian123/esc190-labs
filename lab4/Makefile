CFLAGS = -Wall -g
LDLIBS = -lm
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
.DEFAULT_GOAL := run
.PHONY = clean

tournament: tournament.o lab4.o

run: tournament
	./tournament

valgrind: tournament
	valgrind $(VALGRIND_FLAGS) ./tournament

clean:
	rm *.o
	find . -type f -executable -exec rm {} \;

