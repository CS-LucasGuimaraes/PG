#ifndef PRISM_MATRIX_HPP_
#define PRISM_MATRIX_HPP_

#include "prism_export.h"

#include "Prism/core/point.hpp"
#include "Prism/core/vector.hpp"

#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace Prism {

class Matrix; // Forward declaration for proxy classes

/**
 * @class MatrixRow
 * @brief Proxy class for accessing a specific row of a Matrix.
 * This class allows access to a row of the matrix using the syntax `matrix[row][col]`.
 *
 * A MatrixRow object is created by the Matrix class when using the `operator[]` to access a row.
 * It provides a way to access and modify elements in that row without exposing the entire matrix.
 */
class PRISM_EXPORT MatrixRow {
  public:
    /**
     * @brief Constructs a MatrixRow for a specific row of a Matrix.
     * @param matrix The Matrix to access.
     * @param row The index of the row to access.
     * This constructor initializes the MatrixRow with a reference to the specified matrix and the
     * row index.
     * @throws std::out_of_range if the row index is out of bounds.
     */
    MatrixRow(Matrix& matrix, size_t row);

    /**
     * @brief Accesses an element in the specified row of the matrix.
     * @param col The index of the column to access.
     * @return A reference to the element at the specified row and column.
     * @throws std::out_of_range if the column index is out of bounds.
     * This operator allows you to access elements in the specified row of the matrix using the
     * syntax `matrix[row][col]`.
     */
    double& operator[](size_t col);

  private:
    Matrix& matrix_; ///< Reference to the matrix being accessed
    size_t row_;     ///< Index of the row being accessed
};

/**
 * @class ConstMatrixRow
 * @brief Proxy class for accessing a specific row of a Matrix in a const context.
 * This class allows read-only access to a row of the matrix using the syntax `matrix[row][col]`.
 *
 * A ConstMatrixRow object is created by the Matrix class when using the `operator[]` to access a
 * row in a const context. It provides a way to access elements in that row without allowing
 * modifications, ensuring the integrity of the matrix data.
 */
class PRISM_EXPORT ConstMatrixRow {
  public:
    /**
     * @brief Constructs a ConstMatrixRow for a specific row of a Matrix.
     * @param matrix The Matrix to access.
     * @param row The index of the row to access.
     * This constructor initializes the ConstMatrixRow with a reference to the specified matrix and
     * the row index.
     * @throws std::out_of_range if the row index is out of bounds.
     */
    ConstMatrixRow(const Matrix& matrix, size_t row);

    /**
     * @brief Accesses an element in the specified row of the matrix.
     * @param col The index of the column to access.
     * @return A const reference to the element at the specified row and column.
     * @throws std::out_of_range if the column index is out of bounds.
     * This operator allows you to access elements in the specified row of the matrix using the
     * syntax `const_matrix[row][col]`.
     */
    const double& operator[](size_t col) const;

  private:
    const Matrix& matrix_; ///< Reference to the matrix being accessed
    size_t row_;           ///< Index of the row being accessed
};

/**
 * @class Matrix
 * @brief Represents a mathematical matrix with various operations.
 * The Matrix class provides a flexible way to create, manipulate, and perform mathematical
 * operations on matrices. It supports basic operations like addition, subtraction, multiplication,
 * and more advanced operations like determinant calculation and matrix inversion.
 */
class PRISM_EXPORT Matrix {
    friend class MatrixRow;      // Allows MatrixRow to access private members of Matrix
    friend class ConstMatrixRow; // Allows ConstMatrixRow to access private members of Matrix

  public:
    /**
     * @brief Default constructor that initializes a 3x3 identity matrix.
     * This constructor creates a 3x3 matrix with ones on the diagonal and zeros elsewhere.
     * It is useful for initializing matrices in transformations where an identity matrix is needed.
     */
    Matrix();

    /**
     * @brief Constructs a Matrix with specified dimensions.
     * @param rows The number of rows in the matrix.
     * @param cols The number of columns in the matrix.
     */
    Matrix(size_t rows, size_t cols);

    /**
     * @brief Constructs a Matrix from an initializer list of initializer lists.
     * @param data An initializer list containing rows of the matrix, where each row is itself an
     * initializer list of doubles.
     * @throws std::invalid_argument if the rows have different sizes.
     */
    Matrix(std::initializer_list<std::initializer_list<double>> data);

    /**
     * @brief Gets the number of rows in the matrix.
     * @return The number of rows in the matrix.
     */
    size_t getRows() const {
        return rows_;
    }

    /**
     * @brief Gets the number of columns in the matrix.
     * @return The number of columns in the matrix.
     */
    size_t getCols() const {
        return cols_;
    }

    /**
     * @brief Accesses a specific row of the matrix.
     * @param row The index of the row to access.
     * @return A MatrixRow object that allows access to the specified row.
     * @throws std::out_of_range if the row index is out of bounds.
     *
     * This operator allows you to access a row of the matrix using the syntax `matrix[row]`.
     * It returns a MatrixRow object that can be used to access or modify elements in that row.
     */
    MatrixRow operator[](size_t row);

    /**
     * @brief Accesses a specific row of the matrix in a const context.
     * @param row The index of the row to access.
     * @return A ConstMatrixRow object that allows read-only access to the specified row.
     * @throws std::out_of_range if the row index is out of bounds.
     *
     * This operator allows you to access a row of the matrix in a const context using the syntax
     * `matrix[row]`. It returns a ConstMatrixRow object that can be used to read elements in that
     * row without allowing modifications.
     */
    ConstMatrixRow operator[](size_t row) const;

    /**
     * @brief Checks if two matrices are equal.
     * @param m The matrix to compare with.
     * @return True if the matrices have the same dimensions and all corresponding elements are
     * equal, false otherwise.
     */
    bool operator==(const Matrix& m) const;

    /**
     * @brief Checks if two matrices are not equal.
     * @param m The matrix to compare with.
     * @return True if the matrices differ in dimensions or any corresponding elements are not
     * equal, false otherwise.
     */
    bool operator!=(const Matrix& m) const;

    /**
     * @brief Multiplies this matrix by another matrix.
     * @param m The matrix to multiply with.
     * @return A new Matrix that is the result of the multiplication.
     * @throws std::invalid_argument if the number of columns in this matrix does not match the
     * number of rows in the other matrix.
     */
    Matrix operator*(const Matrix& m) const;

    /**
     * @brief Multiplies this matrix by a scalar.
     * @param scalar The scalar value to multiply with.
     * @return A new Matrix that is the result of the multiplication.
     */
    Matrix operator*(double scalar) const;

    /**
     * @brief Multiplies this matrix by a Point3.
     * @param p The Point3 to multiply with.
     * @return A Point3 that is the result of the multiplication.
     * @throws std::domain_error if the matrix is not 3x3 or 4x4.
     *
     * This operator allows you to multiply a matrix by a Point3, which is useful for applying
     * transformations such as translation, rotation, or scaling to a point in 3D space.
     */
    Point3 operator*(const Point3& p) const;

    /**
     * @brief Multiplies this matrix by a Vector3.
     * @param v The Vector3 to multiply with.
     * @return A Vector3 that is the result of the multiplication.
     * @throws std::domain_error if the matrix is not 3x3 or 4x4.
     *
     * This operator allows you to multiply a matrix by a Vector3, which is useful for transforming
     * vectors in 3D space.
     */
    Vector3 operator*(const Vector3& v) const;

    /**
     * @brief Multiplies this matrix by another matrix in place.
     * @param m The matrix to multiply with.
     * @return Reference to this matrix after multiplication.
     *
     * This operator allows you to perform matrix multiplication and update the current matrix with
     * the result.
     */
    Matrix& operator*=(const Matrix& m);

    /**
     * @brief Multiplies this matrix by a scalar in place.
     * @param scalar The scalar value to multiply with.
     * @return Reference to this matrix after multiplication.
     *
     * This operator allows you to scale all elements of the matrix by a scalar value, modifying the
     * current matrix.
     */
    Matrix& operator*=(double scalar);

    /**
     * @brief Computes the determinant of the matrix.
     * @return The determinant of the matrix.
     * @throws std::domain_error if the matrix is not square.
     */
    double determinant() const;

    /**
     * @brief Computes the inverse of the matrix.
     * @return A new Matrix that is the inverse of this matrix.
     * @throws std::domain_error if the matrix is singular (determinant is zero).
     *
     * This method uses the Gauss-Jordan elimination method to compute the inverse of the matrix.
     * It is applicable only for square matrices (3x3 or 4x4).
     */
    Matrix inverse() const;

    /**
     * @brief Computes the transpose of the matrix.
     * @return A new Matrix that is the transpose of this matrix.
     * This method swaps the rows and columns of the matrix, effectively flipping it over its
     * diagonal.
     * @return A new Matrix that is the transpose of this matrix.
     */
    Matrix transpose() const;

    /**
     * @brief Creates an identity matrix of size n x n.
     * @param n The size of the identity matrix (number of rows and columns).
     * @return A new Matrix that is the identity matrix of size n x n.
     * An identity matrix has ones on the diagonal and zeros elsewhere, serving as the
     * multiplicative identity in matrix operations.
     */
    static Matrix identity(size_t n);

    /**
     * @brief Creates a translation matrix.
     * @param tx The translation distance along the x-axis.
     * @param ty The translation distance along the y-axis.
     * @param tz The translation distance along the z-axis.
     * @return A new Matrix that represents a translation transformation.
     * This matrix can be used to translate points or vectors in 3D space by adding the translation
     * distances to the respective coordinates.
     */
    static Matrix translation(double tx, double ty, double tz);

    /**
     * @brief Creates a scaling matrix.
     * @param sx The scaling factor along the x-axis.
     * @param sy The scaling factor along the y-axis.
     * @param sz The scaling factor along the z-axis.
     * @return A new Matrix that represents a scaling transformation.
     * This matrix can be used to scale points or vectors in 3D space by multiplying the respective
     * coordinates by the scaling factors.
     */
    static Matrix scaling(double sx, double sy, double sz);

    /**
     * @brief Creates a rotation matrix.
     * @param angle The angle of rotation in radians.
     * @param axis The axis of rotation as a Vector3.
     * @return A new Matrix that represents a rotation transformation.
     * This matrix can be used to rotate points or vectors in 3D space around the specified axis by
     * the given angle.
     */
    static Matrix rotation(double angle, const Vector3& axis);

  private:
    size_t rows_;              // Number of rows in the matrix
    size_t cols_;              // Number of columns in the matrix
    std::vector<double> data_; // Data storage for the matrix elements
};

} // namespace Prism

#endif // PRISM_MATRIX_HPP_