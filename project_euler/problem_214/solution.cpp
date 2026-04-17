#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 40000000;

    // Sieve for Euler's totient
    vector<int> phi(N);
    iota(phi.begin(), phi.end(), 0); // phi[i] = i

    for (int i = 2; i < N; i++) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j < N; j += i) {
                phi[j] = phi[j] / i * (i - 1);
            }
        }
    }

    // Compute chain lengths
    vector<int> chain(N, 0);
    chain[1] = 1;
    for (int i = 2; i < N; i++) {
        chain[i] = 1 + chain[phi[i]];
    }

    // Sum primes with chain length 25
    long long answer = 0;
    for (int i = 2; i < N; i++) {
        if (phi[i] == i - 1 && chain[i] == 25) { // i is prime and chain length is 25
            answer += i;
        }
    }

    cout << answer << endl;
    return 0;
}
