#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000;

    // Totient sieve
    vector<int> phi(LIMIT + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int p = 2; p <= LIMIT; p++) {
        if (phi[p] == p) { // p is prime
            for (int m = p; m <= LIMIT; m += p) {
                phi[m] = phi[m] / p * (p - 1);
            }
        }
    }

    // Chain lengths
    vector<int> L(LIMIT + 1, 0);
    for (int n = 2; n <= LIMIT; n++) {
        L[n] = 1 + L[phi[n]];
    }

    // Sum of chain lengths
    long long total = 0;
    for (int n = 2; n <= LIMIT; n++) {
        total += L[n];
    }

    cout << "Sum of L(n) for n=2.." << LIMIT << ": " << total << endl;

    // Print some chain lengths
    cout << "\nChain lengths:" << endl;
    for (int n = 2; n <= 20; n++) {
        cout << "L(" << n << ") = " << L[n];
        // Print chain
        cout << "  chain: " << n;
        int x = n;
        while (x > 1) {
            x = phi[x];
            cout << " -> " << x;
        }
        cout << endl;
    }

    return 0;
}
