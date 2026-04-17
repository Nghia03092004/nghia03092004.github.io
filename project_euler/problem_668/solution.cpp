#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 668: Square Root Smooth Numbers
 *
 * n is square root smooth if all prime factors of n are STRICTLY less than sqrt(n).
 * Count f(N) = |{n <= N : n is square root smooth}|.
 *
 * n > 1 is NOT smooth iff P+(n) >= sqrt(n), i.e., P+(n)^2 >= n.
 *
 * Non-smooth numbers:
 * (A) Primes: pi(N) numbers
 * (B) n = p^2 where p prime, p^2 <= N: pi(sqrtN) numbers (P+(p^2)=p=sqrt(n))
 * (C) n = p*m, p = P+(n), 2 <= m <= p-1, m*p <= N, all prime factors of m < p
 *     (automatic since m < p). For each prime p: min(p-1, N/p) - 1 valid m values.
 *
 * f(N) = 1 + N - pi(N) - pi(sqrtN) - sum_{p prime, p<=N/2} max(0, min(p-1, N/p) - 1)
 */

typedef long long ll;

int main(){
    ll N = 10000000000LL;
    ll sqrtN = 100000LL;

    int sN = (int)sqrtN;
    vector<bool> is_prime(sN + 1, true);
    is_prime[0] = is_prime[1] = false;
    for(int i = 2; (ll)i * i <= sN; i++){
        if(is_prime[i]){
            for(int j = i*i; j <= sN; j += i)
                is_prime[j] = false;
        }
    }

    // Lucy_Hedgehog prime counting
    vector<ll> small(sN + 2, 0), large(sN + 2, 0);
    for(int v = 0; v <= sN; v++) small[v] = max(0, v - 1);
    for(int k = 1; k <= sN; k++) large[k] = N / k - 1;

    for(int p = 2; p <= sN; p++){
        if(!is_prime[p]) continue;
        ll pp = (ll)p * p;
        ll pcnt = small[p - 1];

        for(int k = 1; k <= sN && (ll)k * pp <= N; k++){
            ll val = N / ((ll)k * p);
            ll sub;
            if(val <= sN)
                sub = small[(int)val];
            else
                sub = large[(int)((ll)k * p)];
            large[k] -= sub - pcnt;
        }

        for(int v = sN; v >= pp; v--){
            small[v] -= small[v / p] - pcnt;
        }
    }

    auto getPi = [&](ll x) -> ll {
        if(x <= 0) return 0;
        if(x <= sN) return small[(int)x];
        return large[(int)(N / x)];
    };

    ll piN = large[1];
    ll piSqrtN = small[sN]; // pi(sqrtN)

    // Part 1: primes p <= sqrtN, contribution = p - 2 (for p >= 3)
    ll sum1 = 0;
    for(int p = 3; p <= sN; p++){
        if(is_prime[p]) sum1 += p - 2;
    }

    // Part 2: primes sqrtN < p <= N/2, contribution = N//p - 1
    ll maxq = N / (sqrtN + 1);
    ll sum2 = 0;
    for(ll q = 2; q <= maxq; q++){
        ll phi = N / q;
        ll plo = N / (q + 1);
        if(phi <= sqrtN) continue;
        if(plo < sqrtN) plo = sqrtN;
        ll cnt = getPi(phi) - getPi(plo);
        if(cnt > 0)
            sum2 += (q - 1) * cnt;
    }

    ll answer = 1 + N - piN - piSqrtN - sum1 - sum2;
    cout << answer << endl;

    return 0;
}
