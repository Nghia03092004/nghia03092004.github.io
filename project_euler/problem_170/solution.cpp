#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 170: Largest pandigital concatenated product.
 *
 * Products n*c_i concatenated = pandigital (0-9, 10 digits)
 * n || c_1 || ... || c_m concatenated = pandigital (0-9, 10 digits), m >= 2
 *
 * Answer: 9857164023
 *
 * Approach: for each n (small, 1-4 digits typically), enumerate c values
 * with digits from available pool. Compute products, check digit constraints.
 * Use bitmask DP on (input_mask_used, output_mask_used).
 */

long long p10[11];

int dmask(long long x) {
    if (x == 0) return 1;
    int m = 0;
    while (x > 0) {
        int d = x % 10;
        if (m & (1<<d)) return -1;
        m |= (1<<d);
        x /= 10;
    }
    return m;
}

int ndig(long long x) {
    int c = 0; while (x) { c++; x /= 10; } return c ? c : 1;
}

const int FULL = (1<<10)-1;

int main(){
    p10[0]=1;
    for(int i=1;i<=10;i++) p10[i]=p10[i-1]*10;

    long long best = 0;

    // n can be any positive integer. Its digits must be distinct.
    // n uses d_n digits. Multipliers use 10-d_n digits. Need 10-d_n >= 2.
    // So d_n <= 8. But practically, n*c must have reasonable number of digits,
    // and products concatenated = 10 digits. So n is usually small.

    // For each n from 1 to ~10000 (4 digits max for reasonable search)
    for (long long n = 2; n <= 9999; n++) {
        int nm = dmask(n);
        if (nm < 0) continue;
        int nnd = ndig(n);
        int avail = FULL ^ nm;
        int avail_nd = 10 - nnd;
        if (avail_nd < 2) continue;

        // Enumerate c: positive integer whose digits are subset of 'avail', no repeats
        // c from 1 to ... we limit by product size: n*c < 10^10
        // For n=2, c < 5*10^9. Too many.
        // Better: enumerate c by DFS on available digits.

        // But DFS on available digits generates up to P(avail_nd, 1) + P(avail_nd, 2) + ... numbers
        // For avail_nd=8: sum of P(8,k) for k=1..8 = 8+56+336+1680+6720+20160+40320+40320 = 109600
        // For avail_nd=9: about 986000. Manageable.

        struct CandPair {
            int cm, pm, pnd;
            long long pv;
        };

        // Use DFS to enumerate c values
        vector<CandPair> cpairs;

        struct F { long long v; int m; int d; };
        vector<F> stk;
        for (int d = 0; d <= 9; d++) {
            if (!(avail & (1<<d))) continue;
            if (d == 0) continue; // no leading zero for c
            stk.push_back({(long long)d, 1<<d, 1});
        }

        while (!stk.empty()) {
            auto [v, m, d] = stk.back();
            stk.pop_back();

            long long prod = n * v;
            int pm = dmask(prod);
            if (pm >= 0) {
                int pnd = ndig(prod);
                if (__builtin_popcount(pm) == pnd) {
                    cpairs.push_back({m, pm, pnd, prod});
                }
            }

            if (d < avail_nd) {
                for (int dd = 0; dd <= 9; dd++) {
                    if (!(avail & (1<<dd))) continue;
                    if (m & (1<<dd)) continue;
                    stk.push_back({v*10+dd, m|(1<<dd), d+1});
                }
            }
        }

        if ((int)cpairs.size() < 2) continue;

        // Keep best product per (cm, pm) pair
        unordered_map<long long, pair<long long,int>> best_cp; // key=cm*1024+pm -> (pv, pnd)
        for (auto& cp : cpairs) {
            long long key = (long long)cp.cm * 1024 + cp.pm;
            auto it = best_cp.find(key);
            if (it == best_cp.end() || cp.pv > it->second.first)
                best_cp[key] = {cp.pv, cp.pnd};
        }

        // DP: state = (cu, pu) -> (best_concat_val, nd, nseg)
        // Transition: add one (cm, pm) pair with no overlap
        // Process by increasing popcount of cu

        // Use array indexed by cu (subset of avail) and pu (subset of FULL)
        // Too large: 2^avail_nd * 1024. For avail_nd=9: 512*1024 = 512k. Possible.
        // But we have avail with specific bits set. Let's use unordered_map.

        struct DPV { long long val; int nd; int nseg; };
        unordered_map<long long, DPV> dp;

        // Init: single pairs
        for (auto& [key, val] : best_cp) {
            auto it = dp.find(key);
            if (it == dp.end() || val.first > it->second.val)
                dp[key] = {val.first, val.second, 1};
        }

        // Combine: iterate over existing states and try adding each pair
        // Sort by popcount of cu for correct ordering
        // We do multiple passes until no change
        // Max passes = max number of segments ~ 10
        for (int pass = 0; pass < 9; pass++) {
            bool changed = false;
            vector<pair<long long, DPV>> entries(dp.begin(), dp.end());

            for (auto& [ekey, eval] : entries) {
                int ecu = ekey / 1024;
                int epu = ekey % 1024;

                for (auto& [pkey, pval] : best_cp) {
                    int pcm = pkey / 1024;
                    int ppm = pkey % 1024;
                    if (ecu & pcm) continue;
                    if (epu & ppm) continue;

                    int ncu = ecu | pcm;
                    int npu = epu | ppm;
                    long long nkey = (long long)ncu * 1024 + npu;

                    long long c1 = eval.val * p10[pval.second] + pval.first;
                    long long c2 = pval.first * p10[eval.nd] + eval.val;
                    long long bv = max(c1, c2);
                    int nnd = eval.nd + pval.second;
                    int ns = eval.nseg + 1;

                    auto it = dp.find(nkey);
                    if (it == dp.end() || bv > it->second.val) {
                        dp[nkey] = {bv, nnd, ns};
                        changed = true;
                    }
                }
            }
            if (!changed) break;
        }

        long long target = (long long)avail * 1024 + FULL;
        auto it = dp.find(target);
        if (it != dp.end() && it->second.nseg >= 2 && it->second.nd == 10) {
            if (it->second.val > best) {
                best = it->second.val;
                fprintf(stderr, "n=%lld: %lld (nseg=%d)\n", n, best, it->second.nseg);
            }
        }
    }

    cout << best << endl;
    return 0;
}
