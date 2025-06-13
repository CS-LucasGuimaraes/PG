#include "Prism/matrix.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>

using Prism::Matrix;

TEST(MatrixTest, Construction) {
    Matrix<double> m1(2, 3);
    ASSERT_EQ(m1.getRows(), 2);
    ASSERT_EQ(m1.getCols(), 3);

    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            ASSERT_DOUBLE_EQ(m1[i][j], 0.0);
        }
    }

    Matrix<int> m2 = {{1, 2, 3}, {4, 5, 6}};
    ASSERT_EQ(m2.getRows(), 2);
    ASSERT_EQ(m2.getCols(), 3);
    ASSERT_EQ(m2[1][1], 5);

    Matrix<int> m3(m2);
    ASSERT_TRUE(m3 == m2);

    ASSERT_THROW(Matrix<int> m_err(0, 5), std::invalid_argument);
    ASSERT_THROW(([]() { Matrix<int> m_err_list = {{1, 2}, {3}}; })(), std::invalid_argument);
}

TEST(MatrixTest, AccessAndAssignment) {
    Matrix<double> m1 = {{1.0, 2.0}, {3.0, 4.0}};

    ASSERT_DOUBLE_EQ(m1[0][1], 2.0);

    m1[0][1] = 42.0;
    ASSERT_DOUBLE_EQ(m1[0][1], 42.0);

    Matrix<double> m2(1, 1);
    m2 = m1;
    ASSERT_DOUBLE_EQ(m2[0][1], 42.0);
    ASSERT_TRUE(m1 == m2);

    ASSERT_THROW(m1[5][5], std::out_of_range);
}

TEST(MatrixTest, Equality) {
    Matrix<int> m1 = {{1, 2}, {3, 4}};
    Matrix<int> m2 = {{1, 2}, {3, 4}};
    Matrix<int> m3 = {{9, 8}, {7, 6}};
    Matrix<int> m4(3, 3); // Dimensões diferentes

    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
    ASSERT_FALSE(m1 == m4);
    ASSERT_TRUE(m1 != m3);
    ASSERT_TRUE(m1 != m4);
}

TEST(MatrixTest, Multiplication) {
    Matrix<double> m1 = {{1, 2, 3}, {4, 5, 6}};      // 2x3
    Matrix<double> m2 = {{7, 8}, {9, 10}, {11, 12}}; // 3x2

    Matrix<double> result = m1 * m2;
    Matrix<double> expected = {{58, 64}, {139, 154}};
    ASSERT_TRUE(result == expected);

    Matrix<double> m_err(5, 5);
    ASSERT_THROW(m1 * m_err, std::invalid_argument);
    ASSERT_THROW(m1 * m_err, std::invalid_argument);

    Matrix<double> m_scalar = {{1, 2}, {3, 4}};
    Matrix<double> result_scalar = m_scalar * 2.0;
    Matrix<double> expected_scalar = {{2, 4}, {6, 8}};
    ASSERT_TRUE(result_scalar == expected_scalar);

    m_scalar *= 2.0;
    ASSERT_TRUE(m_scalar == expected_scalar);
}

TEST(MatrixTest, DefaultConstructorCreates3x3ZeroMatrix) {
    Matrix<double> m;

    ASSERT_EQ(m.getRows(), 3);
    ASSERT_EQ(m.getCols(), 3);

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            ASSERT_DOUBLE_EQ(m[i][j], 0);
        }
    }
}