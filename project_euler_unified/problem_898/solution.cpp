/*
 * Problem 898: Product Partition Function
 * Count unordered multiplicative partitions (factorizations) of n.
 * f(n) = number of ways to write n as unordered product of factors > 1.
 *
 * Uses recursive approach with minimum-factor constraint to avoid double counting.
 * f(p^a) = p(a) (additive partition function).
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

unordered_map<ll, ll> memo;

// Encode (n, min_factor) into single key for memoization
ll encode(ll n, ll m) { return n * 100001LL + m; }

ll factorizations(ll n, ll min_factor = 2) {
    if (n < min_factor) return 0;
    ll key = encode(n, min_factor);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    ll count = 1;  // n itself as single factor
    for (ll d = min_factor; d * d <= n; d++) {
        if (n % d == 0) {
            count += factorizations(n / d, d);
        }
    }
    return memo[key] = count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verification
    cout << "=== Multiplicative Partitions ===" << endl;
    vector<pair<int,int>> tests = {
        {2,1},{4,2},{6,2},{8,3},{12,4},{16,5},{24,7},{36,9},{48,12},{64,15}
    };
    for (auto [n, expected] : tests) {
        ll fn = factorizations(n);
        cout << "f(" << n << ") = " << fn
             << (fn == expected ? " OK" : " FAIL") << endl;
        assert(fn == expected);
    }

    // Prime powers: f(2^a) should equal partition function p(a)
    cout << "\n=== Prime Powers ===" << endl;
    int part[] = {0, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42};
    for (int a = 1; a <= 10; a++) {
        ll fn = factorizations(1LL << a);
        cout << "f(2^" << a << ") = " << fn
             << ", p(" << a << ") = " << part[a]
             << (fn == part[a] ? " OK" : " FAIL") << endl;
    }

    // Cumulative sum
    cout << "\n=== Cumulative Sum ===" << endl;
    for (int N : {10, 50, 100, 500, 1000}) {
        ll s = 0;
        for (int n = 2; n <= N; n++) s += factorizations(n);
        cout << "sum_{n=2}^{" << N << "} f(n) = " << s << endl;
    }

    ll ans = 0;
    for (int n = 2; n <= 1000; n++) ans += factorizations(n);
    cout << "\nAnswer: " << ans << endl;

    return 0;
}
