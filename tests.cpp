#include "LongNumber.hpp"
#include "tests.hpp"

#include <iostream>
#include <vector>

Test::Test(std::string _name, std::string _a, std::string _b, std::string _operation, std::string _expected_result):
    name(_name), a(_a), b(_b), operation(_operation), expected_result(_expected_result) {}

std::string Test::getName() const {
    return name;
}

void Test::show() const {
    std::cout << name << ":\n";
    std::cout << "    " << a << "\n";
    std::cout << "    " << operation << "\n";
    if (b != "") std::cout << "    " << b << "\n";
    std::cout << "    " << "  expected result: " << expected_result << "\n";
}

bool Test::compare(std::string result) const {
    return result == expected_result;
}

std::string Test::test() const {
    if (operation == "operator ''_ln") {
        if (name == "TEST10")
            return (34.0151239_LN).str().substr(0, expected_result.length());
    }
    LongNum::Number A(a);
    LongNum::Number B(b);
    if (operation == "+")
        return (A + B).str().substr(0, expected_result.length());
    if (operation == "-")
        return (A - B).str().substr(0, expected_result.length());
    if (operation == "*")
        return (A * B).str().substr(0, expected_result.length());
    if (operation == "/" || operation == ":")
        return (A / B).str().substr(0, expected_result.length());
    if (operation == "<")
        return (A < B ? "True" : "False");
    if (operation == "<=")
        return (A <= B ? "True" : "False");
    if (operation == ">")
        return (A > B ? "True" : "False");
    if (operation == ">=")
        return (A >= B ? "True" : "False");
    if (operation == "=" || operation == "==")
        return (A == B ? "True" : "False");
    return "Unknown operation";
}



void test_all() {
    int ok_cnt = 0;
    for (int i = 0; i < TESTS.size(); ++i) {
        std::string res = TESTS[i].test();
        std::cout << TESTS[i].getName() << ": ";
        if (TESTS[i].compare(res)) {
            ++ok_cnt;
            std::cout << "OK\n";
        } else {
            std::cout << "FAIL\n";
        }
    }
    if (ok_cnt == TESTS.size()) {
        std::cout << "All tests passed\n";
    } else {
        std::cout << ok_cnt << "/" << TESTS.size() << " tests passed\n";
    }
}

void test_one(int n) {
    TESTS[n - 1].show();
    std::string res = TESTS[n - 1].test();
    std::cout << "    calculated result: " << res << "\n";
    if (TESTS[n - 1].compare(res))
        std::cout << "    OK\n";
    else 
        std::cout << "FAIL\n";
}

int main() {
    std::string line;
    while (getline(std::cin, line)) {
        size_t sep = line.find(' ');
        std::string command1 = line.substr(0, sep);
        if (command1 == "test") {
            std::string command2 = (sep == std::string::npos ? "" : line.substr(sep + 1));
            if (command2 == "all")
                test_all();
            else {
                int n = std::stoi(command2);
                if (n <= TESTS.size())
                    test_one(n);
                else
                    std::cout << "No such test found\n";
            }
        } else if (line == "exit") {
            break;
        } else {
            std::cout << "Unknown command\n";
        }
    }
}