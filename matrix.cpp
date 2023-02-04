#include "include/matrix.hpp"

#include <iostream>
#include <math.h>

namespace s21 {
    S21Matrix::S21Matrix(): m_rows(default_rows), m_cols(default_cols) {
        m_matrix = new double[m_rows * m_cols]();
    }

    S21Matrix::S21Matrix(std::int32_t t_rows, std::int32_t t_cols): m_rows(t_rows), m_cols(t_cols) {
        if (t_rows <= 0 || t_cols <= 0) {
            throw std::invalid_argument("size are less then 0\n");
        }
        m_matrix = new double[m_rows * m_cols]();
    }

    S21Matrix::S21Matrix(const S21Matrix& t_matrix) {
        Initialize(t_matrix);
    }

    S21Matrix::S21Matrix(S21Matrix&& t_matrix) noexcept {
        Initialize<S21Matrix>(std::move(t_matrix));
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
                 Initialize<S21Matrix>(std::move(t_matrix));
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

        S21Matrix tmp(t_a.m_rows, t_a.m_cols);
        std::transform(t_a.m_matrix, t_a.m_matrix + t_a.Size(), t_b.m_matrix, tmp.m_matrix, std::plus());
        return tmp;
    }

    S21Matrix operator-(const S21Matrix& t_a, const S21Matrix& t_b) {
        if (t_a.Size() != t_b.Size()) {
            throw std::length_error("matrices sizes doesnt equal\n");
        }

        S21Matrix tmp(t_a.m_rows, t_a.m_cols);
        std::transform(t_a.m_matrix, t_a.m_matrix + t_a.Size(), t_b.m_matrix, tmp.m_matrix, std::minus());
        return tmp;
    }

    S21Matrix operator*(const S21Matrix& t_a, const S21Matrix& t_b) {
        if (t_a.m_cols != t_b.m_rows) {
            throw std::logic_error("the number of columns of the first matrix is not equal to the number of rows of the second matrix\n");
        }

        S21Matrix tmp(t_a.m_rows, t_b.m_cols);
        S21Matrix::Cycle(tmp.m_i, tmp.m_j, tmp, [&](){
            for (std::int32_t k = 0; k < t_a.m_cols; k++) {
                tmp(tmp.m_i, tmp.m_j) += t_a(tmp.m_i, k) * t_b(k, tmp.m_j);
            }
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
        S21Matrix tmp(m_rows, m_cols);
        std::transform(m_matrix, m_matrix + Size(), tmp.m_matrix, std::bind1st(std::multiplies<double>(), t_num));
        return tmp;
    }

    S21Matrix& S21Matrix::operator*=(const double t_num) {
        std::transform(m_matrix, m_matrix + Size(), m_matrix, std::bind1st(std::multiplies<double>(), t_num));
        return *this;
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

    double S21Matrix::operator()(std::int32_t t_i, std::int32_t t_j) const {
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

    bool S21Matrix::EqMatrix(const S21Matrix &t_matrix) const {
        return *this == t_matrix;
    }

    void S21Matrix::SumMatrix(const S21Matrix& t_matrix) {
        *this += t_matrix;
    }

    void S21Matrix::SubMatrix(const S21Matrix& t_matrix) {
        *this -= t_matrix;
    }

    void S21Matrix::MulMatrix(const S21Matrix& t_matrix) {
        *this = *this * t_matrix;
    }

    void S21Matrix::MulNumber(const double t_num) {
        *this *= t_num;
    }

    // thanks @iopmanu for good idea
    void S21Matrix::SetRows(const std::int32_t t_rows){
        if (t_rows < 1) {
            throw std::invalid_argument("incorrect row size\n");
        }

        S21Matrix tmp(t_rows, m_cols);
        for (std::int32_t i = 0; i < std::min(m_rows, t_rows); i++) {
            for (std::int32_t j = 0; j < m_cols; j++) {
                tmp(i, j) = (*this)(i, j);
            }
        }

        *this = std::move(tmp);
        m_rows = t_rows;
    }

    void S21Matrix::SetCols(const std::int32_t t_cols){
        if (t_cols < 1) {
            throw std::invalid_argument("incorrect row size\n");
        }

        S21Matrix tmp(m_rows, t_cols);
        for (std::int32_t i = 0; i < m_rows; i++) {
            for (std::int32_t j = 0; j < std::min(m_cols, t_cols); j++) {
                tmp(i, j) = (*this)(i, j);
            }
        }

        *this = std::move(tmp);
        m_cols = t_cols;
    }

    std::int32_t S21Matrix::GetRows() const noexcept {
        return m_rows;
    }

    std::int32_t S21Matrix::GetCols() const noexcept {
        return m_cols;
    }

    S21Matrix S21Matrix::InverseMatrix() {
        if (m_rows != m_cols) {
            std::logic_error("matrix are not square\n");
        }

        double determinant = Determinant();

        if (std::fabs(determinant) < EPS) {
            throw std::logic_error("error to calc determinant\n");
        }

        return CalcComplements().Transpose() * (1.0 / determinant);
    }

    S21Matrix S21Matrix::Transpose() {
        S21Matrix tmp(m_rows, m_cols);
        S21Matrix::Cycle(m_i, m_j, tmp, [&](){
            tmp(m_j, m_i) = (*this)(m_i, m_j);
        });

        return tmp;
    }

    double S21Matrix::Determinant() {
        if (m_rows != m_cols || IsEmpty()) {
            throw std::logic_error("matrix are not square\n");
        }

        double result = 0;

        if (m_rows == 1) {
            result = (*this)(0, 0);
        } else {
            result = DeterminantHelper(*this, m_rows);
        }

        return result;
    }

    void S21Matrix::Cofactor(const S21Matrix& t_src, S21Matrix& t_dst, std::int32_t t_jmp_r, std::int32_t t_jmp_c, std::int32_t t_size) {
        for (std::int32_t i = 0, row = 0; row < t_size; row++) {
            for (std::int32_t j = 0, col = 0; col < t_size; col++) {
                if (row != t_jmp_r && col != t_jmp_c) {
                    t_dst(i, j++) = t_src(row, col);
                    if (j == t_size - 1) {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    double S21Matrix::DeterminantHelper(const S21Matrix& t_matrix, std::int32_t t_size) {
        if (t_size == 1) {
            return t_matrix(0, 0);
        }

        S21Matrix tmp(t_size, t_size);
        std::int32_t sing = 1;
        double result = 0;

        for (std::int32_t i = 0; i < t_size; i++) {
            Cofactor(t_matrix, tmp, 0, i, t_size);
            result += sing * t_matrix(0, i) * DeterminantHelper(tmp, t_size - 1);
            sing = -sing;
        }

        return result;
    }

    S21Matrix S21Matrix::CalcHelper(const S21Matrix &t_matrix) {
        S21Matrix tmp(t_matrix.m_rows, t_matrix.m_cols);

        if (t_matrix.m_rows == 1) {
            tmp(0, 0) = 1;
            return tmp;
        }

        S21Matrix helper(t_matrix.m_rows, t_matrix.m_cols);

        S21Matrix::Cycle(m_i, m_j, (*this), [&](){
            Cofactor(t_matrix, helper, m_i, m_j, t_matrix.m_rows);
            std::int32_t sign = ((m_i + m_j) % 2 == 0) ? 1 : -1;
            tmp(m_i, m_j) = sign * DeterminantHelper(helper, t_matrix.m_rows - 1);
        });

        return tmp;
    }

    S21Matrix S21Matrix::CalcComplements() {
        if (m_rows != m_cols || IsEmpty()) {
            throw std::logic_error("matrix are not square\n");
        }

        return CalcHelper(*this);
    }

    bool S21Matrix::IsEmpty() const noexcept {
        return !m_matrix || !(m_rows > 0 && m_cols > 0);
    }

    template<typename T>
    constexpr void S21Matrix::Initialize(T&& t_matrix) noexcept(std::is_rvalue_reference_v<decltype(t_matrix)>) {
        if constexpr (std::is_rvalue_reference_v<decltype(t_matrix)>) {
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

    template <class T, class Fn>
    constexpr void S21Matrix::Cycle(std::int32_t& t_i, std::int32_t& t_j, T&& t_matrix, Fn func) {
        for (t_i = 0; t_i < t_matrix.m_rows; t_i++) {
            for (t_j = 0; t_j < t_matrix.m_cols; t_j++) {
                func();
            }
        }
        t_i = 0, t_j = 0;
    }

    std::int32_t S21Matrix::Size() const noexcept {
        return m_rows * m_cols;
    }
}
