.PHONY: all clean

CC=g++
CFLAGS=-Wall -Wextra -Werror

all: clean s21_matrix_oop.a

s21_matrix_oop.a:
	$(CC) $(CFLAGS) -c *.cpp
	ar rc s21_matrix_oop.a *.o
	ranlib s21_matrix_oop.a
tests: clean
	$(CC) $(CFLAGS) matrix.cpp tests/tests.cpp -lgtest
	./a.out

clean:
	rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt gcov_main gcov_report

