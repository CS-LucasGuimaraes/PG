#include "Prism/matrix.hpp"
#include "Prism/vector.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>

using Prism::Matrix;
using Prism::Vector3;

TEST(MatrixTest, Construction) {
    Matrix m1(2, 3);
    ASSERT_EQ(m1.getRows(), 2);
    ASSERT_EQ(m1.getCols(), 3);

    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            ASSERT_DOUBLE_EQ(m1[i][j], 0.0);
        }
    }

    Matrix m2 = {{1, 2, 3}, {4, 5, 6}};
    ASSERT_EQ(m2.getRows(), 2);
    ASSERT_EQ(m2.getCols(), 3);
    ASSERT_EQ(m2[1][1], 5);

    Matrix m3(m2);
    ASSERT_TRUE(m3 == m2);

    ASSERT_THROW(Matrix m_err(0, 5), std::invalid_argument);
    ASSERT_THROW(([]() { Matrix m_err_list = {{1, 2}, {3}}; })(), std::invalid_argument);
}

TEST(MatrixTest, AccessAndAssignment) {
    Matrix m1 = {{1.0, 2.0}, {3.0, 4.0}};

    ASSERT_DOUBLE_EQ(m1[0][1], 2.0);

    m1[0][1] = 42.0;
    ASSERT_DOUBLE_EQ(m1[0][1], 42.0);

    Matrix m2(1, 1);
    m2 = m1;
    ASSERT_DOUBLE_EQ(m2[0][1], 42.0);
    ASSERT_TRUE(m1 == m2);

    ASSERT_THROW(m1[5][5], std::out_of_range);
}

TEST(MatrixTest, Equality) {
    Matrix m1 = {{1, 2}, {3, 4}};
    Matrix m2 = {{1, 2}, {3, 4}};
    Matrix m3 = {{9, 8}, {7, 6}};
    Matrix m4(3, 3); // Dimensões diferentes

    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
    ASSERT_FALSE(m1 == m4);
    ASSERT_TRUE(m1 != m3);
    ASSERT_TRUE(m1 != m4);
}

TEST(MatrixTest, Multiplication) {
    Matrix m1 = {{1, 2, 3}, {4, 5, 6}};      // 2x3
    Matrix m2 = {{7, 8}, {9, 10}, {11, 12}}; // 3x2

    Matrix result = m1 * m2;
    Matrix expected = {{58, 64}, {139, 154}};
    ASSERT_TRUE(result == expected);

    Matrix m_err(5, 5);
    ASSERT_THROW(m1 * m_err, std::invalid_argument);
    ASSERT_THROW(m1 * m_err, std::invalid_argument);

    Matrix m_scalar = {{1, 2}, {3, 4}};
    Matrix result_scalar = m_scalar * 2.0;
    Matrix expected_scalar = {{2, 4}, {6, 8}};
    ASSERT_TRUE(result_scalar == expected_scalar);

    m_scalar *= 2.0;
    ASSERT_TRUE(m_scalar == expected_scalar);
}

TEST(MatrixTest, DefaultConstructorCreates3x3ZeroMatrix) {
    Matrix m;

    ASSERT_EQ(m.getRows(), 3);
    ASSERT_EQ(m.getCols(), 3);

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            ASSERT_DOUBLE_EQ(m[i][j], 0);
        }
    }
}

TEST(MatrixTest, Inverse) {
    Matrix m = {{4, 7}, {2, 6}};
    Matrix inv = m.inverse();
    Matrix expected_inv = {{0.6, -0.7}, {-0.2, 0.4}};

    AssertMatrixAlmostEqual(inv, expected_inv);

    // Test with a non-invertible matrix
    Matrix singular = {{1, 2}, {2, 4}};
    ASSERT_THROW(singular.inverse(), std::domain_error);
}

TEST(MatrixTest, Determinant) {
    Matrix m1 = {{1, 2}, {3, 4}};
    ASSERT_DOUBLE_EQ(m1.determinant(), -2.0);

    Matrix m2 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    ASSERT_DOUBLE_EQ(m2.determinant(), 1.0);

    Matrix m3 = {{1, 2}, {3, 5}};
    ASSERT_DOUBLE_EQ(m3.determinant(), -1.0);

    Matrix singular = {{1, 2}, {2, 4}};
}

TEST(MatrixTest, Transpose) {
    Matrix m = {{1, 2, 3}, {4, 5, 6}};
    Matrix transposed = m.transpose();
    Matrix expected = {{1, 4}, {2, 5}, {3, 6}};
    AssertMatrixAlmostEqual(transposed, expected);

    Matrix m_square = {{1, 2}, {3, 4}};
    Matrix expected_square = {{1, 3}, {2, 4}};
    AssertMatrixAlmostEqual(m_square.transpose(), expected_square);

    Matrix m_symmetric = {{1, 8, 7}, {8, 2, 4}, {7, 4, 3}};
    AssertMatrixAlmostEqual(m_symmetric.transpose(), m_symmetric);

    Matrix m_identity = Matrix::identity(3);
    AssertMatrixAlmostEqual(m_identity.transpose(), m_identity);
}

TEST(MatrixTest, IdentityMatrix) {
    Matrix identity = Matrix::identity(3);
    ASSERT_EQ(identity.getRows(), 3);
    ASSERT_EQ(identity.getCols(), 3);

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (i == j) {
                ASSERT_DOUBLE_EQ(identity[i][j], 1.0);
            } else {
                ASSERT_DOUBLE_EQ(identity[i][j], 0.0);
            }
        }
    }
}

TEST(MatrixTest, TranslationMatrix) {
    Matrix translation = Matrix::translation(2, {3.0, 4.0});
    ASSERT_EQ(translation.getRows(), 3);
    ASSERT_EQ(translation.getCols(), 3);
    ASSERT_DOUBLE_EQ(translation[0][2], 3.0);
    ASSERT_DOUBLE_EQ(translation[1][2], 4.0);
    ASSERT_DOUBLE_EQ(translation[2][2], 1.0);

    // Test with invalid dimension
    ASSERT_THROW(Matrix::translation(-1, {1.0}), std::invalid_argument);
    ASSERT_THROW(Matrix::translation(2, {1.0}), std::invalid_argument);
}

TEST(MatrixTest, ScalingMatrix) {
    Matrix scaling = Matrix::scaling(2, {2.0, 3.0});
    ASSERT_EQ(scaling.getRows(), 3);
    ASSERT_EQ(scaling.getCols(), 3);
    ASSERT_DOUBLE_EQ(scaling[0][0], 2.0);
    ASSERT_DOUBLE_EQ(scaling[1][1], 3.0);
    ASSERT_DOUBLE_EQ(scaling[2][2], 1.0);

    // Test with invalid dimension
    ASSERT_THROW(Matrix::scaling(-1, {1.0}), std::invalid_argument);
    ASSERT_THROW(Matrix::scaling(2, {1.0}), std::invalid_argument);
}

TEST(MatrixTest, Rotation2D) {
    double angle = M_PI / 4; // 45 degrees
    Matrix rotation = Matrix::rotation2d(angle);
    ASSERT_EQ(rotation.getRows(), 3);
    ASSERT_EQ(rotation.getCols(), 3);

    ASSERT_NEAR(rotation[0][0], std::cos(angle), 1e-9);
    ASSERT_NEAR(rotation[0][1], -std::sin(angle), 1e-9);
    ASSERT_NEAR(rotation[1][0], std::sin(angle), 1e-9);
    ASSERT_NEAR(rotation[1][1], std::cos(angle), 1e-9);
    ASSERT_DOUBLE_EQ(rotation[2][2], 1.0);
}

TEST(MatrixTest, Rotation3D) {
    Vector3 axis(1, 0, 0);   // Rotation around x-axis
    double angle = M_PI / 4; // 45 degrees
    Matrix rotation = Matrix::rotation3d(angle, axis);
    ASSERT_EQ(rotation.getRows(), 4);
    ASSERT_EQ(rotation.getCols(), 4);

    ASSERT_NEAR(rotation[1][1], std::cos(angle), 1e-9);
    ASSERT_NEAR(rotation[1][2], -std::sin(angle), 1e-9);
    ASSERT_NEAR(rotation[2][1], std::sin(angle), 1e-9);
    ASSERT_NEAR(rotation[2][2], std::cos(angle), 1e-9);
    ASSERT_DOUBLE_EQ(rotation[0][0], 1.0);
    ASSERT_DOUBLE_EQ(rotation[3][3], 1.0);
}