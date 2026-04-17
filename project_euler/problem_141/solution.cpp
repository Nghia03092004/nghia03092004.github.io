#include <bits/stdc++.h>
using namespace std;

int main() {
    // Find sum of all progressive perfect squares below 10^12
    // n = a^3 * b * c^2 + b^2 * c, with gcd(a,b)=1, a>b>=1, c>=1
    // Check if n is a perfect square

    long long LIMIT = 1000000000000LL; // 10^12
    set<long long> squares;

    for (long long a = 2; a * a * a < LIMIT; a++) {
        for (long long b = 1; b < a; b++) {
            if (__gcd(a, b) != 1) continue;
            // n = a^3*b*c^2 + b^2*c
            // For c=1: n = a^3*b + b^2
            long long a3b = a * a * a * b;
            if (a3b >= LIMIT) break;
            for (long long c = 1; ; c++) {
                long long n = a3b * c * c + b * b * c;
                if (n >= LIMIT) break;
                long long s = (long long)sqrt((double)n);
                // Check around s due to floating point
                for (long long t = max(1LL, s - 2); t <= s + 2; t++) {
                    if (t * t == n) {
                        squares.insert(n);
                        break;
                    }
                }
            }
        }
    }

    long long ans = 0;
    for (long long x : squares) ans += x;
    cout << ans << endl;
    return 0;
}
