#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath>

class Triangle {
protected:
    double sideA, sideB, sideC;

    bool isValidTriangle(double a, double b, double c) const {
        return (a > 0 && b > 0 && c > 0) &&
               (a + b > c) && (a + c > b) && (b + c > a);
    }

    void validateTriangle(double a, double b, double c) const {
        if (!isValidTriangle(a, b, c)) {
            std::cerr << "Error: Invalid triangle sides\n";
            exit(1);
        }
    }

public:
    Triangle(double a, double b, double c) {
        validateTriangle(a, b, c);
        sideA = a;
        sideB = b;
        sideC = c;
    }

    double area() const {
        double p = (sideA + sideB + sideC) / 2;
        return sqrt(p * (p - sideA) * (p - sideB) * (p - sideC));
    }

    double perimeter() const {
        return sideA + sideB + sideC;
    }

    void printInfo() const {
        std::cout << "Triangle with sides: " << sideA << ", " << sideB 
                  << ", " << sideC << "\nArea: " << area() 
                  << "\nPerimeter: " << perimeter() << "\n";
    }
};

class TriangularPyramid : public Triangle {
private:
    double pyramidHeight;

    void validateHeight(double h) const {
        if (h <= 0) {
            std::cerr << "Error: Height must be positive\n";
            exit(1);
        }
    }

public:
    TriangularPyramid(double a, double b, double c, double h) : Triangle(a, b, c) {
        validateHeight(h);
        pyramidHeight = h;
    }

    double volume() const {
        return (area() * pyramidHeight) / 3;
    }

    void printInfo() const {
        std::cout << "Triangular pyramid with base sides: " << sideA << ", " 
                  << sideB << ", " << sideC << "\nHeight: " << pyramidHeight 
                  << "\nBase area: " << area() << "\nVolume: " << volume() << "\n";
    }
};

class FrustumTriangularPyramid : public TriangularPyramid {
private:
    double topSideA, topSideB, topSideC;
    double frustumHeight;

public:
    FrustumTriangularPyramid(double a1, double b1, double c1, 
                           double a2, double b2, double c2,
                           double h) : TriangularPyramid(a1, b1, c1, h) {
        if (!isValidTriangle(a2, b2, c2)) {
            std::cerr << "Error: Invalid top triangle sides\n";
            exit(1);
        }
        topSideA = a2;
        topSideB = b2;
        topSideC = c2;
        frustumHeight = h;
    }

    double topArea() const {
        Triangle top(topSideA, topSideB, topSideC);
        return top.area();
    }

    double volume() const {
        double base = area();
        double top = topArea();
        return (frustumHeight / 3) * (base + top + sqrt(base * top));
    }

    void printInfo() const {
        std::cout << "Frustum of triangular pyramid:\n"
                  << "Base sides: " << sideA << ", " << sideB << ", " << sideC 
                  << "\nTop sides: " << topSideA << ", " << topSideB << ", " << topSideC
                  << "\nHeight: " << frustumHeight 
                  << "\nBase area: " << area()
                  << "\nTop area: " << topArea()
                  << "\nVolume: " << volume() << "\n";
    }
};

#endif