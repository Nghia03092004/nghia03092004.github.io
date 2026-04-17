#include <bits/stdc++.h>
using namespace std;

// Compute arithmetic derivative of n
// For n = p1^a1 * ... * pk^ak, n' = n * sum(ai / pi) = sum(ai * n / pi)
long long arithmetic_derivative(long long n) {
    if (n <= 1) return 0;
    long long result = 0;
    long long temp = n;
    for (long long d = 2; d * d <= temp; d++) {
        while (temp % d == 0) {
            result += n / d;
            temp /= d;
        }
    }
    if (temp > 1) result += n / temp;
    return result;
}

// Sieve-based computation of sum of arithmetic derivatives
long long solve_sieve(int limit) {
    // Smallest prime factor sieve
    vector<int> spf(limit + 1);
    iota(spf.begin(), spf.end(), 0);
    for (int i = 2; (long long)i * i <= limit; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    long long total = 0;
    for (int n = 2; n <= limit; n++) {
        int temp = n;
        long long ad = 0;
        while (temp > 1) {
            int p = spf[temp];
            int a = 0;
            while (temp % p == 0) {
                temp /= p;
                a++;
            }
            ad += (long long)a * (n / p);
        }
        total += ad;
    }
    return total;
}

int main() {
    // Demonstration: sum of arithmetic derivatives for n = 2..1000
    long long ans = solve_sieve(1000);
    cout << "Sum of ad(n) for n=2..1000: " << ans << endl;

    // Verify with direct computation
    long long check = 0;
    for (int n = 2; n <= 1000; n++) {
        check += arithmetic_derivative(n);
    }
    assert(ans == check);

    // For the full problem, scale up as needed
    // The sieve approach handles up to ~10^8 with enough memory
    return 0;
}
