#include "include/matrix.hpp"

#include <iostream>

using namespace s21;

int main() {

    S21Matrix a(3, 2);
    S21Matrix b(2, 3);
    a(1, 1) = 1.1;
    b(1, 1) = 2.2;

    std::cout << a << std::endl << std::endl;

    std::cout << b << std::endl << std::endl;

    try {
        a *= b;
        std::cout << a << std::endl;
    } catch (std::exception& err) {
        std::cout << err.what() << std::endl;
    }

}
