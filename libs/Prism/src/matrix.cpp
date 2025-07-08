#include "Prism/matrix.hpp"
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace Prism {

// --- Implementação do Proxy (não-const) ---
MatrixRow::MatrixRow(Matrix& matrix, size_t row) : matrix_(matrix), row_(row) {
}

double& MatrixRow::operator[](size_t col) {
    // Adicionar checagem de limites (bounds checking) se desejar
    return matrix_.data_[row_ * matrix_.cols_ + col];
}

// --- Implementação do Proxy (const) ---
ConstMatrixRow::ConstMatrixRow(const Matrix& matrix, size_t row) : matrix_(matrix), row_(row) {
}

const double& ConstMatrixRow::operator[](size_t col) const {
    // Adicionar checagem de limites (bounds checking) se desejar
    return matrix_.data_[row_ * matrix_.cols_ + col];
}

// --- Implementação dos Construtores da Matrix ---

Matrix::Matrix() : rows_(3), cols_(3), data_(3 * 3, 0.0) {
    // Construtor padrão agora cria uma matriz identidade 3x3
    (*this)[0][0] = 1.0;
    (*this)[1][1] = 1.0;
    (*this)[2][2] = 1.0;
}

Matrix::Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
    if (rows == 0 || cols == 0) {
        // Permite matrizes 0x0 para consistência, mas não lança erro.
        // Opcional: throw std::invalid_argument("Matrix dimensions cannot be zero.");
        rows_ = 0;
        cols_ = 0;
    }
    data_.assign(rows_ * cols_, 0.0); // Usa 'assign' para preencher com zeros
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) {
    rows_ = list.size();
    if (rows_ > 0) {
        cols_ = list.begin()->size();
    } else {
        cols_ = 0;
    }

    data_.reserve(rows_ * cols_);
    for (const auto& row_list : list) {
        if (row_list.size() != cols_) {
            throw std::invalid_argument("All rows in initializer list must have the same size.");
        }
        // Insere os elementos da linha no final do vetor linear
        data_.insert(data_.end(), row_list.begin(), row_list.end());
    }
}

// --- Implementação dos Operadores ---

MatrixRow Matrix::operator[](size_t row) {
    return MatrixRow(*this, row);
}

ConstMatrixRow Matrix::operator[](size_t row) const {
    return ConstMatrixRow(*this, row);
}

bool Matrix::operator==(const Matrix& m) const {
    if (rows_ != m.rows_ || cols_ != m.cols_) {
        return false;
    }
    // std::vector já tem um operador== eficiente para dados contíguos.
    return data_ == m.data_;
}

bool Matrix::operator!=(const Matrix& m) const {
    return !(*this == m);
}

Matrix Matrix::operator*(const Matrix& m) const {
    if (cols_ != m.rows_) {
        throw std::invalid_argument("Matrix dimensions are not compatible for multiplication.");
    }
    Matrix result(rows_, m.cols_);
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < m.cols_; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < cols_; ++k) {
                sum += (*this)[i][k] * m[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

Point3 Matrix::operator*(const Point3& p) const {
    if (!((rows_ == 3 && cols_ == 3) || (rows_ == 4 && cols_ == 4))) {
        throw std::domain_error("Matrix must be 3x3 or 4x4 to multiply by a Point3.");
    }

    double x = (*this)[0][0] * p.x + (*this)[0][1] * p.y + (*this)[0][2] * p.z;
    double y = (*this)[1][0] * p.x + (*this)[1][1] * p.y + (*this)[1][2] * p.z;
    double z = (*this)[2][0] * p.x + (*this)[2][1] * p.y + (*this)[2][2] * p.z;
    double w = 1.0;

    if (rows_ == 4) { // Adiciona a parte da translação e perspectiva
        x += (*this)[0][3];
        y += (*this)[1][3];
        z += (*this)[2][3];
        w = (*this)[3][0] * p.x + (*this)[3][1] * p.y + (*this)[3][2] * p.z + (*this)[3][3];
    }

    if (w != 1.0 && w != 0.0) {
        return Point3(x / w, y / w, z / w);
    }
    return Point3(x, y, z);
}

Vector3 Matrix::operator*(const Vector3& v) const {
    if (!((rows_ == 3 && cols_ == 3) || (rows_ == 4 && cols_ == 4))) {
        throw std::domain_error("Matrix must be 3x3 or 4x4 to multiply by a Vector3.");
    }
    // A multiplicação de vetores ignora a translação (quarta coluna/linha)
    double x = (*this)[0][0] * v.x + (*this)[0][1] * v.y + (*this)[0][2] * v.z;
    double y = (*this)[1][0] * v.x + (*this)[1][1] * v.y + (*this)[1][2] * v.z;
    double z = (*this)[2][0] * v.x + (*this)[2][1] * v.y + (*this)[2][2] * v.z;
    return Vector3(x, y, z);
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(*this);
    result *= scalar;
    return result;
}


Matrix& Matrix::operator*=(const Matrix& m) {
    *this = *this * m;
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (double& val : data_) {
        val *= scalar;
    }
    return *this;
}

// --- Operações Matemáticas ---

double Matrix::determinant() const {
    if (rows_ != cols_) {
        throw std::domain_error("Matrix must be square to compute determinant.");
    }
    size_t n = rows_;
    if (n == 0)
        return 1.0;
    if (n == 1)
        return (*this)[0][0];
    if (n == 2)
        return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];

    // Para matrizes maiores, é mais eficiente usar a decomposição LU,
    // mas a expansão por cofatores é mantida aqui para consistência com o original.
    double det = 0.0;
    for (size_t j = 0; j < n; ++j) {
        Matrix submatrix(n - 1, n - 1);
        for (size_t r = 1; r < n; ++r) {
            size_t sub_col = 0;
            for (size_t c = 0; c < n; ++c) {
                if (c == j)
                    continue;
                submatrix[r - 1][sub_col++] = (*this)[r][c];
            }
        }
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        det += sign * (*this)[0][j] * submatrix.determinant();
    }
    return det;
}

Matrix Matrix::inverse() const {
    double det = determinant();
    if (std::abs(det) < 1e-9) { // Comparação segura para ponto flutuante
        throw std::domain_error("Matrix is singular and cannot be inverted.");
    }

    size_t n = rows_;
    // Cria uma matriz aumentada [A|I]
    Matrix augmented(n, 2 * n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            augmented[i][j] = (*this)[i][j];
        }
        augmented[i][i + n] = 1.0;
    }

    // Processo de eliminação de Gauss-Jordan
    for (size_t i = 0; i < n; i++) {
        // Encontra o pivô
        size_t pivot = i;
        for (size_t j = i + 1; j < n; j++) {
            if (std::abs(augmented[j][i]) > std::abs(augmented[pivot][i])) {
                pivot = j;
            }
        }
        // Troca as linhas
        for (size_t k = 0; k < 2 * n; ++k) {
            std::swap(augmented[i][k], augmented[pivot][k]);
        }

        // Normaliza a linha do pivô
        double div = augmented[i][i];
        for (size_t j = i; j < 2 * n; j++) {
            augmented[i][j] /= div;
        }

        // Elimina outras entradas na coluna
        for (size_t j = 0; j < n; j++) {
            if (i != j) {
                double mult = augmented[j][i];
                for (size_t k = i; k < 2 * n; k++) {
                    augmented[j][k] -= mult * augmented[i][k];
                }
            }
        }
    }

    // Extrai a matriz inversa da parte direita da matriz aumentada
    Matrix result(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result[i][j] = augmented[i][j + n];
        }
    }
    return result;
}

Matrix Matrix::transpose() const {
    Matrix transposed(cols_, rows_);
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            transposed[j][i] = (*this)[i][j];
        }
    }
    return transposed;
}

// --- Funções Estáticas (Factories) ---

Matrix Matrix::identity(size_t n) {
    Matrix id(n, n); // Já inicializa com zeros
    for (size_t i = 0; i < n; ++i) {
        id[i][i] = 1.0;
    }
    return id;
}

Matrix Matrix::translation(double tx, double ty, double tz) {
    Matrix t = identity(4);
    t[0][3] = tx;
    t[1][3] = ty;
    t[2][3] = tz;
    return t;
}

Matrix Matrix::scaling(double sx, double sy, double sz) {
    Matrix s = identity(4);
    s[0][0] = sx;
    s[1][1] = sy;
    s[2][2] = sz;
    return s;
}

Matrix Matrix::rotation(double angle, const Vector3& axis) {
    Vector3 a = axis.normalize();
    Matrix r = identity(4);
    double c = cos(angle);
    double s = sin(angle);
    double omc = 1.0 - c;

    r[0][0] = c + a.x * a.x * omc;
    r[0][1] = a.x * a.y * omc - a.z * s;
    r[0][2] = a.x * a.z * omc + a.y * s;

    r[1][0] = a.y * a.x * omc + a.z * s;
    r[1][1] = c + a.y * a.y * omc;
    r[1][2] = a.y * a.z * omc - a.x * s;

    r[2][0] = a.z * a.x * omc - a.y * s;
    r[2][1] = a.z * a.y * omc + a.x * s;
    r[2][2] = c + a.z * a.z * omc;

    return r;
}

} // namespace Prism