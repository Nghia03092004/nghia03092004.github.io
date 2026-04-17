#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
typedef long long ll;
const ull LIMIT = 1000000000000ULL; // 10^12
const ull MOD = (1ULL << 32);

// Miller-Rabin primality test for numbers up to 10^12
ull mulmod(ull a, ull b, ull m) {
    return (__uint128_t)a * b % m;
}

ull powmod(ull a, ull b, ull m) {
    ull res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

bool millerRabin(ull n, ull a) {
    if (n % a == 0) return n == a;
    ull d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ull x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool isPrime(ull n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    // Deterministic for n < 3.3 * 10^24
    for (ull a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (!millerRabin(n, a)) return false;
    }
    return true;
}

int main() {
    // Step 1: Generate all Hamming numbers <= LIMIT
    vector<ull> hamming;
    for (ull a = 1; a <= LIMIT; a *= 2) {
        for (ull b = a; b <= LIMIT; b *= 3) {
            for (ull c = b; c <= LIMIT; c *= 5) {
                hamming.push_back(c);
            }
        }
    }
    sort(hamming.begin(), hamming.end());

    // Step 2: Find Hamming primes (h+1 is prime where h is a Hamming number)
    vector<ull> hprimes;
    for (ull h : hamming) {
        if (h >= 2 && isPrime(h + 1)) {
            hprimes.push_back(h + 1);
        }
    }
    // Add 2 (since phi(2) = 1 which is Hamming, but 2-1=1 is Hamming)
    // Actually 2 = 1+1, and 1 is a Hamming number, so 2 is already included
    // We also need to handle that 2,3,5 themselves are special
    // Remove 2, 3, 5 from hprimes since they are handled as part of Hamming numbers
    // Actually: n can have powers of 2,3,5 AND distinct Hamming primes
    // The primes 2,3,5 with higher powers are already covered by Hamming numbers
    // For primes p where p-1 is Hamming: p=2(1+1),3(2+1),5(4+1),7(6+1=2*3),11,13,17,...
    // We keep all but note that 2,3,5 are special (can appear to any power)

    // Remove 2, 3, 5 from hprimes - they're handled separately as Hamming base
    hprimes.erase(remove_if(hprimes.begin(), hprimes.end(),
        [](ull p) { return p == 2 || p == 3 || p == 5; }), hprimes.end());
    sort(hprimes.begin(), hprimes.end());

    // Step 3: For each subset of Hamming primes (product <= LIMIT),
    // multiply by each Hamming number <= LIMIT/product and sum up
    ull ans = 0;

    // DFS to enumerate subsets of hprimes
    // For each product of a subset, multiply by all Hamming numbers that fit
    function<void(int, ull)> dfs = [&](int idx, ull prod) {
        // Add contribution: prod * each Hamming number <= LIMIT/prod
        ull maxH = LIMIT / prod;
        for (ull h : hamming) {
            if (h > maxH) break;
            ans = (ans + (prod * h) % MOD) % MOD;
        }
        // Try adding the next prime
        for (int i = idx; i < (int)hprimes.size(); i++) {
            if (prod > LIMIT / hprimes[i]) break;
            dfs(i + 1, prod * hprimes[i]);
        }
    };

    dfs(0, 1);

    cout << ans << endl;
    return 0;
}
