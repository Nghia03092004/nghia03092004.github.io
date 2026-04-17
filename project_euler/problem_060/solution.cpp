#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Sieve of Eratosthenes
vector<bool> sieve;
vector<int> primes;

void buildSieve(int n) {
    sieve.assign(n + 1, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (sieve[i]) {
            for (int j = i * i; j <= n; j += i)
                sieve[j] = false;
        }
    }
    for (int i = 2; i <= n; i++)
        if (sieve[i]) primes.push_back(i);
}

// Modular multiplication via __int128 to avoid overflow.
ll mulmod(ll a, ll b, ll m) {
    return (__int128)a * b % m;
}

// Fast modular exponentiation.
ll powmod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// Single round of Miller-Rabin with witness a.
bool millerRabin(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ll x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

// Deterministic primality test.
bool isPrime(ll n) {
    if (n < (int)sieve.size()) return sieve[n];
    if (n < 2) return false;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll a : {2, 3, 5, 7, 11, 13}) {
        if (!millerRabin(n, a)) return false;
    }
    return true;
}

// Concatenate a and b: returns a || b = a * 10^{d(b)} + b.
ll concat(ll a, ll b) {
    ll tmp = b, mult = 1;
    while (tmp > 0) { mult *= 10; tmp /= 10; }
    return a * mult + b;
}

// Check if primes a and b form a prime pair.
bool pairCheck(int a, int b) {
    return isPrime(concat(a, b)) && isPrime(concat(b, a));
}

int main() {
    const int LIMIT = 10000;
    buildSieve(LIMIT * 100);  // sieve large enough for concatenations

    int n = primes.size();
    int bound = upper_bound(primes.begin(), primes.end(), LIMIT) - primes.begin();

    // Build adjacency lists
    vector<vector<int>> adj(bound);
    for (int i = 0; i < bound; i++) {
        for (int j = i + 1; j < bound; j++) {
            if (pairCheck(primes[i], primes[j])) {
                adj[i].push_back(j);
            }
        }
    }

    // Fast pair lookup via set
    set<pair<int,int>> pairSet;
    for (int i = 0; i < bound; i++) {
        for (int j : adj[i]) {
            pairSet.insert({i, j});
        }
    }

    auto paired = [&](int i, int j) -> bool {
        if (i > j) swap(i, j);
        return pairSet.count({i, j});
    };

    int bestSum = INT_MAX;

    // Depth-first 5-clique search
    for (int i = 0; i < bound; i++) {
        for (int j : adj[i]) {
            for (int k : adj[j]) {
                if (k <= j) continue;
                if (!paired(i, k)) continue;
                for (int l : adj[k]) {
                    if (l <= k) continue;
                    if (!paired(i, l) || !paired(j, l)) continue;
                    for (int m : adj[l]) {
                        if (m <= l) continue;
                        if (!paired(i, m) || !paired(j, m) || !paired(k, m)) continue;
                        int s = primes[i] + primes[j] + primes[k] + primes[l] + primes[m];
                        bestSum = min(bestSum, s);
                    }
                }
            }
        }
    }

    cout << bestSum << endl;
    return 0;
}
