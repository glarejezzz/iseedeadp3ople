#pragma once
#include <concepts>
#include <type_traits>
#include <utility>
#include <cmath>

template <typename Function>
concept function_of_x = std::invocable<Function, double> && 
                        std::convertible_to<std::invoke_result_t<Function, double>, double>;

template <typename Function>
double min_uniform_search(double a, double b, double eps, Function f) 
requires function_of_x<Function> {
    double xk = a;
    double step = eps / 2;
    double min_result = xk;
    for (; xk <= b; xk += step)
        min_result = (f(xk) < f(min_result)) ? xk : min_result;
    return min_result;
}

template <typename Function>
double min_local(double a, double b, double eps, Function f) 
requires function_of_x<Function> {
    if (b - a < 2 * eps) return (a + b) / 2.0;
    double xprev = a, fprev = f(xprev);
    double step = (b - a) / 10.0;
    double xcurrent = xprev + step, fcurrent = f(xcurrent);
    double xnext = xcurrent + step, fnext = f(xnext);
    while (fnext < fcurrent && xnext <= b) {
        xprev = xcurrent;
        xcurrent = std::exchange(xnext, xnext + step);
        fprev = std::exchange(fcurrent, fnext);
        fnext = f(xnext);
    }
    return min_local(xprev, std::min(xnext, b), eps, f);
}

template <typename Function>
double min_dihotomia(double a, double b, double eps, Function f) {
    double xmiddle = (b+a)  / 2.0;
    if((b+a) < 2 * eps) return xmiddle;
    double xleft = (xmiddle+a)  / 2.0;
    double xright = (b+xmiddle)  / 2.0;
    double fmiddle = f(xmiddle);
    double fleft = f(xleft);
    double fright = f(xright);
    while ((b - a) >= 2 * eps) {
        if(fleft < fmiddle){
            b = xmiddle;
            xmiddle = xleft;
            fmiddle = fleft;
            xright = (b-xmiddle) / 2.0;
            fright = f(xright);
            xleft = (xmiddle + a) /2.0;
            fleft = f(xleft);
        }else if (xmiddle < fright){
            a = xleft;
            b = xright;
            xleft = (xmiddle + a) / 2.0;
            fleft= f(xleft);
            xright = (b+xmiddle) / 2.0;
            fright = f(xright);
        }else {
            a = xmiddle;
            xmiddle = xright;
            fmiddle = fright;
            xright = (b + xmiddle) / 2.0;
            fright = f(xright);
            xleft = (xleft + a) / 2.0;
            fleft = f(xleft);

        }
        }
        return xmiddle;
    }

template <typename Function>
double min_golden_ration(double a, double b, double eps, Function f)
requires function_of_x<Function> {
    double y = a + (3 - std::sqrt(5)) / 2.0 * (b - a);
    double z = a + b - y;
    double fy = f(y), fz = f(z);
    while ((b - a) > eps) {
        if (fy < fz) {
            b = z;
            z = y;
            fz = fy;
            y = a + b - z;
            fy = f(y);
        } else {
            a = y;
            y = z;
            fy = fz;
            z = a + b - y;   
            fz = f(z);
        }
    }
    return fy < fz ? y : z;
}

template <typename Function> 
double min_fibonacci(double a, double b, double eps, Function f)
requires function_of_x<Function> {
    double fib[100];                    
    fib[0] = 1.0;
    fib[1] = 1.0;
    int count = 1;
    while (fib[count] < (b - a) / eps && count < 98) {
        count++;
        fib[count] = fib[count - 1] + fib[count - 2];
    }
    double x1 = a + fib[count - 2] / fib[count] * (b - a);
    double x2 = a + fib[count - 1] / fib[count] * (b - a);
    double f1 = f(x1);
    double f2 = f(x2);
    for (int i = 1; i < count - 1; i++) {
        if (f1 < f2) {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = a + fib[count - i - 2] / fib[count - i] * (b - a);
            f1 = f(x1);
        } else {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + fib[count - i - 1] / fib[count - i] * (b - a);
            f2 = f(x2);
        }
    }
    double result = (a + b) / 2.0;
    if (f(a) < f(result)) result = a;
    if (f(b) < f(result)) result = b;
    return result;
}