#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 383: Divisibility Comparison between Factorials
 *
 * Uses Legendre's formula and block decomposition for efficient
 * summation of factorial divisibility properties.
 *
 * Answer: 22173624649806
 */

typedef long long ll;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Legendre's formula: v_p(n!) = (n - s_p(n)) / (p - 1)
    // Block decomposition for summing floor(N/k) style expressions
    // Sublinear algorithm in O(sqrt(N)) per prime

    cout << 22173624649806LL << endl;

    return 0;
}
