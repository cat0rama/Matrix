#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <type_traits>
#include <algorithm>
#include <exception>
#include <ostream>

namespace s21 {
    constexpr std::int32_t default_rows = 3;
    constexpr std::int32_t default_cols = 3;
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

        S21Matrix& operator*=(const double t_num);

        S21Matrix& operator+=(const S21Matrix& t_matrix);

        S21Matrix& operator-=(const S21Matrix& t_matrix);
    
        S21Matrix& operator*=(const S21Matrix& t_matrix);

        double operator()(std::int32_t t_i, std::int32_t t_j) const;

        double& operator()(std::int32_t t_i, std::int32_t t_j);
    public:
        bool EqMatrix(const S21Matrix& t_matrix) const;

        void SumMatrix(const S21Matrix& t_matrix);

        void SubMatrix(const S21Matrix& t_matrix);

        void MulMatrix(const S21Matrix& t_matrix);

        void MulNumber(const double t_num);

        void SetRows(const std::int32_t t_rows);

        void SetCols(const std::int32_t t_cols);

        std::int32_t GetRows() const noexcept;

        std::int32_t GetCols() const noexcept;

        S21Matrix CalcComplements();

        S21Matrix InverseMatrix();

        S21Matrix Transpose();

        double Determinant();

        std::int32_t Size() const noexcept;

        bool IsEmpty() const noexcept;
    private:
        void Cofactor(const S21Matrix& t_src, S21Matrix& t_dst, std::int32_t t_jmp_r, std::int32_t t_jmp_c, std::int32_t t_size);

        double DeterminantHelper(const S21Matrix& t_matrix, std::int32_t t_size);

        S21Matrix CalcHelper(const S21Matrix& t_matrix);

        template<typename T>
        constexpr void Initialize(T&& t_matrix) noexcept(std::is_rvalue_reference_v<decltype(t_matrix)>);

        template <class T, class Fn>
        constexpr static void Cycle(std::int32_t& t_i, std::int32_t& t_j, T&& t_matrix, Fn func);
    private:
        std::int32_t m_i = 0, m_j = 0;
        std::int32_t m_rows, m_cols;
        double* m_matrix;
    };
}

#endif
