#include "ObjReader/ObjReader.hpp"
#include "Prism.hpp"
#include <iostream>

int main() {
    Prism::Vector3 v1(1, 2, 3);
    Prism::Vector3 v2(4, 5, 6);

    std::cout << "Vector 1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
    std::cout << "Vector 2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;
    Prism::Vector3 v3 = v1 + v2;
    std::cout << "Vector 3 (v1 + v2): (" << v3.x << ", " << v3.y << ", " << v3.z << ")"
              << std::endl;

    std::cout << "Hello, Sílvio!" << std::endl;

    v1.cross(v2);

    objReader obj("data/inputs/cubo.obj");

    obj.print_faces();

    return 0;
}