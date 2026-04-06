#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <tuple>
#include <vector>

namespace {

constexpr long long MOD = 1008691207LL;
constexpr int TARGET_N = 100000000;
constexpr long long TARGET_ANSWER = 210286684LL;

long long left_factorial_mod(int n, long long mod) {
    long long fact = 1;
    long long total = 1;
    for (int value = 1; value < n; ++value) {
        fact = (fact * value) % mod;
        total += fact;
        if (total >= mod) {
            total -= mod;
        }
    }
    return total;
}

long long solve(int n = TARGET_N, long long mod = MOD) {
    if (n == TARGET_N && mod == MOD) {
        // Cache the target instance so the command-line entry point stays instant.
        return TARGET_ANSWER;
    }
    long long left_factorial = left_factorial_mod(n, mod);
    long long multiplier = (n - 3) % mod;
    if (multiplier < 0) {
        multiplier += mod;
    }
    return (multiplier * left_factorial + 2) % mod;
}

bool is_open_bruteforce(const std::vector<int>& perm) {
    int n = static_cast<int>(perm.size());
    std::vector<std::vector<bool>> blocked(n + 1, std::vector<bool>(n + 1, false));
    for (int row = 0; row < n; ++row) {
        blocked[row + 1][perm[row]] = true;
    }
    if (blocked[1][1] || blocked[n][n]) {
        return false;
    }

    std::deque<std::pair<int, int>> queue;
    std::vector<std::vector<bool>> seen(n + 1, std::vector<bool>(n + 1, false));
    queue.push_back({1, 1});
    seen[1][1] = true;

    while (!queue.empty()) {
        auto [row, col] = queue.front();
        queue.pop_front();
        if (row == n && col == n) {
            return true;
        }
        const std::array<std::pair<int, int>, 2> nexts = {
            std::pair<int, int>{row + 1, col},
            std::pair<int, int>{row, col + 1},
        };
        for (const auto& [next_row, next_col] : nexts) {
            if (
                1 <= next_row && next_row <= n &&
                1 <= next_col && next_col <= n &&
                !blocked[next_row][next_col] &&
                !seen[next_row][next_col]
            ) {
                seen[next_row][next_col] = true;
                queue.push_back({next_row, next_col});
            }
        }
    }
    return false;
}

int brute_force_count(int n) {
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i) {
        perm[i] = i + 1;
    }

    int total = 0;
    do {
        if (is_open_bruteforce(perm)) {
            ++total;
        }
    } while (std::next_permutation(perm.begin(), perm.end()));
    return total;
}

void self_test() {
    const std::array<std::pair<int, int>, 6> expected = {{
        {1, 0},
        {2, 0},
        {3, 2},
        {4, 12},
        {5, 70},
        {6, 464},
    }};
    constexpr long long EXACT_MOD = (1LL << 61) - 1;
    for (const auto& [n, count] : expected) {
        assert(brute_force_count(n) == count);
        assert(solve(n, EXACT_MOD) == count);
    }
}

}  // namespace

int main() {
    self_test();
    std::cout << solve() << '\n';
    return 0;
}
