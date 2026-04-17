#include <bits/stdc++.h>
using namespace std;

int main() {
    // Find the 150000th number n for which phi(n) = 13!
    long long target = 6227020800LL; // 13!

    // Factorization of 13! = 2^10 * 3^5 * 5^2 * 7 * 11 * 13
    vector<pair<long long, int>> factors = {{2,10},{3,5},{5,2},{7,1},{11,1},{13,1}};

    // Generate all divisors
    vector<long long> divisors;
    divisors.push_back(1);
    for (auto& [p, e] : factors) {
        int sz = divisors.size();
        long long pk = 1;
        for (int k = 1; k <= e; k++) {
            pk *= p;
            for (int i = 0; i < sz; i++) {
                divisors.push_back(divisors[i] * pk);
            }
        }
    }
    sort(divisors.begin(), divisors.end());

    auto isPrime = [](long long n) -> bool {
        if (n < 2) return false;
        if (n < 4) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    };

    // Candidate primes
    vector<long long> primes;
    for (long long d : divisors) {
        if (isPrime(d + 1)) {
            primes.push_back(d + 1);
        }
    }
    sort(primes.begin(), primes.end());
    primes.erase(unique(primes.begin(), primes.end()), primes.end());

    // Recursively find all n such that phi(n) = target
    // n = p1^a1 * p2^a2 * ... where p1 < p2 < ...
    // phi(n) = prod(pi^(ai-1) * (pi-1))

    vector<long long> solutions;

    function<void(int, long long, long long)> search = [&](int idx, long long rem, long long n) {
        if (rem == 1) {
            // n and 2*n both have the same totient if n is odd
            // But p=2 with k=1 contributes factor 1 to phi, so 2*n is handled
            // when we include p=2 with k=1.
            solutions.push_back(n);
            return;
        }
        for (int i = idx; i < (int)primes.size(); i++) {
            long long p = primes[i];
            if (p - 1 > rem) break;
            if (rem % (p - 1) != 0) continue;
            long long r = rem / (p - 1);
            long long pk = 1; // p^(k-1), n_factor = p^k
            long long n_factor = p;
            while (true) {
                if (r % pk != 0) break;
                // Check overflow: n * n_factor
                if (n_factor > 1e18 / n) break; // overflow protection
                search(i + 1, r / pk, n * n_factor);
                pk *= p;
                n_factor *= p;
            }
        }
    };

    search(0, target, 1);

    sort(solutions.begin(), solutions.end());

    // 150000th (1-indexed)
    if ((int)solutions.size() >= 150000) {
        cout << solutions[150000 - 1] << endl;
    } else {
        cout << "Not enough solutions: " << solutions.size() << endl;
    }

    return 0;
}
