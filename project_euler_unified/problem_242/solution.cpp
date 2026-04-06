/*
 * Project Euler Problem 242: Odd Triplets
 *
 * Count triplets (a,b,c) with 0 < a <= b <= c, a+b+c <= N, a^b^c = 0.
 * N = 10^12, answer mod 10^9.
 *
 * Digit DP on the binary representation of N.
 * Valid bit patterns: (0,0,0), (0,1,1), (1,0,1), (1,1,0).
 * State: (bit position, carry, tight, ordering).
 */

#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000000LL;

int bits[50];
int nbits;
map<tuple<int,int,int,int>, long long> memo;

int patterns[4][3] = {{0,0,0},{0,1,1},{1,0,1},{1,1,0}};

long long solve(int pos, int carry, int tight, int order) {
    if (pos < 0)
        return carry == 0 ? 1 : 0;

    auto key = make_tuple(pos, carry, tight, order);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    long long result = 0;
    int n_bit = bits[pos];

    for (int p = 0; p < 4; p++) {
        int ai = patterns[p][0], bi = patterns[p][1], ci = patterns[p][2];

        bool a_eq_b = (order == 0 || order == 1);
        bool b_eq_c = (order == 0 || order == 2);

        if (a_eq_b && ai > bi) continue;
        if (b_eq_c && bi > ci) continue;

        bool new_a_lt_b = (order == 2 || order == 3) || (ai < bi);
        bool new_b_lt_c = (order == 1 || order == 3) || (bi < ci);

        int new_order;
        if (!new_a_lt_b && !new_b_lt_c) new_order = 0;
        else if (!new_a_lt_b && new_b_lt_c) new_order = 1;
        else if (new_a_lt_b && !new_b_lt_c) new_order = 2;
        else new_order = 3;

        int s = ai + bi + ci + carry;
        int sum_bit = s % 2;
        int new_carry = s / 2;

        int new_tight;
        if (tight) {
            if (sum_bit > n_bit) continue;
            new_tight = (sum_bit == n_bit) ? 1 : 0;
        } else {
            new_tight = 0;
        }

        result = (result + solve(pos - 1, new_carry, new_tight, new_order)) % MOD;
    }

    memo[key] = result;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);

    long long N_val = 1000000000000LL;
    nbits = 0;
    long long tmp = N_val;
    while (tmp > 0) {
        bits[nbits++] = tmp & 1;
        tmp >>= 1;
    }

    long long ans = solve(nbits - 1, 0, 1, 0);
    ans = (ans - 1 + MOD) % MOD;
    cout << ans << endl;
    return 0;
}
