#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 916: Graph Isomorphism Counting
 *
 * Count non-isomorphic simple graphs on n=7 vertices.
 *
 * Approach: Burnside's lemma with S_n acting on graphs.
 *   count = (1/n!) * sum_{pi in S_n} 2^{c(pi)}
 * where c(pi) = number of edge orbits under pi.
 *
 * For a permutation with cycle type (l1, l2, ...):
 *   c = sum_{i<j} gcd(l_i, l_j) + sum_i floor(l_i / 2)
 *
 * We enumerate partitions of n, compute the contribution of each
 * conjugacy class, and sum.
 *
 * OEIS A000088: 1, 1, 2, 4, 11, 34, 156, 1044, 12346, ...
 */

typedef long long ll;
typedef vector<int> vi;

ll factorial(int n) {
    ll r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

// Number of permutations with given cycle type
ll count_perms(int n, const vi& cycle_type) {
    map<int, int> freq;
    for (int l : cycle_type) freq[l]++;
    ll denom = 1;
    for (auto& [len, cnt] : freq) {
        for (int i = 0; i < cnt; i++) denom *= len;
        denom *= factorial(cnt);
    }
    return factorial(n) / denom;
}

// Number of edge orbits from cycle type
int edge_orbits(const vi& ct) {
    int c = 0;
    int k = ct.size();
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
            c += __gcd(ct[i], ct[j]);
        }
        c += ct[i] / 2;
    }
    return c;
}

// Generate all partitions of n
void gen_partitions(int n, int max_val, vi& current, vector<vi>& result) {
    if (n == 0) {
        result.push_back(current);
        return;
    }
    for (int v = min(n, max_val); v >= 1; v--) {
        current.push_back(v);
        gen_partitions(n - v, v, current, result);
        current.pop_back();
    }
}

ll count_graphs(int n) {
    vector<vi> parts;
    vi cur;
    gen_partitions(n, n, cur, parts);

    ll total = 0;
    for (auto& p : parts) {
        int c = edge_orbits(p);
        ll np = count_perms(n, p);
        total += np * (1LL << c);
    }
    return total / factorial(n);
}

int main() {
    int n = 7;
    ll answer = count_graphs(n);
    cout << answer << endl;

    // Verification against known values
    assert(count_graphs(1) == 1);
    assert(count_graphs(2) == 2);
    assert(count_graphs(3) == 4);
    assert(count_graphs(4) == 11);
    assert(count_graphs(5) == 34);
    assert(count_graphs(6) == 156);
    assert(count_graphs(7) == 1044);

    return 0;
}
