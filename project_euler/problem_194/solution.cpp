#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

unsigned long long unit_a_count(unsigned long long c) {
    return c * c * c * c * c - 9 * c * c * c * c + 34 * c * c * c - 69 * c * c + 77 * c - 38;
}

unsigned long long unit_b_count(unsigned long long c) {
    return c * c * c * c * c - 8 * c * c * c * c + 27 * c * c * c - 50 * c * c + 52 * c - 24;
}

unsigned long long exact_binomial_small(int n, int k) {
    if (k > n - k) {
        k = n - k;
    }
    unsigned long long result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * static_cast<unsigned long long>(n - k + i) / static_cast<unsigned long long>(i);
    }
    return result;
}

unsigned long long pow_ull(unsigned long long base, int exp) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

unsigned long long configuration_count_small(int a_units, int b_units, unsigned long long colors) {
    return colors * (colors - 1) * exact_binomial_small(a_units + b_units, a_units)
        * pow_ull(unit_a_count(colors), a_units) * pow_ull(unit_b_count(colors), b_units);
}

unsigned long long mod_pow(unsigned long long base, int exp, unsigned long long mod) {
    unsigned long long result = 1 % mod;
    unsigned long long value = base % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * value) % mod;
        }
        value = (value * value) % mod;
        exp >>= 1;
    }
    return result;
}

int prime_exponent_in_factorial(int n, int p) {
    int exponent = 0;
    while (n > 0) {
        n /= p;
        exponent += n;
    }
    return exponent;
}

unsigned long long binomial_mod_100_25(unsigned long long mod) {
    const int n = 100;
    const int k = 25;
    const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
        53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

    unsigned long long result = 1;
    for (int p : primes) {
        int exponent = prime_exponent_in_factorial(n, p)
            - prime_exponent_in_factorial(k, p)
            - prime_exponent_in_factorial(n - k, p);
        if (exponent > 0) {
            result = result * mod_pow(static_cast<unsigned long long>(p), exponent, mod) % mod;
        }
    }
    return result;
}

int main() {
    const unsigned long long mod = 100000000ULL;
    const unsigned long long colors = 1984;

    assert(configuration_count_small(1, 0, 3) == 24ULL);
    assert(configuration_count_small(0, 2, 4) == 92928ULL);
    assert(configuration_count_small(2, 2, 3) == 20736ULL);

    unsigned long long answer = colors % mod;
    answer = answer * ((colors - 1) % mod) % mod;
    answer = answer * binomial_mod_100_25(mod) % mod;
    answer = answer * mod_pow(unit_a_count(colors) % mod, 25, mod) % mod;
    answer = answer * mod_pow(unit_b_count(colors) % mod, 75, mod) % mod;

    assert(answer == 61190912ULL);
    cout << answer << '\n';
    return 0;
}
