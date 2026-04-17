#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 279: Triangles with integer sides and perimeter <= 10^8
 * that have at least one angle that is an integer number of degrees.
 *
 * By Niven's theorem, the only angles in (0,180) that are integer degrees
 * AND have rational cosine are: 60, 90, 120 degrees.
 * (cos must be rational for the law of cosines to yield integer sides.)
 *
 * So we count triangles with at least one angle equal to 60, 90, or 120 degrees.
 * These three families have NO overlap (30-60-90 triangle has irrational side ratio,
 * and 30-90-60, 60-90-30 etc. all require cos(30)=sqrt(3)/2 which is irrational).
 *
 * Total = count_60 + count_90 + count_120 + equilateral
 * (with equilateral = all angles 60, counted separately since parametrization excludes them)
 *
 * 120-degree triangles: c^2 = a^2+ab+b^2
 *   Primitive param (m>n>0, gcd(m,n)=1, (m-n)%3!=0):
 *     sides: m^2-n^2, 2mn+n^2, m^2+mn+n^2
 *     perimeter: (2m+n)(m+n)
 *
 * 60-degree (B=60, non-equilateral) Case 1:
 *   sides: m^2-n^2, m^2+mn+n^2, m(m+2n)
 *   perimeter: 3m(m+n)
 *
 * 60-degree Case 2:
 *   sides: n(2m+n), m^2+mn+n^2, m(m+2n)
 *   perimeter: (2m+n)(m+2n)
 *
 * 90-degree (Pythagorean): c^2 = a^2+b^2
 *   Primitive param (m>n>0, gcd(m,n)=1, m-n odd):
 *     sides: m^2-n^2, 2mn, m^2+n^2
 *     perimeter: 2m(m+n)
 *
 * Equilateral: a=b=c, perimeter 3a <= N. Count: floor(N/3).
 */

int main(){
    const long long N = 100000000LL;
    long long count = 0;

    // Equilateral triangles (all angles = 60)
    count += N / 3;

    // 120-degree triangles
    for (long long m = 2; ; m++) {
        long long min_p = (2*m+1)*(m+1);
        if (min_p > N) break;
        for (long long n = 1; n < m; n++) {
            if (__gcd(m, n) != 1LL) continue;
            if ((m - n) % 3 == 0) continue;
            long long p0 = (2*m+n)*(m+n);
            if (p0 > N) break;
            count += N / p0;
        }
    }

    // 60-degree, Case 1: perimeter = 3m(m+n)
    for (long long m = 2; ; m++) {
        long long min_p = 3*m*(m+1);
        if (min_p > N) break;
        for (long long n = 1; n < m; n++) {
            if (__gcd(m, n) != 1LL) continue;
            if ((m - n) % 3 == 0) continue;
            long long p0 = 3*m*(m+n);
            if (p0 > N) break;
            count += N / p0;
        }
    }

    // 60-degree, Case 2: perimeter = (2m+n)(m+2n)
    for (long long m = 2; ; m++) {
        long long min_p = (2*m+1)*(m+2);
        if (min_p > N) break;
        for (long long n = 1; n < m; n++) {
            if (__gcd(m, n) != 1LL) continue;
            if ((m - n) % 3 == 0) continue;
            long long p0 = (2*m+n)*(m+2*n);
            if (p0 > N) break;
            count += N / p0;
        }
    }

    // 90-degree (Pythagorean) triangles
    for (long long m = 2; ; m++) {
        long long min_p = 2*m*(m+1);
        if (min_p > N) break;
        for (long long n = 1; n < m; n++) {
            if (__gcd(m, n) != 1LL) continue;
            if ((m - n) % 2 == 0) continue;
            long long p0 = 2*m*(m+n);
            if (p0 > N) break;
            count += N / p0;
        }
    }

    cout << count << endl;
    return 0;
}
