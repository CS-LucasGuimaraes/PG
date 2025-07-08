#ifndef PRISM_MATRIX_HPP_
#define PRISM_MATRIX_HPP_

#include "prism_export.h"

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>
#include <cmath>
#include "Prism/vector.hpp"
#include "Prism/point.hpp"

namespace Prism {

class PRISM_EXPORT Matrix {
public:
    // --- Construtores e Destrutor ---
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(std::initializer_list<std::initializer_list<double>> data);
    Matrix(const Matrix& m);

    // --- Classes aninhadas para acesso ---
    class PRISM_EXPORT MatrixRow {
    private:
        std::vector<double>& m_row_vector;
    public:
        MatrixRow(std::vector<double>& row_vec);
        double& operator[](int col);
    };

    class PRISM_EXPORT ConstMatrixRow {
    private:
        const std::vector<double>& m_row_vector;
    public:
        ConstMatrixRow(const std::vector<double>& row_vec);
        const double& operator[](int col) const;
    };

    // --- Getters ---
    size_t getRows() const;
    size_t getCols() const;

    // --- Operadores ---
    bool operator==(const Matrix& m) const;
    bool operator!=(const Matrix& m) const;
    Matrix& operator=(const Matrix& m);
    Matrix operator*(const Matrix& m) const;
    Matrix operator*(const double& scalar) const;
    Point3 operator*(const Point3& p) const;
    Vector3 operator*(const Vector3& v) const;
    Matrix& operator*=(const Matrix& m);
    Matrix& operator*=(const double& scalar);
    MatrixRow operator[](size_t i);
    ConstMatrixRow operator[](size_t i) const;

    // --- Operações Matemáticas ---
    double determinant() const;
    Matrix inverse() const;
    Matrix transpose() const;

    // --- Funções Estáticas (Factories) ---
    static Matrix identity(size_t n);
    static Matrix translation(int dimension, std::initializer_list<double> values);
    static Matrix scaling(int dimension, std::initializer_list<double> values);
    static Matrix rotation2d(double angle);
    static Matrix rotation3d(double angle, const Vector3& axis);

private:
    size_t rows_;
    size_t cols_;
    std::vector<std::vector<double>> data_;
};

} // namespace Prism

#endif // PRISM_MATRIX_HPP_