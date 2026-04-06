#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 463: A Weird Recurrence Relation
 *
 * f(1)=1, f(3)=3, f(2n)=f(n),
 * f(4n+1) = 2f(2n+1) - f(n),
 * f(4n+3) = 3f(2n+1) - 2f(n)
 *
 * S(n) = f(1) + f(2) + ... + f(n).
 * Compute S(3^37) mod 10^9.
 *
 * Strategy: Mutual recursion S, T, f with memoization.
 *   S(2N)   = S(N) + T(N-1)
 *   S(2N+1) = S(N) + T(N)
 *   T(2k)   = 2T(k) + 3T(k-1) - S(k) - 2S(k-1) - 1
 *   T(2k+1) = T(2k) + 3f(2k+1) - 2f(k)
 *
 * Complexity: O(log^2 n) distinct sub-problems.
 */

const long long MOD = 1000000000LL;

unordered_map<long long, long long> memo_f, memo_S, memo_T;

long long mod(long long x) {
    return ((x % MOD) + MOD) % MOD;
}

long long f_func(long long n);
long long S_func(long long n);
long long T_func(long long k);

long long f_func(long long n) {
    if (n == 1) return 1;
    if (n % 2 == 0) return f_func(n / 2);
    if (memo_f.count(n)) return memo_f[n];

    long long result;
    if (n % 4 == 1) {
        long long k = (n - 1) / 4;
        result = mod(2 * f_func(2 * k + 1) - f_func(k));
    } else { // n % 4 == 3
        long long k = (n - 3) / 4;
        result = mod(3 * f_func(2 * k + 1) - 2 * f_func(k));
    }
    return memo_f[n] = result;
}

long long T_func(long long k) {
    if (k == 0) return 1;
    if (k == 1) return 4;
    if (memo_T.count(k)) return memo_T[k];

    long long result;
    if (k % 2 == 0) {
        long long half = k / 2;
        result = mod(2 * T_func(half) + 3 * T_func(half - 1)
                     - S_func(half) - 2 * S_func(half - 1) - 1);
    } else {
        result = mod(T_func(k - 1) + 3 * f_func(k) - 2 * f_func((k - 1) / 2));
    }
    return memo_T[k] = result;
}

long long S_func(long long n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (memo_S.count(n)) return memo_S[n];

    long long result;
    if (n % 2 == 0) {
        result = mod(S_func(n / 2) + T_func(n / 2 - 1));
    } else {
        result = mod(S_func(n / 2) + T_func(n / 2));
    }
    return memo_S[n] = result;
}

int main() {
    // Compute 3^37
    long long power = 1;
    for (int i = 0; i < 37; i++) {
        power *= 3;
    }

    // Verify small cases
    assert(S_func(8) == 22 % MOD);
    assert(S_func(100) == 3604 % MOD);

    long long ans = S_func(power);
    cout << ans << endl;

    assert(ans == 808981553LL);

    return 0;
}
