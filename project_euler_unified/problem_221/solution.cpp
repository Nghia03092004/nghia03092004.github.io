#include <bits/stdc++.h>
using namespace std;

int main(){
    // Alexandrian integer: A = p*q*r, 1/A = 1/p + 1/q + 1/r
    // => pq + pr + qr = 1 => (q+p)(r+p) = 1 + p^2
    //
    // For positive A: p > 0, and we need A = p*q*r > 0.
    // From (q+p)(r+p) = 1+p^2, let d | (1+p^2), d > 0.
    // q = d - p, r = (1+p^2)/d - p.
    // A = p * (d-p) * ((1+p^2)/d - p)
    //
    // For p >= 1: 1+p^2 >= 2. Divisors d of 1+p^2 with 1 <= d <= 1+p^2.
    // q = d - p, r = (1+p^2)/d - p.
    //
    // For A > 0: need q*r > 0 (since p > 0).
    // Case q > 0, r > 0: d > p and (1+p^2)/d > p, i.e. d < (1+p^2)/p = p + 1/p.
    //   So p < d < p + 1/p. Since d is integer and p >= 1, no integer d in (p, p+1/p) for p >= 2.
    //   For p = 1: d in (1, 2), no integer.
    //   So this case gives nothing.
    //
    // Case q < 0, r < 0: d < p and (1+p^2)/d < p, i.e. d > (1+p^2)/p = p + 1/p.
    //   Need d < p AND d > p + 1/p. Impossible since p + 1/p > p.
    //   So this case gives nothing either.
    //
    // Wait, that can't be right. Let me reconsider. Maybe p can be negative.
    //
    // Actually, A must be positive, but p, q, r can be any nonzero integers (positive or negative).
    // The simplest parametrization: WLOG assume p > 0 (we can always relabel).
    // Then from (q+p)(r+p) = 1+p^2 > 0, both factors have same sign.
    //
    // If q+p > 0 and r+p > 0: let d = q+p > 0, (1+p^2)/d = r+p > 0.
    //   q = d-p, r = (1+p^2)/d - p.
    //   For q > 0: d > p. Then (1+p^2)/d < (1+p^2)/p = p + 1/p, so r = (1+p^2)/d - p < 1/p <= 1.
    //   Since r is integer, r <= 0. For r = 0: (1+p^2)/d = p, d = (1+p^2)/p, need p | 1+p^2, i.e. p|1, p=1.
    //   d = 2, q = 1, r = 0: A = 0, invalid.
    //   For r < 0: A = p * (d-p) * r, with d-p > 0 and r < 0, so A < 0. Invalid.
    //
    //   For q = 0: d = p, need p | 1+p^2, i.e. p | 1, p = 1. d = 1, q = 0. Invalid.
    //
    //   For q < 0: d < p. Then (1+p^2)/d > (1+p^2)/p = p + 1/p > p, so r > 0.
    //   A = p * (d-p) * ((1+p^2)/d - p) = p * (negative) * (positive) < 0. Invalid.
    //
    // If q+p < 0 and r+p < 0: let d = -(q+p) > 0, e = -(r+p) > 0. d*e = 1+p^2.
    //   q = -d-p, r = -e-p. Both q, r < 0 (since d,e > 0, p > 0).
    //   A = p * (-d-p) * (-e-p) = p * (d+p) * (e+p).
    //   This is always positive! Great.
    //   A = p * (d+p) * (e+p) where d*e = 1+p^2, d >= 1, e >= 1.
    //   WLOG d <= e (to avoid double counting... but actually different (d,e) give
    //   different (q,r) and potentially the same A).
    //
    //   Actually, swapping d and e swaps q and r, potentially giving the same A.
    //   But since we're collecting A values in a set, duplicates are handled.

    // So: A = p * (d+p) * ((1+p^2)/d + p) for each divisor d of 1+p^2, d >= 1.
    // Equivalently: A = p * (d+p) * (e+p) where d*e = 1+p^2.

    // For each p >= 1, for each divisor d of 1+p^2 (with d <= sqrt(1+p^2) to avoid
    // double counting with e), compute A.
    // If d = e = sqrt(1+p^2), count once.

    // We need to find the 150000th smallest A.

    // Upper bound: the 150000th A is 1884161251122450.
    // A = p*(d+p)*(e+p) >= p*(1+p)*((1+p^2)+p) ~ p^4 for large p.
    // So p up to ~(1.9e15)^(1/4) ~ 37000. But for small d, A is much larger.
    // For d=1: A = p*(1+p)*(1+p^2+p) = p*(1+p)*(1+p+p^2). For p=1: A=1*2*3=6.
    // For p=2: A=2*3*7=42. These grow as p^4.
    // For d=p+1 (when p+1 | 1+p^2): 1+p^2 = (p+1)(p-1)+2, so p+1|2.
    //   p+1=1 (p=0, skip) or p+1=2 (p=1). d=2, e=1. A=1*3*2=6. Same as d=1.

    // For general d: A = p*(d+p)*(e+p). The minimum A for given p is when d and e
    // are closest to sqrt(1+p^2). Since d*e = 1+p^2, d+e >= 2*sqrt(1+p^2) ~ 2p.
    // So A >= p * (d+p) * (e+p) >= p * (sqrt(1+p^2)+p)^2 ~ p*(2p)^2 = 4p^3.

    // The d=1 case: A = p*(1+p)*(1+p^2+p). For large p, A ~ p^4.
    // So we need p up to about (1.9e15)^(1/3) for the minimum-A case ~ 12400.
    // But for d=1 case, A ~ p^4, so p up to (1.9e15)^(1/4) ~ 37000.

    // To be safe, let's go up to p = 50000 or so.

    const int NEED = 150000;
    const long long PMAX = 120000; // generous upper bound
    const long long ALIMIT = 2000000000000000LL; // 2e15, generous upper bound for answer

    set<long long> alex_set;

    for(long long p = 1; p <= PMAX; p++){
        long long N = 1 + p * p;
        // Find all divisors of N
        vector<long long> divs;
        for(long long d = 1; d * d <= N; d++){
            if(N % d == 0){
                divs.push_back(d);
            }
        }
        for(long long d : divs){
            long long e = N / d;
            // A = p * (d+p) * (e+p), check overflow
            __int128 A128 = (__int128)p * (d + p) * (e + p);
            if(A128 > ALIMIT) continue;
            long long A = (long long)A128;
            alex_set.insert(A);
        }
    }

    // Convert to sorted vector
    vector<long long> alex(alex_set.begin(), alex_set.end());

    if((int)alex.size() >= NEED){
        cout << alex[NEED - 1] << endl;
    } else {
        cout << "Not enough: " << alex.size() << endl;
    }

    return 0;
}
