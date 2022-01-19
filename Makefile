CFLAGS = -Wall -g
LDLIBS = -lm
.DEFAULT_GOAL := run2

test1: lab1.o test1.o

run1: test1
	./test1

lab2_task1: lab2_task1.o
lab2_task2: lab2_task2.o
lab2_task3: lab2_task3.o
lab2_task4: lab2_task4.o
lab2_task5: lab2_task5.o
lab2_task6: lab2_task6.o
lab2_task7: lab2_task7.o lab2_test7.o

run2: lab2_task1 lab2_task2 lab2_task3 lab2_task4 lab2_task5 lab2_task6 lab2_task7
	find . -type f -executable -name "lab2_task*" -exec {} \;

clean:
	rm *.o
	find . -type f -executable -exec rm {} \;

