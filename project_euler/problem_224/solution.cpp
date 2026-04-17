#include <bits/stdc++.h>
using namespace std;

int main(){
    // a^2+b^2 = c^2-1, a<=b<=c, a+b+c<=L. Only even a work.
    // a=2m, n=4m^2+1, d*e=n (both odd), d<=e.
    // b=(e-d)/2>=a=2m, perimeter=2m+e<=L.
    // Need: d<=sqrt(n), e-d>=4m, 2m+n/d<=L.

    const long long L = 75000000LL;
    const long long MMAX = L / 6;

    // Sieve and divide: for each m, val[m] starts as 4m^2+1.
    // For each prime p ≡ 1 (mod 4), find m where p | val[m], divide out p.
    // Track factorization by storing number of prime factors "seen" in ndiv.
    // But we need to enumerate divisors, not just count them.

    // Key insight: we can accumulate the divisor count directly.
    // Initialize cnt[m] = 1 for all m (the trivial divisor d=1).
    // For each prime p, for m where p^k || n, multiply cnt[m] by (k+1).
    // But this gives TOTAL divisor count, not the count of valid divisors.

    // We need more than the count. We need to enumerate divisors or compute
    // sum of indicator function. This requires the actual factorization.

    // PRACTICAL APPROACH: sieve to divide out primes from val[m].
    // After sieve, val[m] is either 1 or a large prime.
    // Store the factorization as we go: use a flat array with linked list per m.

    // Actually, let me use a simpler storage: for each m, store just the
    // list of (prime, exponent) in a global vector, indexed by m.

    // Memory estimate: each m has ~3-4 prime factors on average.
    // 12.5M * 4 * 8 bytes ~ 400MB. That's too much.

    // More compact: store just (prime, exponent) as a single int (prime < 25M fits in 25 bits,
    // exp < 60 fits in 6 bits). Pack into 32-bit int.

    // Or even simpler: store divisor lists. Average number of divisors is ~10-20.
    // 12.5M * 20 * 8 bytes = 2GB. Way too much.

    // DIFFERENT APPROACH: don't store factorizations. Instead, build divisors
    // on the fly during the sieve.

    // For each m, maintain a list of divisors of the "sieved part" of n.
    // Initially divs[m] = {1}. When prime p is sieved and p^k || val[m],
    // multiply the divisor list by {1, p, p^2, ..., p^k}.
    // After sieve, if val[m] > 1 (large prime q), multiply by {1, q}.
    // Then check each divisor against constraints.

    // But maintaining 12.5M divisor lists is expensive in memory.

    // MUCH SIMPLER WORKING APPROACH: use the "sieve and divide" to get val[m]
    // down to 1 or a large prime. Then to enumerate divisors, re-factor n
    // using val[m]'s original value divided by val[m]'s final value to get the
    // "smooth part", and its factorization can be obtained by trial division
    // of the smooth part (which has all prime factors <= 25M, and the smooth
    // part itself is at most n, but we can factor it by re-dividing).

    // Actually, simplest: sieve to get the full factorization by recording
    // which primes divide val[m] during the sieve. Use a segmented approach.

    // OK, I think the cleanest practical solution is:
    // Process m in blocks. For each block, use the sieve to fully factor val[m].

    // But let me try yet another approach: iterate over divisor d directly.
    // For each odd d from 1 to sqrt(L^2), find m where d | 4m^2+1.
    // Then e = (4m^2+1)/d, check constraints.

    // d | 4m^2+1 means 4m^2 ≡ -1 (mod d). Need d odd.
    // This has solutions iff all prime power factors p^k of d satisfy:
    // -1 is a QR mod p^k and 4 is invertible (always since d odd).

    // For each d, find residues of m mod d, then iterate m in arithmetic progression.

    // Number of valid d: up to sqrt(max n) = sqrt(4*(12.5M)^2) = 25M.
    // For each d, work = (MMAX/d) * (number of residues).
    // Total: sum over d of (MMAX/d * sigma_0_-like) ~ MMAX * sum(1/d) ~ MMAX * ln(25M) ~ 200M.
    // This should be fast enough!

    // But finding the residues of m mod d (solutions to 4m^2 ≡ -1 mod d) for each d
    // is non-trivial when d is composite. We need to factor d, solve mod each prime power,
    // then CRT.

    // This is complex to implement. Let me try the block sieve approach instead.

    // BLOCK SIEVE APPROACH:
    // Process m in blocks of size B (e.g., B = 1M).
    // For each block [lo, hi]:
    //   - Initialize val[m-lo] = 4m^2+1 for m in [lo, hi].
    //   - For each prime p ≡ 1 (mod 4) up to 25M:
    //       find m in [lo, hi] with p | val, divide out p, record factor.
    //   - After all primes, remaining val > 1 is a large prime.
    //   - Enumerate divisors and count valid ones.

    // Problem: for each block, iterating over ALL ~800K primes is expensive.
    // For large p, few m in the block are affected.
    // Optimization: only iterate primes up to sqrt(max val in block). But max val ~ 6.25e14,
    // sqrt = 25M, so all primes are needed.

    // Alternative: for small primes, precompute starting positions.
    // For large primes (> sqrt(hi_val)), val/p < p, so at most one large prime factor.

    // Total sieve work: for each prime p, ~MMAX/p hits across all blocks.
    // Sum over p ≡ 1 mod 4 of MMAX/p ~ MMAX * ln(ln(25M)) ~ 12.5M * 3 ~ 37.5M.
    // But the per-hit work includes dividing and recording. Should be fast.

    // Let me implement this.

    auto power_mod = [](long long base, long long exp, long long mod) -> long long {
        long long result = 1; base %= mod;
        while(exp > 0){
            if(exp & 1) result = (__int128)result * base % mod;
            base = (__int128)base * base % mod;
            exp >>= 1;
        }
        return result;
    };

    long long PMAX_SIEVE = 25000001LL;
    vector<bool> is_prime(PMAX_SIEVE + 1, true);
    is_prime[0] = is_prime[1] = false;
    for(long long i = 2; i <= PMAX_SIEVE; i++){
        if(!is_prime[i]) continue;
        for(long long j = i*i; j <= PMAX_SIEVE; j += i) is_prime[j] = false;
    }

    // For each prime p ≡ 1 (mod 4), compute the two starting residues m0, m1.
    struct PrimeInfo {
        int p;
        long long m0, m1; // residues mod p where p | 4m^2+1
    };
    vector<PrimeInfo> primes_info;

    for(long long p = 5; p <= PMAX_SIEVE; p++){
        if(!is_prime[p] || p % 4 != 1) continue;
        long long r = 0;
        for(long long g = 2; g < p; g++){
            r = power_mod(g, (p-1)/4, p);
            if((__int128)r * r % p == p - 1) break;
        }
        long long inv2 = (p + 1) / 2;
        long long m0 = (__int128)r * inv2 % p;
        long long m1 = (__int128)(p - r) * inv2 % p;
        primes_info.push_back({(int)p, m0, m1});
    }

    // Block sieve
    const int B = 500000;
    // For each m in current block, store remaining unfactored part and divisor list.
    // Divisor list is built up multiplicatively.

    long long count = 0;

    for(long long lo = 1; lo <= MMAX; lo += B){
        long long hi = min(lo + B - 1, MMAX);
        int sz = hi - lo + 1;

        // val[i] = remaining unfactored part of 4*(lo+i)^2 + 1
        vector<long long> bval(sz);
        for(int i = 0; i < sz; i++){
            long long m = lo + i;
            bval[i] = 4LL*m*m + 1;
        }

        // divs[i] = list of divisors of the factored-out part so far
        vector<vector<long long>> divs(sz, vector<long long>(1, 1LL));

        // Sieve with each prime
        for(auto& pi : primes_info){
            long long p = pi.p;
            // Find first m >= lo with m ≡ pi.m0 (mod p)
            for(int s = 0; s < 2; s++){
                long long residue = (s == 0) ? pi.m0 : pi.m1;
                long long first_m = lo + ((residue - lo % p) % p + p) % p;
                if(first_m < lo) first_m += p;
                for(long long m = first_m; m <= hi; m += p){
                    int idx = m - lo;
                    if(bval[idx] % p != 0) continue;
                    int exp = 0;
                    while(bval[idx] % p == 0){
                        bval[idx] /= p;
                        exp++;
                    }
                    // Multiply divisor list by {p, p^2, ..., p^exp}
                    int old_sz = divs[idx].size();
                    long long pk = 1;
                    for(int e = 0; e < exp; e++){
                        pk *= p;
                        for(int j = 0; j < old_sz; j++){
                            divs[idx].push_back(divs[idx][j] * pk);
                        }
                    }
                }
            }
        }

        // After sieve, bval[i] is 1 or a large prime
        for(int i = 0; i < sz; i++){
            long long m = lo + i;
            long long n = 4LL*m*m + 1;
            long long a = 2*m;

            if(bval[i] > 1){
                // Large prime factor q
                long long q = bval[i];
                int old_sz = divs[i].size();
                for(int j = 0; j < old_sz; j++){
                    divs[i].push_back(divs[i][j] * q);
                }
            }

            long long sqrtn = (long long)sqrt((double)n);
            while((sqrtn+1)*(sqrtn+1) <= n) sqrtn++;
            while(sqrtn * sqrtn > n) sqrtn--;

            for(long long d : divs[i]){
                if(d > sqrtn) continue;
                long long e = n / d;
                if(e - d < 4*m) continue;
                if(2*m + e > L) continue;
                count++;
            }
        }
    }

    cout << count << endl;
    return 0;
}
