/*
 * Problem 881: Divisor Graph Coloring
 * chi(G_n) = floor(log2(n)) + 1 by Dilworth's theorem.
 * The longest chain 1, 2, 4, ..., 2^k determines the chromatic number.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int chromatic_number(ll n) {
    if (n <= 0) return 0;
    int chi = 0;
    while ((1LL << chi) <= n) chi++;
    return chi;
}

ll edge_count(int n) {
    ll total = 0;
    for (int k = 1; k <= n; k++)
        total += n / k - 1;
    return total;
}

bool verify_coloring(int n) {
    // color(k) = floor(log2(k))
    vector<int> color(n + 1);
    for (int k = 1; k <= n; k++) {
        int c = 0;
        while ((1 << (c + 1)) <= k) c++;
        color[k] = c;
    }
    for (int i = 1; i <= n; i++)
        for (int j = 2 * i; j <= n; j += i)
            if (color[i] == color[j]) return false;
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Chromatic Number ===" << endl;
    for (ll n : {1LL,2LL,4LL,8LL,10LL,16LL,100LL,1000LL,(ll)1e9,(ll)1e18}) {
        cout << "chi(G_" << n << ") = " << chromatic_number(n) << endl;
    }

    cout << "\n=== Coloring Verification ===" << endl;
    for (int n : {4, 8, 16, 32, 50}) {
        bool ok = verify_coloring(n);
        cout << "n=" << n << ": " << (ok ? "OK" : "FAIL") << endl;
        assert(ok);
    }

    cout << "\n=== Edge Counts ===" << endl;
    for (int n : {10, 50, 100, 500}) {
        cout << "|E(G_" << n << ")| = " << edge_count(n) << endl;
    }

    cout << "\nAnswer: chi(G_{10^18}) = " << chromatic_number((ll)1e18) << endl;
    return 0;
}
