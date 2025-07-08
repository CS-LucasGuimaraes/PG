#include "Prism/matrix.hpp"
#include <stdexcept>
#include <cmath>

namespace Prism {

// --- Implementações dos Construtores ---
Matrix::Matrix() : rows_(3), cols_(3), data_(3, std::vector<double>(3, 0.0)) {};

Matrix::Matrix(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("Matrix dimensions must be greater than zero.");
    }
    rows_ = rows;
    cols_ = cols;
    data_.resize(rows_, std::vector<double>(cols_, 0.0));
};

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> data) {
    rows_ = data.size();
    if (rows_ == 0) {
        cols_ = 0;
        data_ = {};
        return;
    }

    cols_ = data.begin()->size();
    data_.reserve(rows_);
    for (const auto& row : data) {
        if (row.size() != cols_) {
            throw std::invalid_argument("All rows in initializer list must have the same size.");
        }
        data_.emplace_back(row);
    }
}

Matrix::Matrix(const Matrix& m) : rows_(m.rows_), cols_(m.cols_), data_(m.data_) {};

// --- Implementações das Classes Aninhadas ---

Matrix::MatrixRow::MatrixRow(std::vector<double>& row_vec) : m_row_vector(row_vec) {}

double& Matrix::MatrixRow::operator[](int col) {
    return m_row_vector.at(col);
}

Matrix::ConstMatrixRow::ConstMatrixRow(const std::vector<double>& row_vec) : m_row_vector(row_vec) {}

const double& Matrix::ConstMatrixRow::operator[](int col) const {
    return m_row_vector.at(col);
}

// --- Implementações dos Getters ---

size_t Matrix::getRows() const {
    return rows_;
}

size_t Matrix::getCols() const {
    return cols_;
}

// --- Implementações dos Operadores ---

bool Matrix::operator==(const Matrix& m) const {
    if (rows_ != m.rows_ || cols_ != m.cols_) {
        return false;
    }
    return data_ == m.data_;
}

bool Matrix::operator!=(const Matrix& m) const {
    return !(*this == m);
}

Matrix& Matrix::operator=(const Matrix& m) {
    if (this != &m) {
        rows_ = m.rows_;
        cols_ = m.cols_;
        data_ = m.data_;
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix& m) const {
    if (cols_ != m.rows_) {
        throw std::invalid_argument("Matrix dimensions are not compatible for multiplication.");
    }
    Matrix result(rows_, m.cols_);
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < m.cols_; ++j) {
            for (size_t k = 0; k < cols_; ++k) {
                result.data_[i][j] += data_[i][k] * m.data_[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(const double& scalar) const {
    Matrix result(*this);
    result *= scalar;
    return result;
}

Point3 Matrix::operator*(const Point3& p) const {
    if (rows_ == 3 && cols_ == 3) {
        // Lógica para Matriz 3x3: aplica rotação/escala
        double x = data_[0][0] * p.x + data_[0][1] * p.y + data_[0][2] * p.z;
        double y = data_[1][0] * p.x + data_[1][1] * p.y + data_[1][2] * p.z;
        double z = data_[2][0] * p.x + data_[2][1] * p.y + data_[2][2] * p.z;
        return Point3(x, y, z);

    } else if (rows_ == 4 && cols_ == 4) {
        // Lógica para Matriz 4x4: aplica rotação/escala E translação
        // Coordenada homogênea w=1 para pontos
        double x = data_[0][0] * p.x + data_[0][1] * p.y + data_[0][2] * p.z + data_[0][3];
        double y = data_[1][0] * p.x + data_[1][1] * p.y + data_[1][2] * p.z + data_[1][3];
        double z = data_[2][0] * p.x + data_[2][1] * p.y + data_[2][2] * p.z + data_[2][3];
        double w = data_[3][0] * p.x + data_[3][1] * p.y + data_[3][2] * p.z + data_[3][3];

        if (w != 1.0 && w != 0.0) {
            return Point3(x / w, y / w, z / w);
        }
        return Point3(x, y, z);

    } else {
        throw std::domain_error("Matrix must be 3x3 or 4x4 to multiply by a Point3.");
    }
}

Vector3 Matrix::operator*(const Vector3& v) const {
    if ((rows_ == 3 && cols_ == 3) || (rows_ == 4 && cols_ == 4)) {
        // Lógica para Matriz 3x3: aplica rotação/escala
        // Lógica para Matriz 4x4: aplica rotação/escala (ignora translação)
        // Coordenada homogênea w=0 para vetores
        double x = data_[0][0] * v.x + data_[0][1] * v.y + data_[0][2] * v.z;
        double y = data_[1][0] * v.x + data_[1][1] * v.y + data_[1][2] * v.z;
        double z = data_[2][0] * v.x + data_[2][1] * v.y + data_[2][2] * v.z;
        return Vector3(x, y, z);
    } else {
        throw std::domain_error("Matrix must be 3x3 or 4x4 to multiply by a Vector3.");
    }
}

Matrix& Matrix::operator*=(const Matrix& m) {
    *this = *this * m;
    return *this;
}

Matrix& Matrix::operator*=(const double& scalar) {
    for (auto& row : data_) {
        for (auto& val : row) {
            val *= scalar;
        }
    }
    return *this;
}

Matrix::MatrixRow Matrix::operator[](size_t i) {
    return MatrixRow(data_.at(i));
}

Matrix::ConstMatrixRow Matrix::operator[](size_t i) const {
    return ConstMatrixRow(data_.at(i));
}


// --- Implementações das Operações Matemáticas ---

double Matrix::determinant() const {
    if (rows_ != cols_) {
        throw std::domain_error("Matrix must be square to compute determinant.");
    }
    if (rows_ == 0) return 1; // Determinant of 0x0 matrix is 1
    if (rows_ == 1) return data_[0][0];
    if (rows_ == 2) return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];

    double det = 0;
    for (size_t j = 0; j < cols_; ++j) {
        Matrix submatrix(rows_ - 1, cols_ - 1);
        for (size_t r = 1; r < rows_; ++r) {
            size_t sub_col = 0;
            for (size_t c = 0; c < cols_; ++c) {
                if (c != j) {
                    submatrix.data_[r - 1][sub_col++] = data_[r][c];
                }
            }
        }
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        det += sign * data_[0][j] * submatrix.determinant();
    }
    return det;
}

Matrix Matrix::inverse() const {
    double det = determinant();
    if (det == 0) {
        throw std::domain_error("Matrix is singular and cannot be inverted.");
    }

    size_t n = rows_;
    Matrix augmented(n, 2 * n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            augmented.data_[i][j] = data_[i][j];
        }
        augmented.data_[i][i + n] = 1;
    }

    for (size_t i = 0; i < n; i++) {
        size_t pivot = i;
        for (size_t j = i + 1; j < n; j++) {
            if (std::abs(augmented.data_[j][i]) > std::abs(augmented.data_[pivot][i])) {
                pivot = j;
            }
        }
        std::swap(augmented.data_[i], augmented.data_[pivot]);

        double div = augmented.data_[i][i];
        for (size_t j = i; j < 2 * n; j++) {
            augmented.data_[i][j] /= div;
        }

        for (size_t j = 0; j < n; j++) {
            if (i != j) {
                double mult = augmented.data_[j][i];
                for (size_t k = i; k < 2 * n; k++) {
                    augmented.data_[j][k] -= mult * augmented.data_[i][k];
                }
            }
        }
    }

    Matrix result(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result.data_[i][j] = augmented.data_[i][j + n];
        }
    }
    return result;
}

Matrix Matrix::transpose() const {
    Matrix transposed(cols_, rows_);
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            transposed.data_[j][i] = data_[i][j];
        }
    }
    return transposed;
}

// --- Implementações das Funções Estáticas (Factories) ---

Matrix Matrix::identity(size_t n) {
    Matrix id(n, n);
    for (size_t i = 0; i < n; ++i) {
        id.data_[i][i] = 1;
    }
    return id;
}

Matrix Matrix::translation(int dimension, std::initializer_list<double> values) {
    if (dimension <= 0 || values.size() != static_cast<size_t>(dimension)) {
        throw std::invalid_argument("Invalid dimension or values for translation matrix.");
    }

    Matrix t = identity(dimension + 1);
    auto val_it = values.begin();
    for (size_t i = 0; i < static_cast<size_t>(dimension); ++i) {
        t.data_[i][dimension] = *val_it++;
    }
    return t;
}

Matrix Matrix::scaling(int dimension, std::initializer_list<double> values) {
    if (dimension <= 0 || values.size() != static_cast<size_t>(dimension)) {
        throw std::invalid_argument("Invalid dimension or values for scaling matrix.");
    }
    Matrix s = identity(dimension + 1);
    auto val_it = values.begin();
    for (size_t i = 0; i < static_cast<size_t>(dimension); ++i) {
        s.data_[i][i] = *val_it++;
    }
    return s;
}

Matrix Matrix::rotation2d(double angle) {
    Matrix r = identity(3);
    double c = std::cos(angle);
    double s = std::sin(angle);
    r.data_[0][0] = c; r.data_[0][1] = -s;
    r.data_[1][0] = s; r.data_[1][1] = c;
    return r;
}

Matrix Matrix::rotation3d(double angle, const Vector3& axis) {
    Vector3 a = axis.normalize();
    Matrix r = identity(4);
    double c = cos(angle);
    double s = sin(angle);
    double omc = 1.0 - c;

    r.data_[0][0] = c + a.x * a.x * omc;
    r.data_[0][1] = a.x * a.y * omc - a.z * s;
    r.data_[0][2] = a.x * a.z * omc + a.y * s;

    r.data_[1][0] = a.y * a.x * omc + a.z * s;
    r.data_[1][1] = c + a.y * a.y * omc;
    r.data_[1][2] = a.y * a.z * omc - a.x * s;

    r.data_[2][0] = a.z * a.x * omc - a.y * s;
    r.data_[2][1] = a.z * a.y * omc + a.x * s;
    r.data_[2][2] = c + a.z * a.z * omc;

    return r;
}

} // namespace Prism