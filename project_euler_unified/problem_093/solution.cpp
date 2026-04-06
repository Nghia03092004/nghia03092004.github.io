#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

struct Rational {
    long long num;
    long long den;

    Rational(long long numerator = 0, long long denominator = 1) : num(numerator), den(denominator) {
        normalize();
    }

    void normalize() {
        if (den < 0) {
            num = -num;
            den = -den;
        }
        const long long g = std::gcd(num >= 0 ? num : -num, den);
        if (g != 0) {
            num /= g;
            den /= g;
        }
    }
};

bool operator<(const Rational& a, const Rational& b) {
    return static_cast<__int128>(a.num) * b.den < static_cast<__int128>(b.num) * a.den;
}

bool operator==(const Rational& a, const Rational& b) {
    return a.num == b.num && a.den == b.den;
}

Rational operator+(const Rational& a, const Rational& b) {
    return Rational(a.num * b.den + b.num * a.den, a.den * b.den);
}

Rational operator-(const Rational& a, const Rational& b) {
    return Rational(a.num * b.den - b.num * a.den, a.den * b.den);
}

Rational operator*(const Rational& a, const Rational& b) {
    return Rational(a.num * b.num, a.den * b.den);
}

Rational operator/(const Rational& a, const Rational& b) {
    return Rational(a.num * b.den, a.den * b.num);
}

std::vector<Rational> next_values(const Rational& a, const Rational& b) {
    std::vector<Rational> out = {a + b, a - b, b - a, a * b};
    if (b.num != 0) {
        out.push_back(a / b);
    }
    if (a.num != 0) {
        out.push_back(b / a);
    }
    return out;
}

void collect_values(const std::vector<Rational>& values, std::set<Rational>& out) {
    if (values.size() == 1) {
        out.insert(values[0]);
        return;
    }

    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(values.size()); ++j) {
            const Rational& a = values[i];
            const Rational& b = values[j];

            std::vector<Rational> rest;
            rest.reserve(values.size() - 1);
            for (int k = 0; k < static_cast<int>(values.size()); ++k) {
                if (k != i && k != j) {
                    rest.push_back(values[k]);
                }
            }

            for (const Rational& value : next_values(a, b)) {
                rest.push_back(value);
                collect_values(rest, out);
                rest.pop_back();
            }
        }
    }
}

std::string solve() {
    int best_len = 0;
    std::string best_digits;

    for (int a = 1; a <= 6; ++a) {
        for (int b = a + 1; b <= 7; ++b) {
            for (int c = b + 1; c <= 8; ++c) {
                for (int d = c + 1; d <= 9; ++d) {
                    std::set<Rational> reachable;
                    collect_values({Rational(a), Rational(b), Rational(c), Rational(d)}, reachable);

                    std::set<int> integers;
                    for (const Rational& value : reachable) {
                        if (value.den == 1 && value.num > 0) {
                            integers.insert(static_cast<int>(value.num));
                        }
                    }

                    int len = 0;
                    while (integers.count(len + 1) != 0) {
                        ++len;
                    }

                    if (len > best_len) {
                        best_len = len;
                        best_digits = std::to_string(a) + std::to_string(b)
                                    + std::to_string(c) + std::to_string(d);
                    }
                }
            }
        }
    }

    return best_digits;
}

int main() {
    const std::string answer = solve();
    assert(answer == "1258");
    std::cout << answer << '\n';
    return 0;
}
