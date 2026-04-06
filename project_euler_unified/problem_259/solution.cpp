#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 259: Reachable Numbers
 *
 * From the string "123456789", by concatenating adjacent digits and inserting
 * +, -, *, / between groups (with arbitrary parenthesization), find the sum
 * of all positive integers that can be produced.
 *
 * Method: Interval DP over substrings. R(i,j) = set of all rationals
 * achievable from digits i..j. Use exact fractions (p/q in lowest terms).
 *
 * Answer: 20101196798
 */

typedef pair<long long, long long> Frac; // (numerator, denominator), always reduced, denom > 0

long long gcd_abs(long long a, long long b) {
    a = abs(a); b = abs(b);
    while (b) { a %= b; swap(a, b); }
    return a;
}

Frac make_frac(long long p, long long q) {
    if (q == 0) return {0, 0}; // invalid
    if (p == 0) return {0, 1};
    if (q < 0) { p = -p; q = -q; }
    long long g = gcd_abs(p, q);
    return {p / g, q / g};
}

Frac add(Frac a, Frac b) { return make_frac(a.first * b.second + b.first * a.second, a.second * b.second); }
Frac sub(Frac a, Frac b) { return make_frac(a.first * b.second - b.first * a.second, a.second * b.second); }
Frac mul(Frac a, Frac b) { return make_frac(a.first * b.first, a.second * b.second); }
Frac divf(Frac a, Frac b) {
    if (b.first == 0) return {0, 0};
    return make_frac(a.first * b.second, a.second * b.first);
}

set<Frac> dp[10][10]; // dp[i][j] for digits i..j (1-indexed)
bool computed[10][10];

long long concat_num(int i, int j) {
    long long num = 0;
    for (int k = i; k <= j; k++) num = num * 10 + k;
    return num;
}

void solve(int i, int j) {
    if (computed[i][j]) return;
    computed[i][j] = true;

    // Base: concatenated number
    dp[i][j].insert(make_frac(concat_num(i, j), 1));

    // Split
    for (int k = i; k < j; k++) {
        solve(i, k);
        solve(k + 1, j);
        for (auto& a : dp[i][k]) {
            for (auto& b : dp[k+1][j]) {
                dp[i][j].insert(add(a, b));
                dp[i][j].insert(sub(a, b));
                dp[i][j].insert(mul(a, b));
                Frac d = divf(a, b);
                if (d.second != 0) dp[i][j].insert(d);
            }
        }
    }
}

int main() {
    memset(computed, false, sizeof(computed));
    solve(1, 9);

    long long ans = 0;
    for (auto& f : dp[1][9]) {
        if (f.second == 1 && f.first > 0) {
            ans += f.first;
        }
    }
    cout << ans << endl;
    return 0;
}
