#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long modinv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

int main() {
    const int N = 20000;

    // Smallest prime factor sieve
    vector<int> spf(N + 1, 0);
    vector<int> primes;
    for (int i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
            for (long long j = (long long)i * i; j <= N; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    // B(n) = prod_{k=0}^{n} C(n,k)
    // The exponent of prime p in B(n):
    // v_p(B(n)) = sum_{k=0}^{n} v_p(C(n,k))
    //           = sum_{k=0}^{n} [v_p(n!) - v_p(k!) - v_p((n-k)!)]
    //           = (n+1)*v_p(n!) - 2*sum_{k=0}^{n} v_p(k!)
    //
    // Let's define cum_vp(n) = sum_{k=0}^{n} v_p(k!)
    // Then v_p(B(n)) = (n+1)*v_p(n!) - 2*cum_vp(n)
    //
    // Note: v_p(k!) = v_p((k-1)!) + v_p(k)
    // And: cum_vp(n) = cum_vp(n-1) + v_p(n!)
    //
    // Incrementally from n-1 to n:
    // v_p(B(n)) = (n+1)*v_p(n!) - 2*cum_vp(n)
    // v_p(B(n-1)) = n*v_p((n-1)!) - 2*cum_vp(n-1)
    //
    // v_p(B(n)) - v_p(B(n-1)) = (n+1)*v_p(n!) - 2*cum_vp(n) - n*v_p((n-1)!) + 2*cum_vp(n-1)
    //   = (n+1)*(v_p((n-1)!) + v_p(n)) - n*v_p((n-1)!) - 2*v_p(n!)
    //   = v_p((n-1)!) + (n+1)*v_p(n) - 2*(v_p((n-1)!) + v_p(n))
    //   = v_p((n-1)!) + (n+1)*v_p(n) - 2*v_p((n-1)!) - 2*v_p(n)
    //   = (n-1)*v_p(n) - v_p((n-1)!)
    //
    // So delta = (n-1)*v_p(n) - v_p((n-1)!)

    // Precompute v_p(k) for each k using spf
    // v_p(n!) = sum_{i>=1} floor(n/p^i)

    auto vp_factorial = [](long long n, long long p) -> long long {
        long long res = 0;
        long long pk = p;
        while (pk <= n) {
            res += n / pk;
            if (pk > n / p) break; // overflow prevention
            pk *= p;
        }
        return res;
    };

    auto vp_val = [&](int n, int p) -> int {
        int cnt = 0;
        while (n % p == 0) { n /= p; cnt++; }
        return cnt;
    };

    // For each prime, track exponent in B(n)
    // Start with B(1) = C(1,0)*C(1,1) = 1, all exponents 0
    // B(0) = C(0,0) = 1

    // We'll track exponents as a vector indexed by prime index
    int np = primes.size();
    vector<long long> expo(np, 0); // expo[i] = exponent of primes[i] in B(current_n)

    // Precompute modinv for each prime
    vector<long long> inv_pm1(np);
    for (int i = 0; i < np; i++) {
        inv_pm1[i] = modinv(primes[i] - 1, MOD);
    }

    long long S = 0;
    // B(1) = 1, D(1) = 1
    S = 1; // D(1) = 1

    // Compute for n = 2 to N
    // We need to track expo from B(1)
    // B(1): all exponents are 0

    for (int n = 2; n <= N; n++) {
        // delta for prime p: (n-1)*v_p(n) - v_p((n-1)!)
        for (int i = 0; i < np && primes[i] <= n; i++) {
            int p = primes[i];
            long long vpn = vp_val(n, p);
            if (vpn > 0 || p <= n - 1) {
                long long delta = (long long)(n - 1) * vpn - vp_factorial(n - 1, p);
                expo[i] += delta;
            }
        }

        // Compute D(n)
        long long D = 1;
        for (int i = 0; i < np && primes[i] <= n; i++) {
            if (expo[i] > 0) {
                long long num = (power(primes[i], expo[i] + 1, MOD) - 1 + MOD) % MOD;
                D = D * num % MOD * inv_pm1[i] % MOD;
            }
        }

        S = (S + D) % MOD;
    }

    cout << S << endl;
    return 0;
}
