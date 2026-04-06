#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 533: Minimum Values of the Carmichael Function
 *
 * Count n in [1, N] where lambda(n) = lambda(n+1).
 *
 * lambda(n) = lcm of lambda(p^k) over prime powers p^k || n.
 * lambda(p^k) = phi(p^k) for odd p, special rules for p=2.
 *
 * Algorithm: SPF sieve, compute lambda per element, compare consecutive.
 * Time: O(N log log N).  Space: O(N).
 */

const int N = 100000001; // 10^8 + 1

int spf[N + 1];

void build_spf() {
    for (int i = 0; i <= N; i++) spf[i] = i;
    for (int i = 2; (ll)i * i <= N; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

ll compute_lambda(int n) {
    if (n <= 1) return 1;
    ll result = 1;
    while (n > 1) {
        int p = spf[n];
        ll pk = 1;
        int k = 0;
        while (n % p == 0) {
            n /= p;
            pk *= p;
            k++;
        }
        ll local;
        if (p == 2) {
            if (k == 1) local = 1;
            else if (k == 2) local = 2;
            else local = pk / 4;  // 2^{k-2}
        } else {
            local = pk / p * (p - 1);  // phi(p^k)
        }
        result = result / __gcd(result, local) * local;  // lcm
    }
    return result;
}

int main() {
    build_spf();

    int count = 0;
    ll prev = compute_lambda(1);
    for (int n = 2; n <= N; n++) {
        ll curr = compute_lambda(n);
        if (prev == curr && n - 1 <= N - 1) {
            count++;
        }
        prev = curr;
    }

    cout << count << endl;

    return 0;
}
