#include <cmath>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr int TARGET = 1'000'000;
const std::string TARGET_ANSWER = "2.533018434e23";

using i128 = __int128_t;

struct Stats {
    i128 count;
    i128 sum_first;
    i128 sum_product;
};

i128 triangular(i128 n) {
    return n * (n + 1) / 2;
}

std::vector<int> mobius_sieve(int limit) {
    std::vector<int> mu(limit + 1);
    std::vector<int> primes;
    std::vector<bool> composite(limit + 1, false);
    mu[1] = 1;

    for (int value = 2; value <= limit; ++value) {
        if (!composite[value]) {
            primes.push_back(value);
            mu[value] = -1;
        }
        for (int prime : primes) {
            long long next = 1LL * value * prime;
            if (next > limit) {
                break;
            }
            composite[static_cast<int>(next)] = true;
            if (value % prime == 0) {
                mu[static_cast<int>(next)] = 0;
                break;
            }
            mu[static_cast<int>(next)] = -mu[value];
        }
    }

    return mu;
}

std::pair<i128, i128> count_and_sum(const std::string& filter_name, int limit, int divisor) {
    const i128 n = limit / divisor;

    if (filter_name == "all") {
        return {n, static_cast<i128>(divisor) * triangular(n)};
    }

    if (filter_name == "odd") {
        if (divisor % 2 == 0) {
            return {0, 0};
        }
        const i128 odd_count = (n + 1) / 2;
        return {odd_count, static_cast<i128>(divisor) * odd_count * odd_count};
    }

    if (filter_name == "non5") {
        if (divisor % 5 == 0) {
            return {0, 0};
        }
        return {
            n - n / 5,
            static_cast<i128>(divisor) * (triangular(n) - 5 * triangular(n / 5)),
        };
    }

    if (filter_name == "coprime10") {
        if (divisor % 2 == 0 || divisor % 5 == 0) {
            return {0, 0};
        }
        const i128 odd_count = (n + 1) / 2;
        const i128 odd_multiple_5_count = (n / 5 + 1) / 2;
        const i128 count = odd_count - odd_multiple_5_count;
        const i128 total =
            static_cast<i128>(divisor)
            * (odd_count * odd_count - 5 * odd_multiple_5_count * odd_multiple_5_count);
        return {count, total};
    }

    assert(false && "unknown filter");
    return {0, 0};
}

Stats pair_stats(int limit, const std::string& filter_name, const std::vector<int>& mu) {
    i128 count_full = 0;
    i128 sum_first_full = 0;
    i128 sum_product_full = 0;

    for (int divisor = 1; divisor <= limit; ++divisor) {
        const int mobius = mu[divisor];
        if (mobius == 0) {
            continue;
        }
        const auto [count, total] = count_and_sum(filter_name, limit, divisor);
        if (count == 0) {
            continue;
        }
        count_full += static_cast<i128>(mobius) * count * count;
        sum_first_full += static_cast<i128>(mobius) * total * count;
        sum_product_full += static_cast<i128>(mobius) * total * total;
    }

    const auto [total_count, total_sum] = count_and_sum(filter_name, limit, 1);
    return {
        count_full - (2 * total_count - 1),
        sum_first_full - (total_count + total_sum - 1),
        sum_product_full - (2 * total_sum - 1),
    };
}

i128 solve_quarters(int limit = TARGET) {
    const std::vector<int> mu = mobius_sieve(limit);

    const Stats all_stats = pair_stats(limit, "all", mu);
    const Stats odd_stats = pair_stats(limit, "odd", mu);
    const Stats non5_stats = pair_stats(limit, "non5", mu);
    const Stats coprime10_stats = pair_stats(limit, "coprime10", mu);

    const i128 count_10 = all_stats.count - odd_stats.count - non5_stats.count
                        + coprime10_stats.count;
    const i128 sum_first_10 = all_stats.sum_first - odd_stats.sum_first - non5_stats.sum_first
                            + coprime10_stats.sum_first;
    const i128 sum_product_10 =
        all_stats.sum_product - odd_stats.sum_product - non5_stats.sum_product
        + coprime10_stats.sum_product;

    return 8 * all_stats.sum_product
         - 12 * all_stats.sum_first
         + 4 * count_10
         - 6 * sum_product_10
         + 6 * sum_first_10;
}

long double to_long_double(i128 value) {
    return static_cast<long double>(value);
}

long double d_formula(int a, int b) {
    if ((1LL * a * b) % 10 == 0) {
        return static_cast<long double>(2LL * a * b - 3LL * a - 3LL * b + 4) / 4.0L;
    }
    return static_cast<long double>(4LL * a * b - 3LL * a - 3LL * b) / 2.0L;
}

long double brute_sum(int limit) {
    long double total = 0.0L;
    for (int a = 2; a <= limit; ++a) {
        for (int b = 2; b <= limit; ++b) {
            int x = a;
            int y = b;
            while (y != 0) {
                const int temp = x % y;
                x = y;
                y = temp;
            }
            if (x == 1) {
                total += d_formula(a, b);
            }
        }
    }
    return total;
}

std::string format_quarters(i128 quarters, int digits_after_decimal = 9) {
    const long double value = to_long_double(quarters) / 4.0L;
    std::ostringstream out;
    out << std::scientific << std::setprecision(digits_after_decimal) << value;
    const std::string text = out.str();
    const std::size_t split = text.find('e');
    const std::string mantissa = text.substr(0, split);
    const int exponent = std::stoi(text.substr(split + 1));
    return mantissa + "e" + std::to_string(exponent);
}

bool nearly_equal(long double a, long double b, long double eps = 1e-12L) {
    return fabsl(a - b) <= eps;
}

void self_test() {
    assert(nearly_equal(d_formula(2, 3), 4.5L));
    assert(nearly_equal(d_formula(2, 5), 0.75L));
    assert(nearly_equal(d_formula(7, 4), 39.5L));
    assert(nearly_equal(d_formula(7, 5), 52.0L));
    assert(nearly_equal(d_formula(10, 7), 23.25L));

    for (int limit : {3, 4, 5, 10, 20}) {
        assert(nearly_equal(to_long_double(solve_quarters(limit)) / 4.0L, brute_sum(limit)));
    }

    assert(solve_quarters(10) == static_cast<i128>(6410));
    assert(solve_quarters(100) == static_cast<i128>(97026020));
    assert(format_quarters(solve_quarters()) == TARGET_ANSWER);
}

}  // namespace

int main() {
    self_test();
    std::cout << format_quarters(solve_quarters()) << '\n';
    return 0;
}
