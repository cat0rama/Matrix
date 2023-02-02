#include "matrix.hpp"

#include <iostream>
#include <math.h>

namespace s21 {
    S21Matrix::S21Matrix(): m_rows(default_rows), m_cols(default_cols) {
        m_matrix = new double[m_rows * m_cols]();
    }

    S21Matrix::S21Matrix(std::int32_t t_rows, std::int32_t t_cols): m_rows(t_rows), m_cols(t_cols) {
        if (t_rows <= 0 || t_cols <= 0) {
            throw std::length_error("size are less then 0\n");
        }
        m_matrix = new double[m_rows * m_cols]();
    }

    S21Matrix::S21Matrix(const S21Matrix& t_matrix) {
        Initialize(t_matrix);
    }

    S21Matrix::S21Matrix(S21Matrix&& t_matrix) noexcept {
        Initialize(std::move(t_matrix));
    }

     S21Matrix::~S21Matrix() {
        if (m_matrix) {
            delete[] m_matrix;
        }
    }

     S21Matrix& S21Matrix::operator=(const S21Matrix& t_matrix) {
         if (this != &t_matrix) {
             if (Size() == t_matrix.m_rows * t_matrix.m_cols) {
                 std::copy_n(t_matrix.m_matrix, Size(), m_matrix);
             } else {
                 delete[] m_matrix;
                 Initialize(t_matrix);
             }
         }

         return *this;
     }

     S21Matrix& S21Matrix::operator=(S21Matrix&& t_matrix) noexcept {
         if (this != &t_matrix) {
             if (Size() == t_matrix.m_rows * t_matrix.m_cols) {
                 std::copy_n(t_matrix.m_matrix, Size(), m_matrix);
             } else {
                 delete[] m_matrix;
                 Initialize(std::move(t_matrix));
             }
         }

         return *this;
     }

     std::ostream& operator<<(std::ostream& t_out, const S21Matrix& t_matrix) noexcept {
         std::int32_t i = 0, j = 0;
         S21Matrix::Cycle(i, j, t_matrix, [&](){
             if (j % t_matrix.m_cols == 0 && i != 0) {
                t_out << std::endl;
             }
             t_out << t_matrix(i, j) << ' ';
         });
         return t_out;
     }

    S21Matrix operator+(const S21Matrix& t_a, const S21Matrix& t_b) {
        if (t_a.Size() != t_b.Size()) {
            throw std::length_error("matrices sizes doesnt equal\n");
        }

        S21Matrix tmp;
        std::transform(t_a.m_matrix, t_a.m_matrix + t_a.Size(), t_b.m_matrix, tmp.m_matrix, std::plus());
        return tmp;
    }

    S21Matrix operator-(const S21Matrix& t_a, const S21Matrix& t_b) {
        if (t_a.Size() != t_b.Size()) {
            throw std::length_error("matrices sizes doesnt equal\n");
        }

        S21Matrix tmp;
        std::transform(t_a.m_matrix, t_a.m_matrix + t_a.Size(), t_b.m_matrix, tmp.m_matrix, std::minus());
        return tmp;
    }

    S21Matrix operator*(const S21Matrix& t_a, const S21Matrix& t_b) {
        if (t_a.m_cols != t_b.m_rows) {
            throw std::length_error("the number of columns of the first matrix is not equal to the number of rows of the second matrix\n");
        }

        S21Matrix tmp(t_a.m_rows, t_b.m_cols);
        S21Matrix::Cycle(tmp.m_i, tmp.m_j, tmp, [&](){
            double result = 0;
            for (int k = 0; k < tmp.m_cols; k++) {
                result += t_a(tmp.m_i, k) * t_b(k, tmp.m_j);
            }
            tmp(tmp.m_i, tmp.m_j) = result;
        });

        return tmp;
    }

    bool operator==(const S21Matrix& t_a, const S21Matrix& t_b) {
        if (t_a.Size() != t_b.Size()) {
            return false;
        }

        return std::equal(t_a.m_matrix, t_a.m_matrix + t_a.Size(), t_b.m_matrix, [](const double a, const double b){
            return std::fabs(a - b) >= EPS ? false : true;
        });
    }

    S21Matrix S21Matrix::operator*(const double t_num) {
        S21Matrix tmp;
        std::transform(m_matrix, m_matrix + Size(), tmp.m_matrix, std::bind1st(std::multiplies<double>(), t_num));
        return tmp;
    }

    S21Matrix& S21Matrix::operator+=(const S21Matrix& t_matrix) {
        return (*this = *this + t_matrix);
    }

    S21Matrix& S21Matrix::operator-=(const S21Matrix& t_matrix) {
        return (*this = *this - t_matrix);
    }

    S21Matrix& S21Matrix::operator*=(const S21Matrix& t_matrix) {
        return (*this = *this * t_matrix);
    }

    const double S21Matrix::operator()(std::int32_t t_i, std::int32_t t_j) const {
        if (t_i >= m_rows || t_j >= m_cols) {
            throw std::out_of_range("incorrect index. out of range\n");
        }

        return m_matrix[t_i * m_cols + t_j];
    }

    double& S21Matrix::operator()(std::int32_t t_i, std::int32_t t_j) {
        if (t_i >= m_rows || t_j >= m_cols) {
            throw std::out_of_range("incorrect index. out of range\n");
        }

        return m_matrix[t_i * m_cols + t_j];
    }

    S21Matrix S21Matrix::CalcComplements() {
        return{};
    }

    S21Matrix S21Matrix::InverseMatrix() {
        return{};
    }

    S21Matrix S21Matrix::Transpose() {
        S21Matrix tmp(m_rows, m_cols);
        S21Matrix::Cycle(m_i, m_j, tmp, [&](){
            tmp(m_j, m_i) = (*this)(m_i, m_j);
        });

        return tmp;
    }

    double S21Matrix::Determinant() {
        if (m_rows != m_cols) {
            throw std::logic_error("matrix are not square");
        }

//        double temp = 0;
//        for (int i = 0; i < m_rows; i++) {
//            for (int j = i + 1; j < m_rows; j++) {
//                temp =
//            }
//        }

        return {};
    }

    template<typename T>
    constexpr void S21Matrix::Initialize(T&& t_matrix) {
        if constexpr (std::is_rvalue_reference_v<T>) {
            m_rows = std::exchange(t_matrix.m_rows, 0);
            m_cols = std::exchange(t_matrix.m_cols, 0);
            m_matrix = std::exchange(t_matrix.m_matrix, nullptr);
        } else {
            m_rows = t_matrix.m_rows;
            m_cols = t_matrix.m_cols;
            m_matrix = new double[m_cols * m_rows]();
            std::copy_n(t_matrix.m_matrix, Size(), m_matrix);
        }
    }

    template <class Matrix, class Func>
    void S21Matrix::Cycle(std::int32_t& t_i, std::int32_t& t_j, Matrix&& t_matrix, Func func) {
        for (t_i = 0; t_i < t_matrix.m_rows; t_i++) {
            for (t_j = 0; t_j < t_matrix.m_cols; t_j++) {
                func();
            }
        } t_i = 0, t_j = 0;
    }

    std::int32_t S21Matrix::Size() const noexcept {
        return m_rows * m_cols;
    }

    bool S21Matrix::IsEmpty() const noexcept {
        return !m_matrix || !(m_rows > 0 && m_cols > 0);
    }
}
