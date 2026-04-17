#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 919: Multiplicative Persistence
 *
 * Find the sum of all n < 10^7 with multiplicative persistence exactly 4.
 *
 * Multiplicative persistence: repeatedly replace n by the product of its
 * digits until reaching a single digit. Count the steps.
 *
 * Key facts:
 *   - f(n) < n for n >= 10, so persistence is finite.
 *   - Numbers with digit 0 have persistence 1 (f(n) = 0 immediately).
 *   - Erdos conjecture: persistence = O(log log n).
 *   - Record: persistence 11 at n = 277777788888899.
 *
 * Complexity: O(N * log N) time, O(1) space.
 */

int digit_product(int n) {
    int p = 1;
    while (n > 0) {
        p *= n % 10;
        n /= 10;
    }
    return p;
}

int persistence(int n) {
    int c = 0;
    while (n >= 10) {
        n = digit_product(n);
        c++;
    }
    return c;
}

int main() {
    const int N = 10000000;
    const int TARGET = 4;
    long long total = 0;

    for (int n = 10; n < N; n++) {
        if (persistence(n) == TARGET) {
            total += n;
        }
    }

    cout << total << endl;

    // Verification: known smallest values with each persistence
    assert(persistence(77) == 4);    // 77->49->36->18->8
    assert(persistence(679) == 5);   // 679->378->168->48->32->6
    assert(persistence(6788) == 6);
    assert(persistence(68889) == 7);

    return 0;
}
