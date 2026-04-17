#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 86: Cuboid Route
    // Shortest path for a <= b <= c: sqrt((a+b)^2 + c^2)
    // Let s = a+b; need s^2 + c^2 = perfect square.
    // Count of valid (a,b): floor(s/2) - max(1, s-c) + 1.
    // Answer: 1818

    int total = 0;
    int M = 0;

    while (total <= 1000000) {
        M++;
        int c = M;
        for (int s = 2; s <= 2 * c; s++) {
            long long dsq = (long long)s * s + (long long)c * c;
            long long d = (long long)sqrt((double)dsq);
            while (d * d < dsq) d++;
            while (d * d > dsq) d--;
            if (d * d == dsq) {
                int lo = max(1, s - c);
                int hi = s / 2;
                if (hi >= lo) {
                    total += hi - lo + 1;
                }
            }
        }
    }

    cout << M << endl;
    return 0;
}
