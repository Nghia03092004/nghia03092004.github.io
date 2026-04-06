#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 631: Constrained Permutations
 *
 * Compute the permanent of a 0-1 matrix via Ryser's formula:
 * perm(A) = (-1)^n * sum_{S} (-1)^|S| * prod_i (sum_{j in S} A_ij)
 *
 * Complexity: O(2^n * n)
 */

ll permanent_ryser(vector<vector<int>>& A) {
    int n = A.size();
    ll total = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int bits = __builtin_popcount(mask);
        ll prod = 1;
        for (int i = 0; i < n; i++) {
            int s = 0;
            for (int j = 0; j < n; j++)
                if (mask & (1 << j))
                    s += A[i][j];
            prod *= s;
        }
        if ((n - bits) % 2 == 0) total += prod;
        else total -= prod;
    }
    if (n % 2 == 1) total = -total;
    return total;
}

// Brute force (verification for small n)
ll permanent_brute(vector<vector<int>>& A) {
    int n = A.size();
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    ll total = 0;
    do {
        ll prod = 1;
        for (int i = 0; i < n; i++) prod *= A[i][perm[i]];
        total += prod;
    } while (next_permutation(perm.begin(), perm.end()));
    return total;
}

int main() {
    // Test: identity matrix
    vector<vector<int>> I3 = {{1,0,0},{0,1,0},{0,0,1}};
    assert(permanent_ryser(I3) == 1);
    assert(permanent_brute(I3) == 1);

    // Test: all-ones matrix
    vector<vector<int>> J3 = {{1,1,1},{1,1,1},{1,1,1}};
    assert(permanent_ryser(J3) == 6);
    assert(permanent_brute(J3) == 6);

    // Random tests
    srand(42);
    for (int trial = 0; trial < 50; trial++) {
        int n = 2 + rand() % 5;
        vector<vector<int>> A(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                A[i][j] = rand() % 2;
        assert(permanent_ryser(A) == permanent_brute(A));
    }

    cout << "All verifications passed." << endl;
    return 0;
}
