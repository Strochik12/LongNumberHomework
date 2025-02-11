//#include "LongNumber.hpp"
#include <iostream>
#include <time.h>
#include <iomanip>

#include "LongNumber2.cpp"


int main() {
    unsigned int precision;
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

    std::string realpi = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";
    std::cout << realpi << "\n";
    std::cout << pi.toDecimal().substr(0, realpi.length()) << "\n";
    std::cout << "time: " << std::setprecision(15) << static_cast<long double>(finish - start)/CLOCKS_PER_SEC << " sec\n";
    //pi.test();
}