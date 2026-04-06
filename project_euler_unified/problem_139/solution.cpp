#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long P = 100000000LL;
    long long count = 0;

    for (long long m = 2; 2 * m * (m + 1) <= P; m++) {
        for (long long n = 1; n < m; n++) {
            if ((m - n) % 2 == 0) continue;
            if (__gcd(m, n) != 1) continue;

            long long a = m * m - n * n;
            long long b = 2 * m * n;
            long long c = m * m + n * n;
            long long perim = a + b + c; // = 2*m*(m+n)

            if (perim > P) break;

            long long diff = abs(b - a);
            if (diff > 0 && c % diff == 0) {
                count += P / perim;
            }
        }
    }

    cout << count << endl;
    return 0;
}
