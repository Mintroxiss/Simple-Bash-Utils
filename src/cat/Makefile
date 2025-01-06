CFLAGS = -g -Wall -Werror -Wextra -std=c11

all: s21_cat

s21_cat : s21_cat.o
	gcc $(CFLAGS) s21_cat.o -o s21_cat

s21_cat.o : s21_cat.c
	gcc $(CFLAGS) -c s21_cat.c

clean :
	rm -rf *.o
	rm -rf s21_cat

rebuild : clean s21_cat
