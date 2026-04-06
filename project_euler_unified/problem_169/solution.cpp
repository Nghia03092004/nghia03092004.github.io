#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 169: Number of ways to express n as sum of powers of 2,
 * each used at most twice. Find f(10^25).
 *
 * Recurrence:
 *   f(0) = 1
 *   f(n) = f(n/2)             if n is odd
 *   f(n) = f(n/2) + f(n/2-1)  if n is even
 *
 * We use Python-style big integers via strings or __int128.
 * Since 10^25 fits in unsigned 128-bit, we use __int128.
 */

typedef __int128 lll;

map<lll, long long> memo;

long long f(lll n) {
    if (n == 0) return 1;
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;

    long long result;
    if (n % 2 == 1) {
        result = f(n / 2);
    } else {
        result = f(n / 2) + f(n / 2 - 1);
    }
    memo[n] = result;
    return result;
}

int main(){
    // 10^25
    lll n = 1;
    for (int i = 0; i < 25; i++) n *= 10;

    cout << (long long)f(n) << endl;
    return 0;
}
