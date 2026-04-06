#include <bits/stdc++.h>
using namespace std;

const int LIMIT = 1000000;

int dr(int n) {
    if (n == 0) return 0;
    int r = n % 9;
    return r == 0 ? 9 : r;
}

int mdrs[LIMIT];

int main() {
    ios::sync_with_stdio(false);

    // Initialize mdrs[n] = dr(n)
    for (int n = 2; n < LIMIT; n++) {
        mdrs[n] = dr(n);
    }

    // Sieve: for each factor a >= 2, update multiples
    for (int a = 2; a * a < LIMIT; a++) {
        for (int j = a; (long long)a * j < LIMIT; j++) {
            int n = a * j;
            mdrs[n] = max(mdrs[n], dr(a) + mdrs[j]);
        }
    }

    long long total = 0;
    for (int n = 2; n < LIMIT; n++) {
        total += mdrs[n];
    }

    cout << total << endl;
    return 0;
}
