#include <bits/stdc++.h>
using namespace std;

const int LIMIT = 100000000; // 10^8

int main() {
    // Sieve of Eratosthenes
    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    long long ans = 0;

    // Enumerate x, k, y
    for (long long x = 2; x * x <= LIMIT; x++) {
        for (long long k = 1; k * x * x <= LIMIT; k++) {
            long long c1 = k * x * x; // c + 1
            if (c1 - 1 < 2) continue;
            if (!is_prime[c1 - 1]) continue;

            for (long long y = 1; y < x; y++) {
                // Skip if both even
                if (x % 2 == 0 && y % 2 == 0) continue;

                long long a1 = k * y * y; // a + 1
                long long b1 = k * x * y; // b + 1

                if (a1 < 2 || b1 < 2) continue;
                if (a1 - 1 < 2) continue;

                if (!is_prime[a1 - 1]) continue;
                if (!is_prime[b1 - 1]) continue;

                // Check gcd(x, y) == 1
                if (__gcd(x, y) != 1) continue;

                ans += (a1 - 1) + (b1 - 1) + (c1 - 1);
            }
        }
    }

    cout << ans << endl;
    return 0;
}
