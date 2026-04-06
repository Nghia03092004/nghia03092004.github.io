#include <bits/stdc++.h>
using namespace std;

const int KMAX = 12000;
int best[KMAX + 1];

// Enumerate factorizations of N.
// prod = current product so far, sum_f = sum of factors so far,
// count = number of factors so far, min_factor = minimum next factor.
// We are building factorizations of the target number N = prod * (remaining).
void factorize(int n, int prod, int sum_f, int count, int min_factor) {
    // n is the remaining part to factorize (prod * n = original N)
    // At any point, we can stop and use n as the last factor
    // Last factor is n itself (n >= min_factor is required)
    if (n >= min_factor) {
        int total_prod = prod * n;
        int total_sum = sum_f + n;
        int total_count = count + 1;
        if (total_count >= 2) {
            int k = total_prod - total_sum + total_count;
            if (k >= 2 && k <= KMAX) {
                if (total_prod < best[k]) {
                    best[k] = total_prod;
                }
            }
        }
    }

    // Try splitting n further: pick factor f from min_factor to sqrt(n)
    for (int f = min_factor; (long long)f * f <= n; f++) {
        if (n % f == 0) {
            factorize(n / f, prod * f, sum_f + f, count + 1, f);
        }
    }
}

int main() {
    fill(best, best + KMAX + 1, INT_MAX);

    // Upper bound: N_k <= 2k, so we check N up to 2 * KMAX
    for (int N = 2; N <= 2 * KMAX; N++) {
        factorize(N, 1, 0, 0, 2);
    }

    // Sum distinct minimal product-sum numbers
    set<int> distinct;
    for (int k = 2; k <= KMAX; k++) {
        distinct.insert(best[k]);
    }

    long long answer = 0;
    for (int v : distinct) answer += v;

    cout << answer << endl;
    return 0;
}
