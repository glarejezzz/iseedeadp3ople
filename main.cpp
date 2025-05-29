#include "geometry.h"

int main() {
    // Создаем треугольник
    Triangle t(3, 4, 5);
    t.printInfo();

    // Создаем треугольную пирамиду
    TriangularPyramid tp(3, 4, 5, 7);
    tp.printInfo();

    // Создаем усеченную треугольную пирамиду
    FrustumTriangularPyramid ftp(6, 6, 6, 2, 2, 2, 5);
    ftp.printInfo();

    return 0;
}