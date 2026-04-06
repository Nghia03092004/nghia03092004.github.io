#include <bits/stdc++.h>
using namespace std;

int main() {
    // By Primorial Optimality (Theorem 3), the answer is the largest
    // primorial not exceeding 1,000,000.
    // 2 * 3 * 5 * 7 * 11 * 13 * 17 = 510510 <= 1000000
    // 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 = 9699690 > 1000000

    const int LIMIT = 1000000;
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    long long n = 1;

    for (int p : primes) {
        if (n * p > LIMIT) break;
        n *= p;
    }

    cout << n << endl;

    return 0;
}
