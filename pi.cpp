//#include "LongNumber.hpp"
#include <iostream>
#include <time.h>
#include <iomanip>

#include "LongNumber2.cpp"


int main() {
    unsigned int precision;
    std::cout << "precision: ";
    std::cin >> precision;
    clock_t start, finish;
    int K = precision;
    //precision = 333;


    start = clock();
    
    LongNum::Number pi(0, precision);
            
    LongNum::Number n(1, precision);
    LongNum::Number p(16, precision);
    
    ///*
    LongNum::Number a(4, precision);
    LongNum::Number b(2, precision);
    LongNum::Number c(1, precision);
    LongNum::Number d(1, precision);

    LongNum::Number a0(1);
    LongNum::Number b0(4);
    LongNum::Number c0(5);
    LongNum::Number d0(6);

    LongNum::Number ln8(8);
    //*/

    if (precision == 0) {
        pi = 3;
    }

    /*
    LongNum::Number uk2(960, precision);
    LongNum::Number uk1(376, precision);
    
    LongNum::Number dk4(4096, precision);
    LongNum::Number dk3(8192, precision);
    LongNum::Number dk2(5696, precision);
    LongNum::Number dk1(120, precision);

    LongNum::Number ln1552(120, precision);
    LongNum::Number ln1208(1208, precision);
    //*/

    for (ull k = 0; k < K; ++k) {
        ///*
        pi += n*(a / a0 - b / b0 - c / c0 - d / d0);
        n /= p;
        a0 += ln8;
        b0 += ln8;
        c0 += ln8;
        d0 += ln8;
        //*/
        /*
        LongNum::Number k2(k*k, precision), k3(k*k*k, precision), k4(k*k*k*k, precision);
        LongNum::Number u = uk2*k2 + uk1;
        LongNum::Number d = dk4*k4 + dk3*k3 + dk2*k2 + dk1;
        pi += n * (u / d);
        n /= p; 
        uk1 += ln1208;
        dk1 += ln1552;
        //*/
    }

    finish = clock();

    //realpi contains 200 decimal digits after the decimal point of pi
    std::string realpi = "3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038";
    std::cout << "actual pi:     " <<  realpi.substr(0, K + 2) << "\n";
    std::cout << "calculated pi: " << pi.toDecimal().substr(0, K + 2) << "\n";
    std::cout << "time: " << std::setprecision(15) << static_cast<long double>(finish - start)/CLOCKS_PER_SEC << " sec\n";
    //pi.test();
}