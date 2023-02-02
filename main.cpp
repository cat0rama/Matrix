#include <iostream>
#include <string>

#include "matrix.hpp"

using namespace s21;

int main() {
    try {
    S21Matrix a;
    S21Matrix c;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a(i, j) = i + j;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            c(i, j) = i + j + i;
        }
    }

    S21Matrix b(c);

    b = c = S21Matrix(10, 10) = c * 2.23 = (a += b);

    a = c = S21Matrix(8, 8);

    std::cout << (a == c) << std::endl;

    std::cout << b << std::endl;

    std::cout << b * b << std::endl << std::endl;

    b(0, 0) = 1;
    b(0, 1) = 3;
    b(0, 2) = 1;

    b(1, 0) = 1;
    b(1, 1) = 3;
    b(1, 2) = 1;

    b(2, 0) = 1;
    b(2, 1) = 3;
    b(2, 2) = 1;

    std::cout << b << std::endl << std::endl;

    std::cout << b.Transpose() << std::endl;
    } catch(std::exception& err) {
        std::cout << err.what() << std::endl;
    }
}
