#include <bits/stdc++.h>
using namespace std;

int main(){
    // PE 202: Laser beam in equilateral triangle, R = 12017639147 reflections.
    // n = (R + 3) / 2 = 6008819575.
    // Count b in [1, n-1] with b ≡ 2 mod 3 and gcd(b, n) = 1.
    // Use Mobius inversion over squarefree divisors of n.

    long long R = 12017639147LL;
    long long n = (R + 3) / 2;  // 6008819575

    // Factor n
    vector<long long> primes;
    {
        long long tmp = n;
        for(long long p = 2; p * p <= tmp; p++){
            if(tmp % p == 0){
                primes.push_back(p);
                while(tmp % p == 0) tmp /= p;
            }
        }
        if(tmp > 1) primes.push_back(tmp);
    }

    int np = primes.size();
    long long ans = 0;

    for(int mask = 0; mask < (1 << np); mask++){
        long long d = 1;
        int bits = __builtin_popcount(mask);
        long long mu = (bits % 2 == 0) ? 1 : -1;
        for(int i = 0; i < np; i++){
            if(mask & (1 << i)) d *= primes[i];
        }

        if(n % d != 0) continue; // d must divide n

        long long nd = n / d;
        long long d_mod3 = d % 3;
        long long cnt = 0;

        if(d_mod3 == 0){
            // 3 | d => d*k ≡ 0 mod 3 ≠ 2. No valid k.
            cnt = 0;
        } else {
            long long dinv3 = (d_mod3 == 1) ? 1 : 2;
            long long r = (2 * dinv3) % 3;
            long long upper = nd - 1;
            if(r == 0){
                if(upper >= 3)
                    cnt = (upper - 3) / 3 + 1;
            } else {
                if(upper >= r)
                    cnt = (upper - r) / 3 + 1;
            }
        }

        ans += mu * cnt;
    }

    cout << ans << endl;
    return 0;
}
