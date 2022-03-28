CFLAGS = -Wall -g
LDLIBS = -lm
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
.DEFAULT_GOAL := run
.PHONY = clean

lab5: extras.o lab5.o test5.o

run: lab5
	./lab5

valgrind: lab5
	valgrind $(VALGRIND_FLAGS) ./lab5

clean:
	rm *.o; find . -type f -executable -exec rm {} \;

