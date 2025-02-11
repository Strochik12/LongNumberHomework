#ifndef LONG_NUMBER_HPP
#define LONG_NUMBER_HPP

#include <vector>
#include <string>

typedef unsigned long long ull;

namespace LongNum {
    class Number {
    private:
        static const ull BASE = ((ull)1<<31); // ((ull)1<<31)
        static const unsigned BASE_SZ = 31; // 31
        static const unsigned FIXED_PRECISION = 11; // 10^100/BASE
        std::vector<ull> digits;
        bool negative = false;
        unsigned fraction_len = 0;

        bool isZero() const;

        void deleteZeros();
    public:
        void test();

        Number();

        Number(int, unsigned);

        explicit Number(std::string);

        std::string str() const;
        std::string toDecimal() const;

        friend std::ostream &operator<<(std::ostream &, const Number &);

        Number operator-() const;

        friend bool operator==(const Number &, const Number &);

        friend bool operator!=(const Number &, const Number &);

        friend bool operator<(const Number &, const Number &);

        friend bool operator>(const Number &, const Number &);
        
        friend bool operator<=(const Number &, const Number &);

        friend bool operator>=(const Number &, const Number &);

        friend Number operator+(const Number &, const Number &);

        friend Number operator-(const Number &, const Number &);

        friend Number operator*(const Number &, const Number &);

        friend Number operator/(const Number &, const Number &);
        
        Number &operator+=(const Number &);

        Number &operator-=(const Number &);
        
        Number &operator*=(const Number &);
        
        Number &operator/=(const Number &);
    };
    std::string multiplyDecimalStringBy2(std::string);

    std::string addDecimalStrings(std::string, std::string);

    std::string divideDecimalStringBy2(std::string);

    std::string deepDivideDecimalStringBy2(std::string);

    std::string toBinary(std::string, unsigned);
}

LongNum::Number operator ""_LN(ull);

LongNum::Number operator ""_LN(long double);

#endif