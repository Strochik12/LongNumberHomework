#include "LongNumber.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <time.h>
#include <iomanip>

namespace LongNum {
    unsigned Number::FIXED_PRECISION = 11;

    void Number::change_precision(unsigned new_precision) {
        FIXED_PRECISION = new_precision / BASE_SZ + (new_precision % BASE_SZ > 0);
    }

    Number::Number() {
        digits.push_back(0);
    }

    Number::Number(int number, unsigned precision_) {
        if (number < 0) {
            negative = true;
            number = -number;
        }
        fraction_len = precision_ / BASE_SZ + (precision_ % BASE_SZ > 0);
        digits.resize(fraction_len);
        digits.push_back(number);
    }
    
    //number is binary only
    Number::Number(std::string number) {
        if (number[0] == '-') {
            negative = true;
            number.erase(number.begin());
        }
        size_t dot_pos = number.find('.');
        std::string integer_part = number.substr(0, dot_pos);
        std::string fraction_part = (dot_pos == std::string::npos) ? "0" : number.substr(dot_pos + 1);
        while (fraction_part.size() > 1 && fraction_part.back() == '0')
            fraction_part.pop_back();
        while (integer_part.size() > 1 && integer_part[0] == '0')
            integer_part.erase(integer_part.begin());
        //--fraction-------------------------
        while (!fraction_part.empty()) {
            digits.push_back(0);
            for (int i = 0; i < BASE_SZ; ++i) {
                int digit = (fraction_part.empty() ? 0 : fraction_part[0] - '0');
                digits.back() *= 2;
                digits.back() += digit;
                if (!fraction_part.empty()) fraction_part.erase(fraction_part.begin());
            }
        }
        std::reverse(digits.begin(), digits.end());
        fraction_len = digits.size();
        //-------------------------fraction--

        //--integer--------------------------
        while (!integer_part.empty()) {
            digits.push_back(0);
            for (ull pow2 = 1; pow2 < BASE; pow2 *= 2) {
                int digit = (integer_part.empty() ? 0 : integer_part.back() - '0');
                digits.back() += pow2 * digit;
                if (!integer_part.empty()) integer_part.pop_back();
            }
        }
        //--------------------------integer--
    }

    std::string Number::toDecimal() const {
        std::string number = str();
        std::string sign = "";
        if (number[0] == '-') {
            sign = "-";
            number.erase(number.begin());
        }
        size_t dot_pos = number.find('.');
        std::string int_part = number.substr(0, dot_pos);
        std::string frac_part = (dot_pos == std::string::npos) ? "0" : number.substr(dot_pos + 1);
        while (frac_part.size() > 1 && frac_part.back() == '0')
            frac_part.pop_back();
        while (int_part.size() > 1 && int_part[0] == '0')
            int_part.erase(int_part.begin());

        std::string decimal_int = "0";
        for (char bit: int_part) {
            decimal_int = multiplyDecimalStringBy2(decimal_int);
            if (bit == '1')
                decimal_int = addDecimalStrings(decimal_int, "1");
        }
        
        std::string decimal_frac = "0";
        std::string rev_pow2 = "0.5";
        for (char bit: frac_part) {
            if (bit == '1')
                decimal_frac = addDecimalStrings(decimal_frac, rev_pow2);
            rev_pow2 = deepDivideDecimalStringBy2(rev_pow2);
        }
        return sign + addDecimalStrings(decimal_int, decimal_frac);
    }

    std::string Number::str() const {
        if (BASE == 1e9) {
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
        } else {
            std::string frac_part = "", int_part = "";
            for (int i = 0; i < digits.size(); ++i) {
                ull digit = digits[i];
                for (int j = 0; j < BASE_SZ; ++j) {
                    if (i < fraction_len) frac_part += (digit % 2 + '0');
                    else int_part += (digit % 2 + '0');
                    digit /= 2;
                }
            }
            if (frac_part.empty()) frac_part = "0";
            std::reverse(frac_part.begin(), frac_part.end());
            std::reverse(int_part.begin(), int_part.end());
            while (int_part.size() > 1 && int_part[0] == '0')
                int_part.erase(int_part.begin());
            while (frac_part.size() > 1 && frac_part.back() == '0')
                frac_part.pop_back();
            return (negative ? "-" : "") + int_part + '.' + (frac_part.empty() ? "0" : frac_part);
        }
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
        for (int i = 0; i < a.digits.size(); ++i) {
            ull carry = 0;
            for (int j = 0; j < b.digits.size(); ++j) {
                carry += result.digits[i + j] + a.digits[i] * b.digits[j];
                result.digits[i + j] = carry % result.BASE;
                carry /= result.BASE;
            }
            result.digits[i + b.digits.size()] += carry;
        }
        while (result.fraction_len > result.FIXED_PRECISION) {
            result.digits.erase(result.digits.begin());
            --result.fraction_len;
        }
        result.deleteZeros();
        return result;
    }

    Number operator/(const Number &a, const Number &b) {
        if (b.isZero()) throw std::overflow_error("division by 0");
        if (a.isZero()) return 0_LN;
        Number result;
        result.fraction_len = result.FIXED_PRECISION;

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





    std::string toBinary(std::string number, unsigned precision) {
        std::string sign = "";
        if (number[0] == '-') {
            sign = "-";
            number.erase(number.begin());
        }
        size_t dot_pos = number.find('.');
        std::string integer_part = number.substr(0, dot_pos);
        std::string fraction_part = (dot_pos == std::string::npos) ? "0" : number.substr(dot_pos + 1);
        while (fraction_part.size() > 1 && fraction_part.back() == '0')
            fraction_part.pop_back();
        while (integer_part.size() > 1 && integer_part[0] == '0')
            integer_part.erase(integer_part.begin());

        //--integer--------------------------
        std::string bin_int = "";
        while (integer_part != "0") {
            int digit = (integer_part.back() - '0') % 2;
            bin_int += (digit ? '1' : '0');
            integer_part = divideDecimalStringBy2(integer_part);
        }
        std::reverse(bin_int.begin(), bin_int.end());
        if (bin_int.empty()) bin_int = "0";
        //--------------------------integer--

        //--fraction-------------------------
        std::string bin_frac = "";
        int sz = fraction_part.length();
        while (!fraction_part.empty() && (unsigned)bin_frac.length() < precision) {
            fraction_part = multiplyDecimalStringBy2(fraction_part);
            if (fraction_part.length() > sz) {
                bin_frac += '1';
                fraction_part[0] -= 5;
                fraction_part.erase(fraction_part.begin());
            } else {
                bin_frac += '0';
            }
        }
        //-------------------------fraction--

        return sign + bin_int + (bin_frac.empty() ? "" : "." + bin_frac);
    }

    std::string multiplyDecimalStringBy2(std::string number) {
        std::string multiplied = "";
        int carry = 0;
        for (int i = number.length() - 1; i >= 0; --i) {
            char digit = number[i];
            int cur = (digit - '0') * 2 + carry;
            multiplied += (cur % 10) + '0';
            carry = cur / 10;
        }
        if (carry > 0) multiplied += (carry + '0');
        std::reverse(multiplied.begin(), multiplied.end());
        return multiplied;
    }

    std::string divideDecimalStringBy2(std::string number) {
        std::string divided = "";
        int carry = 0;
        for (char digit: number) {
            int cur = carry * 10 + (digit - '0');
            divided += (cur / 2) + '0';
            carry = cur % 2;
        }
        while (divided.size() > 1 && divided[0] == '0') {
                divided.erase(divided.begin());
        }
        return divided;
    }

    //only works with number < 1
    std::string deepDivideDecimalStringBy2(std::string number) {
        std::string divided = "0.";
        int carry = 0;
        for (int i = 2; i < number.length(); ++i) {
            char digit = number[i];
            int cur = carry * 10 + (digit - '0');
            divided += (cur / 2) + '0';
            carry = cur % 2;
        }
        if (carry) divided += '5';
        return divided;
    }

    std::string addDecimalStrings(std::string a, std::string b) {
        std::reverse(a.begin(), a.end());
        std::reverse(b.begin(), b.end());
        int ap = a.find('.'), bp = b.find('.');
        if (ap == std::string::npos) {
            a = '.' + a;
            ap = 0;
        }
        if (bp == std::string::npos) {
            b = '.' + b;
            bp = 0;
        }

        int len = std::max(ap, bp) + std::max(a.length() - ap, b.length() - bp);
        int ai = ap - std::max(ap, bp), bi = bp - std::max(ap, bp);
        int carry = 0;
        std::string result = "";
        for (int i = 0; i < len; ++i) {
            if (i + ai >= 0 && a[i + ai] == '.') {
                result += '.';
                continue;
            }
            int cur = carry;
            if (i + ai >= 0 && i + ai < a.length()) cur += (a[i + ai] - '0');
            if (i + bi >= 0 && i + bi < b.length()) cur += (b[i + bi] - '0');
            result += (cur % 10) + '0';
            carry = cur / 10;
        }
        if (carry) result += (carry + '0');
        std::reverse(result.begin(), result.end());
        while (result.back() == '0' || result.back() == '.')
            result.pop_back();
        return result;
    }
}

LongNum::Number operator ""_LN(unsigned long long n) {
    return LongNum::Number(n);
}

LongNum::Number operator ""_LN(long double n) {
    return LongNum::Number(LongNum::toBinary(std::to_string(n), 100));
}
