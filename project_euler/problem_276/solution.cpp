#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 276: Primitive Triangles
 *
 * Count primitive triangles (gcd(a,b,c)=1) with a<=b<=c and perimeter <= N=10^7.
 *
 * Method:
 * 1. Compute t(s) for all s using the recurrence from the generating function
 *    G(x) = x^3 / ((1-x^2)(1-x^3)(1-x^4)), which gives:
 *    t(n) = t(n-2)+t(n-3)+t(n-4)-t(n-5)-t(n-6)-t(n-7)+t(n-9) for n >= 12
 *
 * 2. Apply Mobius sieve: for n from 1 to N/2, subtract t[n] from t[kn] for k>=2.
 *    After sieving, t[n] = number of PRIMITIVE triangles with perimeter exactly n.
 *
 * 3. Sum all t[n] for n = 3..N.
 */

int main(){
    const int N = 10000000;

    // Step 1: Compute triangle counts using recurrence
    vector<long long> t(N + 1, 0);
    // Initial values (OEIS A005044):
    t[3] = 1; t[5] = 1; t[6] = 1; t[7] = 2; t[8] = 1; t[9] = 3; t[10] = 2; t[11] = 4;

    for (int n = 12; n <= N; n++) {
        t[n] = t[n-2] + t[n-3] + t[n-4] - t[n-5] - t[n-6] - t[n-7] + t[n-9];
    }

    // Step 2: Mobius sieve to extract primitive counts
    for (long long n = 1; n <= N / 2; n++) {
        if (t[n] == 0) continue;
        for (long long k = 2 * n; k <= N; k += n) {
            t[k] -= t[n];
        }
    }

    // Step 3: Sum primitive counts
    long long answer = 0;
    for (int n = 3; n <= N; n++) {
        answer += t[n];
    }

    cout << answer << endl;
    return 0;
}
