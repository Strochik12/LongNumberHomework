#include "LongNumber.hpp"

#include <iostream>
#include <time.h>
#include <math.h>

const long double ln_approximation = log(10) / log(2);

int main() {
    unsigned int precision;
    std::cout << "precision: ";
    std::cin >> precision;
    clock_t start, finish;

    LongNum::Number pi(0, precision);

    unsigned bin_precision = int((long double)precision * ln_approximation) + 1;
    pi.change_precision(bin_precision);
            
    LongNum::Number n(1, precision);
    LongNum::Number p(1, precision);
    p /= 16;
    LongNum::Number a(4, precision);
    LongNum::Number b(2, precision);
    LongNum::Number c(1, precision);
    LongNum::Number d(1, precision);
    LongNum::Number a0(1);
    LongNum::Number b0(4);
    LongNum::Number c0(5);
    LongNum::Number d0(6);
    LongNum::Number ln8(8);

    start = clock();

    if (precision == 0) {
        pi = 3_LN;
    }

    for (ull k = 0; k < precision; ++k) {
        pi += n*(a / a0 - b / b0 - c / c0 - d / d0);
        n *= p;
        a0 += ln8;
        b0 += ln8;
        c0 += ln8;
        d0 += ln8;
    }

    finish = clock();

    //realpi contains 200 decimal digits after the decimal point of pi
    std::string realpi = "3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196";
    std::cout << "actual pi:     " <<  realpi.substr(0, precision + 2) << "\n";
    std::cout << "calculated pi: " << pi.toDecimal().substr(0, precision + 2) << "\n";
    if (precision > 200)
        std::cout << "'actual pi:' contains only 200 decimal digits after the decimal point of pi :(\n";
    std::cout << "time: " << static_cast<long double>(finish - start)/CLOCKS_PER_SEC << " sec\n";
}