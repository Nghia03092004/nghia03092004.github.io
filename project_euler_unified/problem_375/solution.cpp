#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 375: Minimum of Subsequences
 *
 * S_0 = 290797, S_{n+1} = S_n^2 mod 50515093
 * M(N) = sum of min(S_i,...,S_j) for all 1 <= i <= j <= N
 * Compute M(2,000,000,000).
 *
 * The sequence has a cycle of length L=6308948 starting from index 1.
 * So S_n = S_{1 + ((n-1) mod L)} for n >= 1.
 *
 * N = 2e9: we have full_cycles = N / L and remainder = N % L.
 * 2000000000 / 6308948 = 317 full cycles with remainder.
 *
 * Strategy: use the monotone stack approach but exploit the periodicity.
 * After processing one full cycle, the stack state and running sums
 * can be used to compute subsequent cycles efficiently.
 *
 * However, the stack state at the end of each cycle may differ.
 * A simpler approach: since N/L ~ 317, we process 317+ cycles sequentially,
 * each of ~6.3M elements. Total: ~2e9 operations, feasible in C++ with O(N).
 *
 * Answer: 7435327983715286168
 */

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

int main(){
    const ll N = 2000000000LL;
    const ll MOD_SEQ = 50515093LL;

    // We generate the sequence on the fly and use the stack approach.
    // Total operations: O(N) = O(2e9), each operation is simple.
    // Should run in ~30-60 seconds with optimized C++.

    ll s_prev = 290797;

    // Monotone stack: use arrays for speed
    // Max stack size: bounded by N but practically much smaller
    static ll stk_val[7000000]; // values
    static ll stk_cnt[7000000]; // counts
    int stk_top = 0;

    lll T = 0;
    ll R = 0;

    for (ll j = 1; j <= N; j++) {
        ll s_cur = (s_prev * s_prev) % MOD_SEQ;
        s_prev = s_cur;

        ll cnt = 1;
        while (stk_top > 0 && stk_val[stk_top - 1] >= s_cur) {
            stk_top--;
            R -= stk_val[stk_top] * stk_cnt[stk_top];
            cnt += stk_cnt[stk_top];
        }
        R += s_cur * cnt;
        stk_val[stk_top] = s_cur;
        stk_cnt[stk_top] = cnt;
        stk_top++;

        T += R;
    }

    // Print __int128
    ll val = (ll)T;
    if (T == (lll)val) {
        printf("%lld\n", val);
    } else {
        // T might be larger than ll max
        ull uT = (ull)T;
        // Actually for this problem T fits in signed long long based on expected answer
        // But let's be safe
        ll hi = (ll)(T / 1000000000000000000LL);
        ll lo = (ll)(T % 1000000000000000000LL);
        if (hi > 0) printf("%lld%018lld\n", hi, lo);
        else printf("%lld\n", lo);
    }

    return 0;
}
