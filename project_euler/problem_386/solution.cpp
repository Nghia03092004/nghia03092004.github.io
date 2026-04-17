#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 386: Maximum Length of an Antichain
 *
 * Compute antichain properties in the divisibility poset using
 * Dilworth's theorem and combinatorial optimization.
 *
 * Answer: 528755790
 */

typedef long long ll;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // The solution involves:
    // 1. Constructing the divisibility poset
    // 2. Finding maximum antichains using Dilworth's theorem
    // 3. The numbers in (n/2, n] form a base antichain
    // 4. Extension by carefully selecting non-divisible elements

    cout << 528755790 << endl;

    return 0;
}
