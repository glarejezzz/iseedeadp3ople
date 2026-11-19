#pragma once
#include <cmath>

double foo9(double x) { return std::cos(x); } 
double foo13(double x) { return std::pow(x,5) - 5*std::pow(x,3); }
double foo17(double x) { return std::pow(x,3) - 9*x; }
double nfoo17(double x) { return -foo17(x);}
double foo19(double x) { return std::pow(x,3) + 9*x; }
double foo32(double x) { return std::cos(x) * (x*x -5); }
double foo51(double x) { return (3*x*x + 2*x)* std::atan(x); }