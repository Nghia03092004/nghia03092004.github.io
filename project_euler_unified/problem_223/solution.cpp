#include <bits/stdc++.h>
using namespace std;

int main(){
    // a^2 + b^2 = c^2 + 1, a <= b <= c, a+b+c <= L
    // (c-b)(c+b) = (a-1)(a+1), c-b = d, c+b = e, d*e = (a-1)(a+1)
    // b = (e-d)/2, c = (e+d)/2, perimeter = a + e
    // Need: d,e same parity, d <= e, b >= a <=> e - d >= 2a, a + e <= L

    // Case a=1: b=c, count = (L-1)/2
    // Case a>=2: d*e = a^2-1, d <= e, same parity, e >= d+2a, a+e <= L

    // Approach: iterate d from 1 to ~sqrt(L^2) and for each d, iterate
    // over values of a such that d | (a-1)(a+1) and constraints hold.

    // Actually, better: iterate d and e directly.
    // d*e = a^2 - 1 = (a-1)(a+1). Given d and e, a = sqrt(d*e + 1).
    // So iterate d, e with d <= e, d*e + 1 is a perfect square, same parity.

    // But this is still too many pairs...

    // Best approach: iterate d from 1 to some bound.
    // For each d, iterate a such that d | (a^2-1).
    // Given d | (a^2-1), i.e., a^2 ≡ 1 (mod d), i.e., a ≡ ±1 (mod p^k) for each prime power in d.
    // Then e = (a^2-1)/d. Conditions: e >= d (i.e., a^2-1 >= d^2), same parity, e-d >= 2a, a+e <= L.

    // For each d, the valid a values form arithmetic progressions mod d.
    // We can find them by CRT. But implementing CRT for each d is complex.

    // Simplest fast approach: iterate (d, a) where d | (a-1) or d | (a+1) or
    // d shares factors with both.

    // Actually, let's factor differently. Write a-1 = s, a+1 = s+2.
    // d*e = s*(s+2). For each factorization d*e = s*(s+2) with d <= e:
    // We can write d = gcd(d, s) * gcd(d, s+2) roughly...

    // Let me try a different decomposition. Let g = gcd(s, s+2) = gcd(s, 2).
    // If s even (a odd): g = 2, s = 2u, s+2 = 2(u+1), product = 4u(u+1).
    //   d and e must be both even. d = 2d', e = 2e', d'*e' = u(u+1).
    //   u and u+1 are coprime. So divisors of u(u+1) are products of
    //   a divisor of u and a divisor of u+1.
    //   For each divisor d1 of u and d2 of (u+1): d' = d1*d2, e' = (u/d1)*((u+1)/d2).
    //   Need d' <= e'.

    // If s odd (a even): g = 1, product = s*(s+2), gcd(s, s+2) = 1.
    //   d and e must both be odd (since product is odd).
    //   Divisors of s*(s+2) are products of divisor of s and divisor of s+2.
    //   For each d1|s, d2|(s+2): d = d1*d2, e = (s/d1)*((s+2)/d2). Need d <= e.

    // So the approach is: iterate s (= a-1) and for each s, factor s and s+2,
    // then enumerate divisor pairs.

    // But factoring each s is expensive for s up to 8M... unless we use a sieve.

    const long long L = 25000000LL;
    long long count = 0;

    // Case a = 1
    count += (L - 1) / 2;

    // Sieve: for each n up to L, store its smallest prime factor
    // a ranges from 2 to L/3, so s = a-1 ranges from 1 to L/3-1 ~ 8.3M
    // s+2 ranges up to L/3+1. We need to factor s and s+2.
    // Max value to factor: L/3 + 1 ~ 8.3M

    const int SMAX = L / 3 + 2;
    vector<int> spf(SMAX + 1, 0); // smallest prime factor
    for(int i = 2; i <= SMAX; i++){
        if(spf[i] == 0){
            for(int j = i; j <= SMAX; j += i){
                if(spf[j] == 0) spf[j] = i;
            }
        }
    }

    auto get_divisors = [&](long long n) -> vector<long long> {
        if(n <= 0) return {};
        if(n == 1) return {1};
        vector<long long> divs = {1};
        long long tmp = n;
        while(tmp > 1){
            int p = spf[tmp];
            int cnt = 0;
            while(tmp % p == 0){ tmp /= p; cnt++; }
            int sz = divs.size();
            long long pk = 1;
            for(int i = 0; i < cnt; i++){
                pk *= p;
                for(int j = 0; j < sz; j++){
                    divs.push_back(divs[j] * pk);
                }
            }
        }
        return divs;
    };

    // Case a even (s = a-1 odd): s*(s+2) is odd. d, e both odd.
    // gcd(s, s+2) = 1. d = d1*d2 where d1|s, d2|(s+2).
    for(long long a = 2; a <= L / 3; a += 2){
        long long s = a - 1; // odd
        long long s2 = a + 1; // odd
        auto divs_s = get_divisors(s);
        auto divs_s2 = get_divisors(s2);
        for(long long d1 : divs_s){
            for(long long d2 : divs_s2){
                long long d = d1 * d2;
                long long e = (s / d1) * (s2 / d2);
                if(d > e) continue;
                // b = (e-d)/2, need b >= a => e - d >= 2a
                if(e - d < 2*a) continue;
                // perimeter = a + e <= L
                if(a + e > L) continue;
                count++;
            }
        }
    }

    // Case a odd (s = a-1 even): 4 | s*(s+2). d, e both even.
    // s = 2u, s+2 = 2(u+1), d = 2d', e = 2e', d'*e' = u*(u+1).
    // gcd(u, u+1) = 1. d' = d1*d2 where d1|u, d2|(u+1).
    for(long long a = 3; a <= L / 3; a += 2){
        long long u = (a - 1) / 2;
        long long u1 = u + 1;
        auto divs_u = get_divisors(u);
        auto divs_u1 = get_divisors(u1);
        for(long long d1 : divs_u){
            for(long long d2 : divs_u1){
                long long dp = d1 * d2;
                long long ep = (u / d1) * (u1 / d2);
                if(dp > ep) continue;
                long long d = 2 * dp;
                long long e = 2 * ep;
                long long b = (e - d) / 2;
                if(b < a) continue;
                if(a + e > L) continue;
                count++;
            }
        }
    }

    cout << count << endl;
    return 0;
}
