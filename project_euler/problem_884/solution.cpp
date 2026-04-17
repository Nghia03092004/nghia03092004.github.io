/*
 * Problem 884: Lattice Walks with Barriers
 * Catalan numbers, ballot problem, LGV lemma.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll comb_val(int n, int r) {
    if (r < 0 || r > n) return 0;
    if (r > n - r) r = n - r;
    ll result = 1;
    for (int i = 0; i < r; i++)
        result = result * (n - i) / (i + 1);
    return result;
}

ll catalan(int n) { return comb_val(2*n, n) / (n + 1); }

ll ballot(int m, int n) {
    if (n > m) return 0;
    return comb_val(m + n, m) - comb_val(m + n, m + 1);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Catalan Numbers ===" << endl;
    for (int n = 0; n <= 15; n++)
        cout << "C_" << n << " = " << catalan(n) << endl;

    cout << "\n=== Ballot Problem ===" << endl;
    for (auto [m, n] : vector<pair<int,int>>{{3,3},{4,4},{5,5},{4,2},{6,3}}) {
        cout << "ballot(" << m << "," << n << ") = " << ballot(m, n) << endl;
    }

    cout << "\nAnswer: C_10 = " << catalan(10) << endl;
    return 0;
}
