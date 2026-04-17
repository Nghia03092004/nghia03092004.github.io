#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 733: Ascending Subsequences
 *
 * Sum of all elements in all length-4 ascending subsequences of a_i = 153^i mod 10000019.
 * Uses Fenwick trees for O(n log n) computation.
 */

const int MODP = 1000000007;
const int SEQ_MOD = 10000019;

struct BIT {
    int n;
    vector<long long> tree;
    BIT(int n) : n(n), tree(n + 1, 0) {}
    void update(int i, long long v) {
        for (; i <= n; i += i & -i) tree[i] = (tree[i] + v) % MODP;
    }
    long long query(int i) {
        long long s = 0;
        for (; i > 0; i -= i & -i) s = (s + tree[i]) % MODP;
        return s;
    }
};

int main() {
    int n = 1000000;
    vector<long long> a(n + 1);
    a[1] = 153;
    for (int i = 2; i <= n; i++) a[i] = a[i-1] * 153 % SEQ_MOD;

    // Coordinate compression
    vector<long long> sorted_a(a.begin() + 1, a.end());
    sort(sorted_a.begin(), sorted_a.end());
    sorted_a.erase(unique(sorted_a.begin(), sorted_a.end()), sorted_a.end());
    int sz = sorted_a.size();
    auto compress = [&](long long v) {
        return lower_bound(sorted_a.begin(), sorted_a.end(), v) - sorted_a.begin() + 1;
    };

    vector<BIT> cnt(5, BIT(sz)), sm(5, BIT(sz));
    long long total = 0;

    for (int j = 1; j <= n; j++) {
        int v = compress(a[j]);
        for (int k = 4; k >= 1; k--) {
            long long ck, sk;
            if (k == 1) {
                ck = 1;
                sk = a[j] % MODP;
            } else {
                ck = cnt[k-1].query(v - 1);
                sk = (sm[k-1].query(v - 1) + a[j] % MODP * ck) % MODP;
            }
            cnt[k].update(v, ck);
            sm[k].update(v, sk);
            if (k == 4) total = (total + sk) % MODP;
        }
    }

    printf("%lld\n", total);
    return 0;
}
