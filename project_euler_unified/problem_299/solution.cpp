#include <bits/stdc++.h>
using namespace std;

// Problem 299: Three Similar Triangles
//
// Two cases:
// CASE 1 (Incenter/Pythagorean):
//   b^2 + d^2 = perfect square. Primitive Pythagorean triples enumerated.
//   For primitive (x,y,z): sum = x+y. Multiples: k from 1 to (L-1)/sum.
//   2 triplets per k (swap b,d since x != y always).
//
// CASE 2 (Parallel, b = d):
//   q^2 + 2f^2 = c^2. Primitive solutions with gcd(m,n)=1, n odd, n^2 != 2m^2:
//   q = |n^2 - 2m^2|, f = 2mn, c = n^2 + 2m^2.
//   b = c + f = n^2 + 2m^2 + 2mn, d = b.
//   b+d = 2b_prim * k < L. Multiples: k from 1 to (L-1)/(2*b_prim).
//   1 triplet per k.
//   IMPORTANT: iterate ALL coprime (m,n) with n odd and m >= 1, not just n > m*sqrt(2).

int main(){
    long long L = 100000000LL;
    long long count = 0;

    // CASE 1: Pythagorean triples
    // m > n > 0, gcd(m,n) = 1, m+n odd
    for (long long n = 1; ; n++) {
        // Minimum sum at m=n+1: (n+1)^2-n^2 + 2(n+1)n = 2n+1+2n^2+2n = 2n^2+4n+1
        if (2*n*n + 4*n + 1 >= L) break;

        for (long long m = n + 1; ; m++) {
            if ((m + n) % 2 == 0) continue;
            if (__gcd(m, n) != 1) continue;

            long long x = m*m - n*n;
            long long y = 2*m*n;
            long long s = x + y;
            if (s >= L) break;

            long long num_k = (L - 1) / s;
            count += 2 * num_k;
        }
    }

    // CASE 2: Parallel (Pell-type)
    // Iterate coprime (m, n) with n odd, m >= 1, n >= 1, n^2 != 2m^2
    // b_prim = n^2 + 2m^2 + 2mn = (n+m)^2 + m^2
    // Need 2*b_prim < L, i.e., b_prim < L/2
    // b_prim = n^2 + 2mn + 2m^2 >= 1 + 2 + 2 = 5 (for m=n=1)

    for (long long n = 1; ; n += 2) { // n odd
        // Minimum b_prim at m=1: n^2 + 2n + 2
        long long bmin = n*n + 2*n + 2;
        if (2*bmin >= L) break;

        for (long long m = 1; ; m++) {
            if (n*n == 2*m*m) continue;
            if (__gcd(m, n) != 1LL) continue;

            long long b_prim = n*n + 2*m*n + 2*m*m;
            if (2*b_prim >= L) break;

            long long num_k = (L - 1) / (2 * b_prim);
            if (num_k > 0)
                count += num_k;
        }
    }

    cout << count << endl;
    return 0;
}
