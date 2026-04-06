#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 167: Investigating Ulam Sequences
 *
 * For U(2, 2n+1) with n=2..10, find the 10^11-th term and sum them.
 * Answer: 3916160068885
 *
 * Key insight: U(2,v) for odd v>=5 has exactly 2 even members: 2 and some e.
 * After that, all members are odd. Odd c is Ulam iff is_ulam[c-2] XOR is_ulam[c-e].
 * This gives O(1) generation per term.
 *
 * The difference sequence is eventually periodic. For small periods we detect
 * them via non-2 diffs. For large periods (U(2,21) with period 2^21), we use
 * the known period from OEIS A100729 and verify it.
 */

const int MAXVAL = 60000000;

// Known periods from OEIS A100729 for U(2, 2n+1), n=2..10
int known_periods[] = {0, 0, 32, 26, 444, 1628, 5906, 80, 126960, 380882, 2097152};

long long solve_ulam(int a, int b, long long k, int nn) {
    // Phase 1: Find second even member
    int SMALL = 100000;
    vector<short> rc(SMALL + 1, 0);
    vector<int> isq = {a, b};
    vector<bool> isu(SMALL + 1, false);
    isu[a] = isu[b] = true;
    if (a + b <= SMALL) rc[a + b]++;

    int second_even = -1;
    int nc2 = b + 1;
    while (nc2 <= SMALL) {
        while (nc2 <= SMALL && rc[nc2] != 1) nc2++;
        if (nc2 > SMALL) break;
        int u = nc2;
        isq.push_back(u);
        isu[u] = true;
        if (u % 2 == 0 && u != a) second_even = u;
        for (int i = 0; i < (int)isq.size() - 1; i++) {
            int s = isq[i] + u;
            if (s <= SMALL && rc[s] < 3) rc[s]++;
        }
        nc2 = u + 1;
        if (second_even > 0) break;
    }

    fprintf(stderr, "U(%d,%d): second_even=%d\n", a, b, second_even);

    // Phase 2: Fast generation using XOR rule
    int kp = known_periods[nn];
    int needed_terms = 6 * kp + 10000; // enough to detect and verify
    if (needed_terms > 14000000) needed_terms = 14000000;
    if (needed_terms < 100000) needed_terms = 100000;

    vector<bool> is_ulam(MAXVAL + 2, false);
    vector<int> seq;
    seq.reserve(needed_terms + 100);
    for (int x : isq) {
        is_ulam[x] = true;
        seq.push_back(x);
    }

    int c = isq.back();
    if (c % 2 == 0) c++;
    else c += 2;

    while (c <= MAXVAL && (int)seq.size() < needed_terms) {
        bool w1 = (c - 2 >= 0 && c - 2 <= MAXVAL && is_ulam[c - 2]);
        bool w2 = (c - second_even >= 0 && c - second_even <= MAXVAL && is_ulam[c - second_even]);
        if (w1 != w2) {
            seq.push_back(c);
            is_ulam[c] = true;
        }
        c += 2;
    }

    int n = seq.size();
    fprintf(stderr, "  generated %d terms, last=%d\n", n, seq[n-1]);

    // Compute diffs
    int nd = n - 1;
    vector<int> diffs(nd);
    for (int i = 0; i < nd; i++) diffs[i] = seq[i+1] - seq[i];

    // Try to detect period using non-2 diffs first (fast for small periods)
    int full_period = -1;

    if (kp <= 500000) {
        // Use non-2 diff approach
        vector<int> non2_val, non2_pos;
        for (int i = 0; i < nd; i++) {
            if (diffs[i] != 2) {
                non2_val.push_back(diffs[i]);
                non2_pos.push_back(i);
            }
        }
        int nn2 = non2_val.size();

        for (int p = 1; p <= nn2 / 6; p++) {
            int b2 = nn2 - p;
            bool ok = true;
            for (int rep = 1; rep <= 4 && ok; rep++) {
                if (b2 - rep * p < 0) { ok = false; break; }
                for (int j = 0; j < p && ok; j++)
                    if (non2_val[b2 + j] != non2_val[b2 - rep * p + j]) ok = false;
            }
            if (ok) {
                int prev_b = b2 - p;
                int fp = non2_pos[b2] - non2_pos[prev_b];

                // Verify
                int ei2 = nd - 1, fb2 = ei2 - fp + 1;
                if (fb2 - 2 * fp >= 0) {
                    bool v = true;
                    for (int rep = 1; rep <= 2 && v; rep++)
                        for (int j = 0; j < fp && v; j++)
                            if (diffs[fb2+j] != diffs[fb2-rep*fp+j]) v = false;
                    if (v) { full_period = fp; break; }
                }
            }
        }
    }

    // If not found, try the known period directly
    if (full_period < 0) {
        int fp = kp;
        int ei2 = nd - 1, fb2 = ei2 - fp + 1;
        if (fb2 - 4 * fp >= 0) {
            bool v = true;
            for (int rep = 1; rep <= 4 && v; rep++)
                for (int j = 0; j < fp && v; j++)
                    if (diffs[fb2+j] != diffs[fb2-rep*fp+j]) v = false;
            if (v) full_period = fp;
        }
    }

    if (full_period < 0) {
        fprintf(stderr, "  FAILED to find/verify period\n");
        return -1;
    }

    // Find start of periodicity
    int ei = nd - 1, fb = ei - full_period + 1;
    int pstart = fb;
    while (pstart >= full_period) {
        bool m = true;
        for (int j = 0; j < full_period && m; j++)
            if (diffs[pstart-full_period+j] != diffs[pstart+j]) m = false;
        if (m) pstart -= full_period;
        else break;
    }

    long long psum = 0;
    for (int j = 0; j < full_period; j++) psum += diffs[pstart + j];

    // Verify extrapolation
    {
        long long tidx = n - 1;
        long long off = tidx - pstart;
        long long q2 = off / full_period;
        long long r = off % full_period;
        long long pred = (long long)seq[pstart+(int)r] + q2 * psum;
        if (pred != seq[n-1]) {
            fprintf(stderr, "  extrapolation FAILED\n");
            return -1;
        }
    }

    fprintf(stderr, "  period=%d, start=%d, psum=%lld\n", full_period, pstart, psum);

    long long idx = k - 1;
    if (idx < n) return seq[idx];
    long long offset = idx - pstart;
    long long q2 = offset / full_period;
    long long r = offset % full_period;
    return (long long)seq[pstart+(int)r] + q2 * psum;
}

int main(){
    long long K = 100000000000LL;
    long long total = 0;
    for (int nn = 2; nn <= 10; nn++) {
        int v = 2 * nn + 1;
        long long val = solve_ulam(2, v, K, nn);
        if (val < 0) {
            fprintf(stderr, "Failed for U(2,%d)\n", v);
            return 1;
        }
        fprintf(stderr, "  U(2,%d)(10^11) = %lld\n\n", v, val);
        total += val;
    }
    cout << total << endl;
    return 0;
}
