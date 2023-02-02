#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <exception>
#include <ostream>

namespace s21 {
    constexpr int default_rows = 3;
    constexpr int default_cols = 3;
    constexpr double EPS = 1e-6;

    class S21Matrix final {
    public:
        S21Matrix();

        S21Matrix(std::int32_t t_rows, std::int32_t t_cols);

        S21Matrix(const S21Matrix& t_matrix);

        S21Matrix(S21Matrix&& t_matrix) noexcept;

        ~S21Matrix();
    public:
        S21Matrix& operator=(const S21Matrix& t_matrix);

        S21Matrix& operator=(S21Matrix&& t_matrix) noexcept;

        friend std::ostream& operator<<(std::ostream& t_out, const S21Matrix& t_mat) noexcept;

        friend S21Matrix operator+(const S21Matrix& t_a, const S21Matrix& t_b);

        friend S21Matrix operator-(const S21Matrix& t_a, const S21Matrix& t_b);

        friend S21Matrix operator*(const S21Matrix& t_a, const S21Matrix& t_b);

        friend bool operator==(const S21Matrix& t_a, const S21Matrix& t_b);

        S21Matrix operator*(const double t_num);

        S21Matrix& operator+=(const S21Matrix& t_matrix);

        S21Matrix& operator-=(const S21Matrix& t_matrix);
    
        S21Matrix& operator*=(const S21Matrix& t_matrix);

        const double operator()(std::int32_t t_i, std::int32_t t_j) const;

        double& operator()(std::int32_t t_i, std::int32_t t_j);

        S21Matrix CalcComplements();

        S21Matrix InverseMatrix();

        S21Matrix Transpose();

        double Determinant();

        std::int32_t Size() const noexcept;

        bool IsEmpty() const noexcept;
    private:
        template<typename T>
        constexpr void Initialize(T&& t_matrix);

        template <class Matrix, class Func>
        static void Cycle(std::int32_t& t_i, std::int32_t& t_j, Matrix&& t_matrix, Func func);
    private:
        std::int32_t m_i = 0, m_j = 0;
        std::int32_t m_rows, m_cols;
        double* m_matrix;
    };
}

#endif
