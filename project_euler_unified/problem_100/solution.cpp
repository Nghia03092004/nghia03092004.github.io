#include <bits/stdc++.h>
using namespace std;

int main() {
    // 2b(b-1) = n(n-1) reduces to 2x^2 - y^2 = 1 (x=2b-1, y=2n-1).
    // Second-order recurrence: b' = 6b - b_prev - 2, n' = 6n - n_prev - 2.
    // Initial: (b0,n0)=(1,1), (b1,n1)=(3,4).

    const long long T = 1000000000000LL;
    long long bp = 1, np = 1;
    long long bc = 3, nc = 4;

    while (nc <= T) {
        long long bn = 6 * bc - bp - 2;
        long long nn = 6 * nc - np - 2;
        bp = bc; np = nc;
        bc = bn; nc = nn;
    }

    cout << bc << endl;
    return 0;
}
