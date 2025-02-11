#include "LongNumber.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <time.h>
#include <iomanip>

namespace LongNum {
    Number::Number() {
        digits.push_back(0);
    }
    Number::Number(int number, unsigned precision = 0) {
        if (number < 0) {
            negative = true;
            number = -number;
        }
        fraction_len = precision / BASE_SZ + (precision % BASE_SZ > 0);
        digits.resize(fraction_len);
        digits.push_back(number);
    }
    
    Number::Number(std::string number) {
        if (number[0] == '-') {
            negative = true;
            number.erase(number.begin());
        }
        size_t point = number.find('.');
        if (point != std::string::npos) {
            unsigned zeros_to_add = BASE_SZ - (number.length() - point - 1) % BASE_SZ;
            for (int i = 0; i < zeros_to_add; ++i) number += "0";
        }
        ull d = 1, n = 0;
        for (auto i = number.end(); i != number.begin();) {
            --i;
            if (*i == '.' || *i == ',') {
                if (d != 1) {
                    digits.push_back(n * (BASE / d));
                    n = 0;
                    d = 1;
                }
                fraction_len = digits.size();
                //precision = fraction_len;
                continue;
            }
            n += (*i - '0') * d;
            d *= 10;
            if (d == BASE) {
                digits.push_back(n);
                n = 0;
                d = 1;
            }
        }
        if (d != 1) {
            digits.push_back(n);
        }
        deleteZeros();
    }

    bool Number::isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }

    void Number::deleteZeros() {
        while (digits.front() == 0 && fraction_len > 0) {
            --fraction_len;
            digits.erase(digits.begin());
        }
        while (digits.back() == 0 && digits.size() > fraction_len + 1) {
            digits.pop_back();
        }
    }

    std::string Number::str() const {
        std::string result;
        if (fraction_len == digits.size()) result += '.';
        for (int i = digits.size() - 1; i >= 0; --i) {
            int cur = digits[i];
            std::string buf;
            while (cur > 0) {
                buf += static_cast<char>(cur % 10 + '0');
                cur /= 10;
            }
            while (buf.length() != BASE_SZ)
                buf += '0';
            reverse(buf.begin(), buf.end());
            result += buf;
            if (i == fraction_len) result += '.';
        }
        while (result.back() == '0')
            result.pop_back();
        while (result[0] == '0' && result[1] != '.')
            result.erase(result.begin());
        if (negative) result = '-' + result;
        return result;
    }

    std::ostream &operator<<(std::ostream &os, const Number &n) {
        return os << n.str();
    }


    Number Number::operator-() const {
        Number inv(*this);
        inv.negative = !negative;
        return inv;
    }

    bool operator==(const Number &a, const Number &b) {
        if (a.negative != b.negative || a.digits.size() != b.digits.size() || a.fraction_len != b.fraction_len)
            return false;
        for (int i = 0; i < a.digits.size(); ++i)
            if (a.digits[i] != b.digits[i])
                return false;
        return true;
    }

    bool operator<(const Number &a, const Number &b) {
        if (a.negative != b.negative) 
            return a.negative;
        if (a.negative)
            return -b < -a;
        int a_int_len = a.digits.size() - a.fraction_len, b_int_len = b.digits.size() - b.fraction_len;
        if (a_int_len != b_int_len)
            return a_int_len < b_int_len;
        for (auto i = a.digits.end(), j = b.digits.end(); i != a.digits.begin() && j != b.digits.begin();) {
            --i; --j;
            if (*i != *j)
                return *i < *j;
        }
        return a.fraction_len < b.fraction_len;
    }

    bool operator!=(const Number &a, const Number &b) {
        return !(a == b);
    }

    bool operator>(const Number &a, const Number &b) {
        return b < a;
    }

    bool operator<=(const Number &a, const Number &b) {
        return a < b || a == b;
    }

    bool operator>=(const Number &a, const Number &b) {
        return b < a || a == b;
    }
    
    Number operator+(const Number &a, const Number &b) {
        if (a.negative != b.negative) {
            if (a.negative) return b - (-a);
            return a - (-b);
        }
        Number result;
        result.negative = a.negative;
        result.fraction_len = std::max(a.fraction_len, b.fraction_len);
        //result.precision = result.fraction_len;
        unsigned len = result.fraction_len + std::max(a.digits.size() - a.fraction_len, b.digits.size() - b.fraction_len);
        result.digits.resize(len + 1);

        unsigned ai = a.fraction_len - result.fraction_len, bi = b.fraction_len - result.fraction_len;
        for (int i = 0; i < len; ++i) {
            if (i + ai >= 0 && i + ai < a.digits.size()) result.digits[i] += a.digits[i + ai];
            if (i + bi >= 0 && i + bi < b.digits.size()) result.digits[i] += b.digits[i + bi];
            result.digits[i + 1] = result.digits[i] / result.BASE;
            result.digits[i] %= result.BASE;
        }
        result.deleteZeros();
        return result;
    }
    
    Number operator-(const Number &a, const Number &b) {
        if (b.negative) return a + (-b);
        if (a.negative) return -(-a + b);
        if (a < b) return -(b - a);

        Number result;
        result.fraction_len = std::max(a.fraction_len, b.fraction_len);
        //result.precision = result.fraction_len;
        unsigned len = result.fraction_len + std::max(a.digits.size() - a.fraction_len, b.digits.size() - b.fraction_len);
        result.digits.resize(len);

        unsigned ai = a.fraction_len - result.fraction_len, bi = b.fraction_len - result.fraction_len;
        bool used = false;
        for (int i = 0; i < len; ++i) {
            long long cura = 0, curb = 0;
            if (i + ai >= 0 && i + ai < a.digits.size()) cura = a.digits[i + ai];
            if (i + bi >= 0 && i + bi < b.digits.size()) curb = b.digits[i + bi];
            if (used) --cura;
            used = false;
            if (cura < curb) {
                cura += a.BASE;
                used = true;
            }
            result.digits[i] = cura - curb;
        }
        result.deleteZeros();
        return result;
    }

    Number operator*(const Number &a, const Number &b) {
        if (a.isZero() || b.isZero()) return 0_LN;
        Number result;
        result.negative = (a.negative != b.negative);
        result.fraction_len = a.fraction_len + b.fraction_len;
        result.digits.resize(result.fraction_len + a.digits.size() - a.fraction_len + b.digits.size() - b.fraction_len);
        //result.precision = result.fraction_len;
        for (int i = 0; i < a.digits.size(); ++i) {
            ull carry = 0;
            for (int j = 0; j < b.digits.size(); ++j) {
                carry += result.digits[i + j] + a.digits[i] * b.digits[j];
                result.digits[i + j] = carry % result.BASE;
                carry /= result.BASE;
            }
            result.digits[i + b.digits.size()] += carry;
        }
        ///*
        while (result.fraction_len > result.FIXED_PRECISION) {
            result.digits.erase(result.digits.begin());
            --result.fraction_len;
        }
        //*/
        result.deleteZeros();
        return result;
    }

    Number operator/(const Number &a, const Number &b) {
        if (b.isZero()) throw std::overflow_error("division by 0");
        if (a.isZero()) return 0_LN;
        Number result;
        //result.fraction_len = std::max(a.fraction_len, b.fraction_len);
        result.fraction_len = result.FIXED_PRECISION;
        //result.precision = result.fraction_len;

        Number max_number = Number(result);
        max_number.digits.resize(max_number.fraction_len + 1, max_number.BASE - 1);
        Number abs_a = Number(a);
        abs_a.negative = false;
        while (max_number * b < abs_a) {
            max_number.digits.push_back(max_number.BASE - 1);
        }
        result.digits.resize(max_number.digits.size(), 0);
        for (int i = result.digits.size() - 1; i >= 0; --i) {
            ull l = 0, r = result.BASE;
            bool solved = false;
            //Number local_res;
            while (r - l > 1) {
                result.digits[i] = (l + r) / 2;
                Number local_res = result * b;
                if (local_res < abs_a)
                    l = result.digits[i];
                else if (local_res > abs_a)
                    r = result.digits[i];
                else {
                    solved = true;
                    break;
                }
            }
            if (solved) break;
            result.digits[i] = l;
        }
        result.negative = (a.negative != b.negative);
        result.deleteZeros();
        return result;
    }

    Number& Number::operator+=(const Number &other) {
        return *this = *this + other;
    }

    Number& Number::operator-=(const Number &other) {
        return *this = *this - other;
    }

    Number& Number::operator*=(const Number &other) {
        return *this = *this * other;
    }

    Number& Number::operator/=(const Number &other) {
        return *this = *this / other;
    }

    void Number::test() {
        std::cout << "size: " << digits.size() << "\n";
        std::cout << "fraction_len: " << fraction_len << "\n";
        for (int i = 0; i < digits.size(); ++i) {
            std::cout << digits[i] << " ";
        }
        std::cout << "\n" << *this << "\n";
    }

}

LongNum::Number operator ""_LN(unsigned long long n) {
    return LongNum::Number(n);
}

LongNum::Number operator ""_LN(long double n) {
    return LongNum::Number(std::to_string(n));
}



/*
int main() {
    LongNum::Number x("123.456789123456789");
    LongNum::Number y("123456789123456.789");
    LongNum::Number ZV("987654321.987654321");
    clock_t start, finish;
    start = clock();
    for (int i = 0; i < 100; ++i) {
        LongNum::Number f = x * (y / ZV + x / ZV + x / y + y / x);
    }
    finish = clock();
    std::cout << "time: " << std::setprecision(15) << static_cast<long double>(finish - start)/CLOCKS_PER_SEC << " sec";
}
//*/