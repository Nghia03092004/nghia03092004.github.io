#include <cassert>
#include <iostream>
#include <vector>

int digit_square_sum(int n) {
    int s = 0;
    while (n > 0) {
        int d = n % 10;
        s += d * d;
        n /= 10;
    }
    return s;
    return s;
}

bool ends_at_89(int n) {
    while (n != 1 && n != 89) {
        n = digit_square_sum(n);
    }
    return n == 89;
}

long long solve() {
    const int digits = 7;
    const int max_sum = digits * 81;

    std::vector<bool> destinations(max_sum + 1, false);
    for (int s = 1; s <= max_sum; ++s) {
        destinations[s] = ends_at_89(s);
    }

    std::vector<long long> dp(max_sum + 1, 0);
    dp[0] = 1;

    for (int pos = 0; pos < digits; ++pos) {
        std::vector<long long> next_dp(max_sum + 1, 0);
        for (int sum = 0; sum <= max_sum; ++sum) {
            if (dp[sum] == 0) {
                continue;
            }
            for (int digit = 0; digit <= 9; ++digit) {
                next_dp[sum + digit * digit] += dp[sum];
            }
        }
        dp = next_dp;
    }

    long long answer = 0;
    for (int s = 0; s <= max_sum; ++s) {
        if (destinations[s]) {
            answer += dp[s];
        }
    }
    return answer;
}

int main() {
    const long long answer = solve();
    assert(answer == 8581146);
    std::cout << answer << '\n';
    return 0;
}
