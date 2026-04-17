#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

const std::string TARGET = "1234567890123456789";
const std::string TARGET_ANSWER = "9.627509725002e33";

struct Tables {
    std::vector<std::vector<unsigned long long>> counts;
    std::vector<std::vector<long double>> totals;
    std::vector<long double> pow10;
};

Tables build_suffix_tables(int length) {
    const int max_sum = 9 * length;
    Tables tables{
        std::vector<std::vector<unsigned long long>>(
            length + 1, std::vector<unsigned long long>(max_sum + 1)
        ),
        std::vector<std::vector<long double>>(length + 1, std::vector<long double>(max_sum + 1)),
        std::vector<long double>(length + 1, 1.0L),
    };

    for (int i = 1; i <= length; ++i) {
        tables.pow10[i] = tables.pow10[i - 1] * 10.0L;
    }
    tables.counts[0][0] = 1;

    for (int digits = 1; digits <= length; ++digits) {
        const long double place = tables.pow10[digits - 1];
        for (int digit_sum = 0; digit_sum <= max_sum; ++digit_sum) {
            unsigned long long count = 0;
            long double total = 0.0L;
            for (int digit = 0; digit <= 9 && digit <= digit_sum; ++digit) {
                const unsigned long long prev_count =
                    tables.counts[digits - 1][digit_sum - digit];
                if (prev_count == 0) {
                    continue;
                }
                count += prev_count;
                total += static_cast<long double>(digit) * place * prev_count
                         + tables.totals[digits - 1][digit_sum - digit];
            }
            tables.counts[digits][digit_sum] = count;
            tables.totals[digits][digit_sum] = total;
        }
    }

    return tables;
}

std::vector<long double> digit_sum_weighted_totals(const std::string& limit) {
    const int length = static_cast<int>(limit.size());
    const int max_sum = 9 * length;
    const Tables tables = build_suffix_tables(length);

    std::vector<long double> by_sum(max_sum + 1, 0.0L);
    long double prefix_value = 0.0L;
    int prefix_sum = 0;

    for (int index = 0; index < length; ++index) {
        const int bound_digit = limit[index] - '0';
        const int remaining = length - 1 - index;
        const long double place = tables.pow10[remaining];

        for (int digit = 0; digit < bound_digit; ++digit) {
            const long double next_prefix = prefix_value * 10.0L + digit;
            for (int suffix_sum = 0; suffix_sum <= max_sum; ++suffix_sum) {
                const unsigned long long suffix_count = tables.counts[remaining][suffix_sum];
                if (suffix_count == 0) {
                    continue;
                }
                const int total_sum = prefix_sum + digit + suffix_sum;
                by_sum[total_sum] += next_prefix * place * suffix_count
                                   + tables.totals[remaining][suffix_sum];
            }
        }

        prefix_value = prefix_value * 10.0L + bound_digit;
        prefix_sum += bound_digit;
    }

    by_sum[prefix_sum] += prefix_value;
    return by_sum;
}

long double solve_value(const std::string& limit = TARGET) {
    const auto totals = digit_sum_weighted_totals(limit);
    long double answer = 0.0L;
    for (std::size_t digit_sum = 1; digit_sum < totals.size(); ++digit_sum) {
        answer += totals[digit_sum] / static_cast<long double>(digit_sum);
    }
    return answer;
}

long double brute_force(unsigned long long limit) {
    long double total = 0.0L;
    for (unsigned long long value = 1; value <= limit; ++value) {
        unsigned long long x = value;
        unsigned digit_sum = 0;
        while (x > 0) {
            digit_sum += static_cast<unsigned>(x % 10);
            x /= 10;
        }
        total += static_cast<long double>(value) / digit_sum;
    }
    return total;
}

std::string format_answer(long double value, int digits_after_decimal = 12) {
    std::ostringstream out;
    out << std::scientific << std::setprecision(digits_after_decimal) << value;
    const std::string text = out.str();
    const std::size_t split = text.find('e');
    const std::string mantissa = text.substr(0, split);
    const int exponent = std::stoi(text.substr(split + 1));
    return mantissa + "e" + std::to_string(exponent);
}

bool nearly_equal(long double a, long double b, long double eps) {
    return std::fabs(a - b) <= eps;
}

void self_test() {
    assert(nearly_equal(solve_value("10"), 19.0L, 1e-30L));
    assert(format_answer(solve_value("123")) == "1.187764610390e3");
    assert(format_answer(solve_value("12345")) == "4.855801996238e6");

    for (unsigned long long limit : {1ULL, 2ULL, 10ULL, 25ULL, 100ULL, 250ULL}) {
        assert(nearly_equal(solve_value(std::to_string(limit)), brute_force(limit), 1e-15L));
    }

    assert(format_answer(solve_value()) == TARGET_ANSWER);
}

}  // namespace

int main() {
    self_test();
    std::cout << format_answer(solve_value()) << '\n';
    return 0;
}
