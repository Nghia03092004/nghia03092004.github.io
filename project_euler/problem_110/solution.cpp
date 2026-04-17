#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 110: Diophantine Reciprocals II
 *
 * Same as Problem 108 but threshold is 4,000,000.
 * Need d(n^2) >= 8,000,001.
 *
 * We use logarithms to avoid overflow when comparing n values,
 * since n can be very large. We track log(n) and reconstruct
 * the actual n at the end using __int128 or careful multiplication.
 */

const int NPRIMES = 20;
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
double log_primes[NPRIMES];
double best_log_n;
vector<int> best_exponents;

void dfs(int idx, int max_exp, double log_n, long long div_count, vector<int>& exponents) {
    if (div_count >= 8000001LL) {
        if (log_n < best_log_n) {
            best_log_n = log_n;
            best_exponents = exponents;
        }
        return;
    }
    if (idx >= NPRIMES) return;
    if (log_n >= best_log_n) return;

    for (int e = 1; e <= max_exp; e++) {
        double new_log = log_n + e * log_primes[idx];
        if (new_log >= best_log_n) break;
        exponents.push_back(e);
        dfs(idx + 1, e, new_log, div_count * (2 * e + 1), exponents);
        exponents.pop_back();
    }
}

int main() {
    for (int i = 0; i < NPRIMES; i++)
        log_primes[i] = log((double)primes[i]);

    best_log_n = 1e18;
    vector<int> exponents;
    dfs(0, 40, 0.0, 1, exponents);

    // Reconstruct the actual n using the best exponents
    // Use __int128 or careful computation
    // Since answer fits in long long (9350130049860600), we can use long long
    long long n = 1;
    for (int i = 0; i < (int)best_exponents.size(); i++) {
        for (int j = 0; j < best_exponents[i]; j++) {
            n *= primes[i];
        }
    }

    cout << n << endl;
    return 0;
}
