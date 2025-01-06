CFLAGS = -g -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep : s21_grep.o
	gcc $(CFLAGS) s21_grep.o -o s21_grep

s21_grep.o : s21_grep.c
	gcc $(CFLAGS) -c s21_grep.c

clean :
	rm -rf *.o
	rm -rf s21_grep

rebuild : clean s21_grep
