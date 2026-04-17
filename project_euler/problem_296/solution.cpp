#include <bits/stdc++.h>
using namespace std;

// Problem 296: Angular Bisector and Tangent
//
// Triangle ABC with integer sides a=BC <= b=AC <= c=AB, perimeter <= 100000.
// BE = c*a / (a+b) must be a positive integer.
//
// Let g = gcd(a,b), a = g*a', b = g*b', gcd(a',b')=1.
// Then BE = c*a'/(a'+b'), and since gcd(a',a'+b')=1, we need (a'+b') | c.
// So c = k*(a'+b') for positive integer k.
//
// Constraints:
//   a <= b:          a' <= b'
//   b <= c:          g*b' <= k*(a'+b')  =>  k >= ceil(g*b'/(a'+b'))
//   a+b > c:         g*(a'+b') > k*(a'+b')  =>  k < g
//   perimeter <= P:  (g+k)*(a'+b') <= P

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const long long P = 100000;
    long long count = 0;

    // Iterate over coprime (a', b') with 1 <= a' <= b'
    // s = a' + b', s ranges from 2 (a'=b'=1) upward
    // Need (g+k)*s <= P, g >= 2 (since k >= 1, k < g => g >= 2), so s <= P/2

    for (long long bp = 1; bp * 2 + 1 <= P; bp++) {
        // a' ranges from 1 to b'
        for (long long ap = 1; ap <= bp; ap++) {
            if (__gcd(ap, bp) != 1) continue;

            long long s = ap + bp;
            // g ranges from 2 to floor(P/s) - 1 (need k >= 1, so g+1 <= P/s => g <= P/s - 1)
            // Also k < g and k >= ceil(g*b'/s)
            long long max_gs = P / s; // (g+k)*s <= P, g+k <= P/s

            for (long long g = 2; g <= max_gs - 1; g++) {
                // k >= ceil(g*b'/s), k < g, g+k <= P/s => k <= P/s - g
                long long k_min = (g * bp + s - 1) / s; // ceil(g*bp/s)
                long long k_max = min(g - 1, max_gs - g);

                if (k_min > k_max) continue;
                count += k_max - k_min + 1;
            }
        }
    }

    cout << count << endl;
    return 0;
}
