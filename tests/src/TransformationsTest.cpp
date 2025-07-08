#include <cmath>
#include <gtest/gtest.h>
#include <memory>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Prism.hpp"
#include "TestHelpers.hpp" // Para comparações de ponto flutuante

using namespace Prism;

// Testa a transformação de um raio
TEST(TransformationsTest, RayTransform) {
    // Arrange
    Ray r(Point3(0, 0, 0), Vector3(0, 0, 1));
    // CORREÇÃO: Passando a dimensão (3D) e a lista de valores
    Matrix translation = Matrix::translation(10, 5, -3);

    // Act
    Ray transformed_r = r.transform(translation);

    // Assert
    AssertPointAlmostEqual(transformed_r.origin(), Point3(10, 5, -3));
    AssertVectorAlmostEqual(transformed_r.direction(), Vector3(0, 0, 1));
}

// Testa a colisão com uma esfera transladada
TEST(TransformationsTest, SphereHitWithTranslation) {
    // Arrange
    auto s = std::make_shared<Sphere>(Point3(0, 0, 0), 1.0, nullptr);
    // CORREÇÃO: Passando a dimensão (3D) e a lista de valores
    Matrix translation = Matrix::translation(10, 0, 0);
    s->setTransform(translation);

    Ray ray_that_hits(Point3(10, 0, -5), Vector3(0, 0, 1));
    Ray ray_that_misses(Point3(0, 0, -5), Vector3(0, 0, 1));
    HitRecord rec;

    // Act & Assert
    EXPECT_TRUE(s->hit(ray_that_hits, 0, 100, rec));
    EXPECT_NEAR(rec.t, 4.0, 1e-6);
    AssertPointAlmostEqual(rec.p, Point3(10, 0, -1));
    AssertVectorAlmostEqual(rec.normal, Vector3(0, 0, -1));

    EXPECT_FALSE(s->hit(ray_that_misses, 0, 100, rec));
}

// Testa a colisão com uma esfera com escala não uniforme
TEST(TransformationsTest, SphereHitWithNonUniformScale) {
    // Arrange
    auto s = std::make_shared<Sphere>(Point3(0, 0, 0), 1.0, nullptr);
    // Estica a esfera 2x no eixo Y
    Matrix scale = Matrix::scaling(1, 2, 1);
    s->setTransform(scale);

    // Atira um raio de cima para baixo, em direção ao topo da esfera esticada
    Ray ray_that_hits(Point3(0, 5, 0), Vector3(0, -1, 0));
    HitRecord rec;

    // Act & Assert
    EXPECT_TRUE(s->hit(ray_that_hits, 0, 100, rec));
    EXPECT_NEAR(rec.t, 1.5, 1e-6);

    // As asserções mais importantes (ponto e normal em espaço global) continuam as mesmas.
    AssertPointAlmostEqual(rec.p, Point3(0, 2, 0));
    AssertVectorAlmostEqual(rec.normal, Vector3(0, 1, 0));
}

// Testa a colisão com um plano rotacionado
TEST(TransformationsTest, PlaneHitWithRotation) {
    // Arrange
    auto p = std::make_shared<Plane>(Point3(0, 0, 0), Vector3(0, 1, 0), nullptr);
    Matrix rotation = Matrix::rotation(M_PI / 2.0, Vector3(0, 0, 1));
    p->setTransform(rotation);

    Ray ray_that_hits(Point3(-5, 0, 0), Vector3(1, 0, 0));
    Ray ray_that_misses(Point3(0, -5, 0), Vector3(0, 1, 0));
    HitRecord rec;

    // Act & Assert
    EXPECT_TRUE(p->hit(ray_that_hits, 0, 100, rec));
    EXPECT_NEAR(rec.t, 5.0, 1e-6);
    AssertPointAlmostEqual(rec.p, Point3(0, 0, 0));
    AssertVectorAlmostEqual(rec.normal, Vector3(-1, 0, 0));

    EXPECT_FALSE(p->hit(ray_that_misses, 0, 100, rec));
}

// Testa a colisão com um triângulo transladado
TEST(TransformationsTest, TriangleHitWithTranslation) {
    // Arrange
    auto tri =
        std::make_shared<Triangle>(Point3(0, 0, 0), Point3(1, 0, 0), Point3(0, 1, 0), nullptr);
    Matrix translation = Matrix::translation(0, 0, 5);
    tri->setTransform(translation);

    Ray ray_that_hits(Point3(0.25, 0.25, 0), Vector3(0, 0, 1));
    HitRecord rec;

    // Act & Assert
    EXPECT_TRUE(tri->hit(ray_that_hits, 0, 100, rec));
    EXPECT_NEAR(rec.t, 5.0, 1e-6);
    AssertPointAlmostEqual(rec.p, Point3(0.25, 0.25, 5));

    // CORREÇÃO: A normal deve ser (0,0,-1) porque o raio atinge por trás
    // e a função set_face_normal inverte a normal para apontar para a câmera.
    AssertVectorAlmostEqual(rec.normal, Vector3(0, 0, -1));
}

class TestableObject : public Object {
  public:
    // Implementação mínima para a função virtual pura
    bool hit(const Ray&, double, double, HitRecord&) const override {
        return false;
    }

    // Funções para acessar as matrizes protegidas da classe base
    const Matrix& getTransform() const {
        return transform;
    }
    const Matrix& getInverseTransform() const {
        return inverseTransform;
    }
    const Matrix& getInverseTransposeTransform() const {
        return inverseTransposeTransform;
    }
};

TEST(TransformationsTest, ObjectSetTransform) {
    // Arrange
    TestableObject test_obj;
    Matrix t = Matrix::translation(10, 20, 30);
    Matrix inv_t = t.inverse();
    Matrix inv_t_transpose = inv_t.transpose();

    // Act
    test_obj.setTransform(t);

    // Assert
    // Verifica se as três matrizes internas foram definidas corretamente.
    // Usamos EXPECT_EQ aqui porque as operações devem ser exatas.
    EXPECT_EQ(test_obj.getTransform(), t);
    EXPECT_EQ(test_obj.getInverseTransform(), inv_t);
    EXPECT_EQ(test_obj.getInverseTransposeTransform(), inv_t_transpose);
}