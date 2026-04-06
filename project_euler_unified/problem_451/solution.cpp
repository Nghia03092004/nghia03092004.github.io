#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 451: Modular Inverses
 *
 * Find sum of I(n) for 3 <= n <= 2*10^7, where I(n) is the largest m < n-1
 * such that m^2 = 1 (mod n).
 *
 * Approach: Sieve-based. For each n, we store a vector of all self-inverse
 * residues. We build these up by processing prime powers: for each prime power
 * q = p^a, for each multiple n of q, we combine existing solutions with
 * solutions mod q via CRT.
 *
 * Answer: 153651073760956
 */

const int MAXN = 20000001;

// spf[i] = smallest prime factor of i
int spf[MAXN];

// For each n, store all self-inverse solutions found so far
// We use a vector of vectors; memory-intensive but manageable for 2*10^7
// Actually, we'll use a more memory-efficient approach:
// Process prime factors one at a time via SPF, combining CRT incrementally.

long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) { x = 0; y = 1; return b; }
    long long x1, y1;
    long long g = extgcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return g;
}

// CRT: x = r1 mod m1, x = r2 mod m2, gcd(m1,m2)=1
// Returns x mod m1*m2
long long crt2(long long r1, long long m1, long long r2, long long m2) {
    long long x, y;
    extgcd(m1, m2, x, y);
    long long mod = m1 * m2;
    long long res = (r1 + m1 * ((r2 - r1) % m2 + m2) % m2 * x) % mod;
    // More carefully:
    long long diff = ((r2 - r1) % m2 + m2) % m2;
    res = r1 + m1 * (diff * ((x % m2 + m2) % m2) % m2);
    res = ((res % mod) + mod) % mod;
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int N = 20000000;

    // Sieve smallest prime factor
    for (int i = 0; i <= N; i++) spf[i] = i;
    for (int i = 2; (long long)i * i <= N; i++) {
        if (spf[i] == i) { // prime
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    // For each n, factor using spf and compute all self-inverses via CRT
    // Then I(n) = second largest (largest < n-1)

    long long total = 0;

    for (int n = 3; n <= N; n++) {
        // Factor n
        int temp = n;
        // Collect prime powers
        // Use small static array since n <= 2*10^7 has at most ~8 prime factors
        int pp[10]; // prime powers p^a
        int npp = 0;

        while (temp > 1) {
            int p = spf[temp];
            int pa = 1;
            while (temp % p == 0) {
                pa *= p;
                temp /= p;
            }
            pp[npp++] = pa;
            // Store p as well for the 2-special case
            // We need to know if p==2 and what a is
            // Let's store both
            // Redo: store (p, pa) pairs
            pp[npp - 1] = pa; // we'll handle p==2 by checking pa
            // Actually let's use a struct approach
            // Simpler: just redo with separate arrays
            (void)p; // we'll fix below
        }

        // Redo factoring properly
        temp = n;
        npp = 0;
        int primes[10], pas[10];
        while (temp > 1) {
            int p = spf[temp];
            int pa = 1;
            while (temp % p == 0) {
                pa *= p;
                temp /= p;
            }
            primes[npp] = p;
            pas[npp] = pa;
            npp++;
        }

        // Build all self-inverse solutions via CRT
        // Start with solutions = {1} mod 1, current_mod = 1
        // Max solutions: 2^(npp) * possible factor of 2 for p=2, a>=3
        // Worst case about 2^9 = 512 solutions, but typically much fewer
        static long long sols[1024], new_sols[1024];
        int nsols = 1;
        sols[0] = 1;
        long long cur_mod = 1;

        for (int i = 0; i < npp; i++) {
            int p = primes[i];
            long long pa = pas[i];

            // Local solutions mod pa
            long long local_s[4];
            int nloc;
            if (p == 2) {
                if (pa == 1) {
                    local_s[0] = 1;
                    nloc = 1;
                } else if (pa == 2) {
                    // Actually for mod 4: 1^2=1, 3^2=9=1 mod 4. So {1, 3}.
                    local_s[0] = 1; local_s[1] = 3;
                    nloc = 2;
                } else {
                    // pa >= 8
                    long long half = pa / 2;
                    local_s[0] = 1;
                    local_s[1] = half - 1;
                    local_s[2] = half + 1;
                    local_s[3] = pa - 1;
                    nloc = 4;
                }
            } else {
                local_s[0] = 1;
                local_s[1] = pa - 1;
                nloc = 2;
            }

            // Combine via CRT
            int nnew = 0;
            for (int a = 0; a < nsols; a++) {
                for (int b = 0; b < nloc; b++) {
                    new_sols[nnew++] = crt2(sols[a], cur_mod, local_s[b], pa);
                }
            }

            cur_mod *= pa;
            nsols = nnew;
            memcpy(sols, new_sols, nsols * sizeof(long long));
        }

        // Find largest solution < n-1
        long long best = 1;
        for (int i = 0; i < nsols; i++) {
            long long s = sols[i];
            if (s > 1 && s < n - 1 && s > best) {
                best = s;
            }
        }

        total += best;
    }

    cout << total << endl;

    // Verification
    if (total == 153651073760956LL) {
        cout << "CORRECT!" << endl;
    } else {
        cout << "WRONG! Expected 153651073760956" << endl;
    }

    return 0;
}
