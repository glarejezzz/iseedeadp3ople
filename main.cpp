#include <iostream>
#include <iomanip>
#include <numbers>
#include <cmath>
#include "functions.h"
#include "plain_methods.h"

int main() {
    std::cout << std::fixed << std::setprecision(8);

    const double a9  = std::numbers::pi / 2,  b9  = 3 * std::numbers::pi / 2;
    const double a13 = 0.0,  b13 = 4.0;
    const double a17 = -3.0, b17 = 0.0;
    const double a19 = -1.0, b19 = 1.0;
    const double a32 = -3.0, b32 = -1.0;
    const double a51 = -0.5, b51 = 2.0;

    std::cout << "Uniform:\n";
    double x9  = min_uniform_search(a9,  b9,  1e-6, foo9);
    double x13 = min_uniform_search(a13, b13, 1e-6, foo13);
    double x17 = min_uniform_search(a17, b17, 1e-6, nfoo17);   
    double x19 = min_uniform_search(a19, b19, 1e-6, foo19);
    double x32 = min_uniform_search(a32, b32, 1e-6, foo32);
    double x51 = min_uniform_search(a51, b51, 1e-6, foo51);
    std::cout << "foo9:  x = " << x9  << " foo(x) = " << foo9(x9)   << '\n';
    std::cout << "foo13: x = " << x13 << " foo(x) = " << foo13(x13) << '\n';
    std::cout << "foo17: x = " << x17 << " foo(x) = " << foo17(x17) << '\n';
    std::cout << "foo19: x = " << x19 << " foo(x) = " << foo19(x19) << '\n';
    std::cout << "foo32: x = " << x32 << " foo(x) = " << foo32(x32) << '\n';
    std::cout << "foo51: x = " << x51 << " foo(x) = " << foo51(x51) << '\n';

    std::cout << "Localization:\n";
    x9  = min_local(a9,  b9,  1e-6, foo9);
    x13 = min_local(a13, b13, 1e-6, foo13);
    x17 = min_local(a17, b17, 1e-6, nfoo17);
    x19 = min_local(a19, b19, 1e-6, foo19);
    x32 = min_local(a32, b32, 1e-6, foo32);
    x51 = min_local(a51, b51, 1e-6, foo51);
    std::cout << "foo9:  x = " << x9  << " foo(x) = " << foo9(x9)   << '\n';
    std::cout << "foo13: x = " << x13 << " foo(x) = " << foo13(x13) << '\n';
    std::cout << "foo17: x = " << x17 << " foo(x) = " << foo17(x17) << '\n';
    std::cout << "foo19: x = " << x19 << " foo(x) = " << foo19(x19) << '\n';
    std::cout << "foo32: x = " << x32 << " foo(x) = " << foo32(x32) << '\n';
    std::cout << "foo51: x = " << x51 << " foo(x) = " << foo51(x51) << '\n';

    std::cout << "Dihotomia:\n";
    x9  = min_dihotomia(a9,  b9,  1e-6, foo9);
    x13 = min_dihotomia(a13, b13, 1e-6, foo13);
    x17 = min_dihotomia(a17, b17, 1e-6, nfoo17);
    x19 = min_dihotomia(a19, b19, 1e-6, foo19);
    x32 = min_dihotomia(a32, b32, 1e-6, foo32);
    x51 = min_dihotomia(a51, b51, 1e-6, foo51);
    std::cout << "foo9:  x = " << x9  << " foo(x) = " << foo9(x9)   << '\n';
    std::cout << "foo13: x = " << x13 << " foo(x) = " << foo13(x13) << '\n';
    std::cout << "foo17: x = " << x17 << " foo(x) = " << foo17(x17) << '\n';
    std::cout << "foo19: x = " << x19 << " foo(x) = " << foo19(x19) << '\n';
    std::cout << "foo32: x = " << x32 << " foo(x) = " << foo32(x32) << '\n';
    std::cout << "foo51: x = " << x51 << " foo(x) = " << foo51(x51) << '\n';

    std::cout << "Golden ration:\n";
    x9  = min_golden_ration(a9,  b9,  1e-6, foo9);
    x13 = min_golden_ration(a13, b13, 1e-6, foo13);
    x17 = min_golden_ration(a17, b17, 1e-6, nfoo17);
    x19 = min_golden_ration(a19, b19, 1e-6, foo19);
    x32 = min_golden_ration(a32, b32, 1e-6, foo32);
    x51 = min_golden_ration(a51, b51, 1e-6, foo51);
    std::cout << "foo9:  x = " << x9  << " foo(x) = " << foo9(x9)   << '\n';
    std::cout << "foo13: x = " << x13 << " foo(x) = " << foo13(x13) << '\n';
    std::cout << "foo17: x = " << x17 << " foo(x) = " << foo17(x17) << '\n';
    std::cout << "foo19: x = " << x19 << " foo(x) = " << foo19(x19) << '\n';
    std::cout << "foo32: x = " << x32 << " foo(x) = " << foo32(x32) << '\n';
    std::cout << "foo51: x = " << x51 << " foo(x) = " << foo51(x51) << '\n';

    std::cout << "Fibonacci:\n";
    x9  = min_fibonacci(a9,  b9,  1e-6, foo9);
    x13 = min_fibonacci(a13, b13, 1e-6, foo13);
    x17 = min_fibonacci(a17, b17, 1e-6, nfoo17);
    x19 = min_fibonacci(a19, b19, 1e-6, foo19);
    x32 = min_fibonacci(a32, b32, 1e-6, foo32);
    x51 = min_fibonacci(a51, b51, 1e-6, foo51);
    std::cout << "foo9:  x = " << x9  << " foo(x) = " << foo9(x9)   << '\n';
    std::cout << "foo13: x = " << x13 << " foo(x) = " << foo13(x13) << '\n';
    std::cout << "foo17: x = " << x17 << " foo(x) = " << foo17(x17) << '\n';
    std::cout << "foo19: x = " << x19 << " foo(x) = " << foo19(x19) << '\n';
    std::cout << "foo32: x = " << x32 << " foo(x) = " << foo32(x32) << '\n';
    std::cout << "foo51: x = " << x51 << " foo(x) = " << foo51(x51) << '\n';

    return 0;
}





