#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000;

    // Totient sieve
    vector<long long> phi(LIMIT + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int p = 2; p <= LIMIT; p++) {
        if (phi[p] == p) { // p is prime
            for (int m = p; m <= LIMIT; m += p) {
                phi[m] = phi[m] / p * (p - 1);
            }
        }
    }

    // Sigma sieve (sum of divisors)
    vector<long long> sigma(LIMIT + 1, 0);
    for (int j = 1; j <= LIMIT; j++) {
        for (int m = j; m <= LIMIT; m += j) {
            sigma[m] += j;
        }
    }

    // Sum of gcd(phi(n), sigma(n))
    long long total = 0;
    for (int n = 1; n <= LIMIT; n++) {
        total += __gcd(phi[n], sigma[n]);
    }

    cout << "Sum of gcd(phi(n), sigma(n)) for n=1.." << LIMIT << ": " << total << endl;

    // Display first few values
    cout << "\nn | phi(n) | sigma(n) | gcd" << endl;
    for (int n = 1; n <= 20; n++) {
        cout << n << " | " << phi[n] << " | " << sigma[n]
             << " | " << __gcd(phi[n], sigma[n]) << endl;
    }

    return 0;
}
