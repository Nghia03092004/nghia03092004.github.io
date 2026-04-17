#include <bits/stdc++.h>
using namespace std;

// Sieve of Eratosthenes
vector<int> sieve_primes(int limit) {
    vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= limit; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= limit; i++)
        if (is_prime[i]) primes.push_back(i);
    return primes;
}

// Rank of apparition: smallest k > 0 with F(k) = 0 (mod p)
int rank_of_apparition(int p) {
    long long a = 0, b = 1;
    for (int k = 1; k <= 6 * p + 10; k++) {
        long long c = (a + b) % p;
        a = b;
        b = c;
        if (a == 0) return k;
    }
    return -1; // Should not happen
}

// Modular multiplication avoiding overflow for 64-bit
// For mod up to 10^16, we need 128-bit multiplication
typedef unsigned long long ull;
typedef __int128 lll;

ull mulmod(ull a, ull b, ull mod) {
    return (lll)a * b % mod;
}

ull addmod(ull a, ull b, ull mod) {
    return (a + b) % mod;
}

// Compute (F(n), F(n+1)) mod m using the doubling method
pair<ull, ull> fib_mod(long long n, ull mod) {
    if (n == 0) return {0, 1};
    auto [a, b] = fib_mod(n >> 1, mod);
    // F(2k) = F(k) * (2*F(k+1) - F(k))
    // F(2k+1) = F(k)^2 + F(k+1)^2
    ull c = mulmod(a, (2 * b % mod - a + mod) % mod, mod);
    ull d = addmod(mulmod(a, a, mod), mulmod(b, b, mod), mod);
    if (n & 1)
        return {d, addmod(c, d, mod)};
    else
        return {c, d};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int TARGET = 100000000; // 10^8
    // Estimate sieve size: ~76.47% of Fibonacci numbers are squarefree
    const int N = (int)(TARGET / 0.76) + 5000;

    printf("Sieve size N = %d\n", N);
    printf("Target: %d-th squarefree Fibonacci number\n", TARGET);

    // Step 1: Generate primes up to N/2
    int prime_limit = N / 2 + 1;
    printf("Generating primes up to %d...\n", prime_limit);
    vector<int> primes = sieve_primes(prime_limit);
    printf("Found %zu primes\n", primes.size());

    // Step 2: Sieve non-squarefree indices
    vector<bool> not_sqfree(N + 1, false);

    printf("Computing ranks of apparition and sieving...\n");
    int primes_used = 0;
    for (int p : primes) {
        int alpha = rank_of_apparition(p);
        long long q = (long long)p * alpha;
        if (q > N) continue;
        primes_used++;
        for (long long j = q; j <= N; j += q)
            not_sqfree[j] = true;
    }
    printf("Primes contributing to sieve: %d\n", primes_used);

    // Step 3: Find the TARGET-th squarefree index
    int count = 0;
    int target_index = -1;
    for (int i = 1; i <= N; i++) {
        if (!not_sqfree[i]) {
            count++;
            if (count == TARGET) {
                target_index = i;
                break;
            }
        }
    }

    if (target_index == -1) {
        printf("ERROR: Sieve too small! Found only %d squarefree Fibonacci numbers.\n", count);
        return 1;
    }

    printf("The %d-th squarefree Fibonacci number is F(%d)\n", TARGET, target_index);

    // Step 4: Compute F(target_index) mod 10^16
    ull MOD = 10000000000000000ULL; // 10^16
    auto [fib_val, _] = fib_mod(target_index, MOD);
    printf("Last 16 digits: %016llu\n", fib_val);

    // Step 5: Compute scientific notation via log10
    // F(n) ~ phi^n / sqrt(5) for large n
    double log10_phi = log10((1.0 + sqrt(5.0)) / 2.0);
    double log10_val = target_index * log10_phi - 0.5 * log10(5.0);
    int exponent = (int)log10_val;
    double mantissa = pow(10.0, log10_val - exponent);
    // Round mantissa to 1 decimal place
    mantissa = round(mantissa * 10.0) / 10.0;
    if (mantissa >= 10.0) {
        mantissa /= 10.0;
        exponent++;
    }

    printf("Scientific notation: %.1fe%d\n", mantissa, exponent);
    printf("\nAnswer: %016llu,%.1fe%d\n", fib_val, mantissa, exponent);

    return 0;
}

/*
Compile and run:
    g++ -O2 -std=c++17 -o solution solution.cpp
    ./solution

Expected output:
    Answer: 1508395636674243,6.5e27330467

Complexity:
    - Time: O(N log log N) for sieve, O(N) for rank computations
    - Space: O(N) for boolean sieve
    - N ~ 1.32 * 10^8

Key insight (Wall's conjecture):
    p^2 | F(n) iff p * alpha(p) | n
    where alpha(p) = smallest k with p | F(k)
*/
