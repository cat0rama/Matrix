.PHONY: all clean

CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++17
SRC_PATH=src

all: clean s21_matrix_oop.a

s21_matrix_oop.a:
	$(CC) $(CFLAGS) -c ${SRC_PATH}/*.cpp
	ar rc s21_matrix_oop.a *.o
	ranlib s21_matrix_oop.a

test: clean
	$(CC) $(CFLAGS) ${SRC_PATH}/*.cpp tests/tests.cpp -lgtest
	./a.out

clean:
	rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt gcov_main gcov_report

