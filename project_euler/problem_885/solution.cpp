/*
 * Problem 885: Modular Fibonacci Products
 * Compute prod_{k=1}^{n} F_k mod m using Pisano period.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int pisano(int m) {
    int prev = 0, curr = 1;
    for (int i = 1; i <= m * m; i++) {
        int next = (prev + curr) % m;
        prev = curr; curr = next;
        if (prev == 0 && curr == 1) return i;
    }
    return -1;
}

ll fib_product_mod(ll n, int m) {
    int pi = pisano(m);
    vector<ll> Q(pi + 1, 1);
    int a = 0, b = 1;
    for (int k = 1; k <= pi; k++) {
        int next = (a + b) % m;
        a = b; b = next;
        Q[k] = Q[k-1] * a % m;
    }
    ll full = n / pi;
    int rem = n % pi;
    ll result = 1;
    // Q[pi]^full mod m
    ll base = Q[pi]; ll exp = full;
    while (exp > 0) {
        if (exp & 1) result = result * base % m;
        base = base * base % m;
        exp >>= 1;
    }
    result = result * Q[rem] % m;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Pisano Periods ===" << endl;
    for (int m = 2; m <= 20; m++)
        cout << "pi(" << m << ") = " << pisano(m) << endl;

    cout << "\n=== Products ===" << endl;
    for (int m : {7, 11, 13, 100, 1000}) {
        for (ll n : {10LL, 100LL, 1000LL}) {
            cout << "prod F_k mod " << m << " (n=" << n << ") = "
                 << fib_product_mod(n, m) << endl;
        }
    }

    int MOD = 1e9 + 7;
    cout << "\nAnswer: prod mod 10^9+7 (n=10^6) = " << fib_product_mod(1000000, MOD) << endl;
    return 0;
}
