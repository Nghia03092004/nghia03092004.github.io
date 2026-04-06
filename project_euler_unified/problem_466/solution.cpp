#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

// P(m, n) = number of distinct values in {i*j : 1<=i<=m, 1<=j<=n}
//
// Approach: for each i from 1 to m, count the number of "new" values
// i*j (j in [1,n]) that haven't appeared in rows 1..i-1.
//
// Value v = i*j appears in row k (k<i) if k|v and v/k <= n.
// v = i*j, so k|i*j. Let g = gcd(k, i). Then k|i*j iff (k/g)|j.
// And v/k <= n iff i*j/k <= n iff j <= k*n/i.
//
// So v = i*j appeared in row k < i iff:
//   (k/gcd(k,i)) | j  AND  j <= k*n/i
//
// v is new in row i iff for ALL k in [1, i-1]:
//   NOT((k/gcd(k,i)) | j  AND  j <= k*n/i)
//
// This is: for all k < i, either (k/gcd(k,i)) does not divide j,
//          or j > k*n/i.
//
// For each k < i, let q_k = k/gcd(k,i) and L_k = floor(k*n/i).
// Then the condition for row k NOT capturing v is:
//   q_k does not divide j, OR j > L_k
// Equivalently: if q_k | j then j > L_k
// Equivalently: j is NOT a multiple of q_k in [1, L_k]
//
// So j is "new" iff j is not a multiple of q_k in [1, L_k] for any k < i.
// In other words: for each k < i with q_k | j, we need j > L_k.
//
// Regroup by the value of q = k/gcd(k,i):
// For a given q, the possible k values that give this q are those k < i
// with k/gcd(k,i) = q, i.e., k = q * gcd(k,i) = q * (some divisor of i).
// So k = q * d where d | i and d | k = q*d, so gcd(q*d, i) = d.
// This means gcd(q, i/d) = 1.
// And k < i means q*d < i, so d < i/q.
//
// For a given q, the tightest constraint is the maximum L_k = floor(k*n/i)
// over all valid k. Since L_k = floor(q*d*n/i), and d can be up to i/q - 1,
// the max d is the largest divisor of i with d < i/q and gcd(q, i/d) = 1.
// The max L_k is floor((max valid k) * n / i).
//
// Actually, let's think about it differently. For a given q (which is a
// potential divisor of j), the constraint is: if q | j, then for ALL k < i
// with q_k = q (or q_k | q), we need j > L_k.
//
// Wait, this is getting too complicated. Let me try a different, simpler approach.
//
// SIMPLER APPROACH:
// For small m, generate all "essential" divisor conditions.
//
// P(m, n) = n + sum_{i=2}^{m} (number of j in [1,n] such that i*j is NOT
//           in {k*j' : 1<=k<i, 1<=j'<=n})
//
// v = i*j is in row k < i iff k|v and v/k <= n.
// Since v = i*j, k|i*j. The divisors of i*j that are < i and fit are...
//
// Actually, let me just use inclusion-exclusion more carefully.
// For each i, define Bad_k = {j in [1,n] : q_k | j and j <= L_k}
//                          = {j : q_k | j, 1 <= j <= L_k}
// where q_k = k / gcd(k,i) and L_k = floor(k*n/i).
//
// New_i = n - |union of Bad_k for k=1..i-1|
//
// Many Bad_k are redundant. Bad_k1 is contained in Bad_k2 if
// q_{k2} | q_{k1} and L_{k1} <= L_{k2}. Actually the reverse:
// if q_{k1} | q_{k2} then multiples of q_{k2} in [1,L_{k2}] is a subset
// of multiples of q_{k1} in [1, L_{k1}] only if L_{k2} <= L_{k1}.
//
// This is still complex. Let me try to group by q value.
// For each q that divides j, the constraint from all k with q_k = q
// is: j > max over those k of L_k.

// For each divisor q of some number <= m, let
// T(i, q) = max_{k < i, k/gcd(k,i)=q} floor(k*n/i), or 0 if no such k exists.
// Then j is new iff for all q with q|j: j > T(i, q).

// The distinct q values that arise: q = k/gcd(k,i) for k < i.
// q must be such that q * d = k for some d | i, d < i/q, gcd(q, i/d) = 1.
// The possible q values are bounded by m.

// This is still complex for general inclusion-exclusion.
// Let me try a completely different, cleaner approach.

// CLEANEST APPROACH: Direct Mobius inversion
// P(m, n) = sum_{d=1}^{m} mu(d) * ... no, that doesn't directly work.

// Actually, the well-known formula:
// P(m, n) = sum_{i=1}^{m} sum_{j=1}^{n} [gcd(i,j)=1 or the "first time" condition]
//
// No. Let's think about it as:
// P(m, n) = |{v : 1 <= v <= m*n, smallest(v, m) exists}|
// where smallest(v, m) = min{d : d|v, d <= m, v/d <= n}
//
// This equals sum_{i=1}^{m} |{v : smallest(v,m) = i}|
// = sum_{i=1}^{m} |{j in [1,n] : for all d|v with d<i, v/d > n, where v=i*j}|

// For d|v with d < i: d|i*j, d < i.
// v/d > n means i*j/d > n, i.e., j > d*n/i... wait: i*j/d > n => j > d*n/i.
// No: i*j/d > n => j > (d*n)/(i)... no again. i*j/d > n => j > d*n/i?
// i*j > d*n => j > d*n/i. Yes.
// But d < i so d/i < 1, so d*n/i < n. So the constraint is j > d*n/i for
// certain d|i*j.

// This is essentially a lattice point counting problem. For small m,
// we can solve it exactly with careful computation.

// Let me try computing P(m, n) as:
// P(m, n) = sum_{a=1}^{m} phi_m(a, n)
// where phi_m(a, n) = count of j in [1, n] such that the smallest divisor
// of a*j that is <= m is exactly a.

// For a = 1: smallest divisor of j that is <= m is always >= 1, and 1|j,
// j/1 = j <= n. So smallest is 1. phi_m(1, n) = n.
// For a = 2: need j such that 2j has no divisor d=1 with 2j/1 <= n.
// But 2j/1 = 2j. We need 2j > n? No wait, we need for d=1: d|2j (yes, 1|2j)
// and 2j/1 <= n iff 2j <= n iff j <= n/2. So for j <= n/2, the value 2j
// already appeared in row 1 (as 1 * 2j). So new values from row 2 are
// j > n/2, i.e., j in [floor(n/2)+1, n]. Count = n - floor(n/2) = ceil(n/2).

// For a = 3: need j such that 3j has no divisor d < 3 with 3j/d <= n.
// d=1: 3j <= n iff j <= n/3. So if j <= n/3, not new (appeared in row 1).
// d=2: 2|3j iff 2|j (since gcd(2,3)=1). And 3j/2 <= n iff j <= 2n/3.
// So if 2|j and j <= 2n/3, not new (appeared in row 2 as 2 * (3j/2)).
//
// New values: j > n/3 AND NOT(2|j AND j <= 2n/3)
// = j > n/3 AND (j is odd OR j > 2n/3)
// = (j > n/3 AND j odd) + (j > 2n/3 AND j even)

// This is getting very case-specific. For m=64, we'd need to automate this.
// The key observation is that for each row a, the "blocking" conditions
// from earlier rows form a set of conditions on j that can be described
// using a collection of (divisor, threshold) pairs.

// Let me implement this properly.

struct Condition {
    int q; // j must be divisible by q
    ll L;  // j must be <= L for this to block
    // Condition: if q | j and j <= L, then j is blocked
};

// For row a, compute all blocking conditions from rows 1..a-1
vector<Condition> get_conditions(int a, int m, ll n) {
    // For each k < a, if there exists a way k | (a*j):
    // q_k = k / gcd(k, a), L_k = floor(k * n / a)
    // Condition: q_k | j AND j <= L_k => blocked

    map<int, ll> q_to_maxL; // for each q, keep the maximum L

    for (int k = 1; k < a; k++) {
        int g = __gcd(k, a);
        int q = k / g;
        ll L = (ll)k * n / a; // floor(k*n/a)
        // But we need to be careful: k*n might overflow ll
        // k < 64, n <= 10^16, so k*n <= 64 * 10^16 < 6.4 * 10^17, fits in ll
        if (L <= 0) continue;
        if (q_to_maxL.find(q) == q_to_maxL.end() || L > q_to_maxL[q]) {
            q_to_maxL[q] = L;
        }
    }

    vector<Condition> conds;
    for (auto& [q, L] : q_to_maxL) {
        conds.push_back({q, L});
    }
    return conds;
}

// Count j in [1, n] that satisfy ALL conditions being "not blocked"
// i.e., for each condition (q, L): NOT(q|j AND j <= L)
// which means: q does not divide j, OR j > L
//
// Equivalently: j is NOT in any of the "blocked" sets B_c = {j : q_c | j, j <= L_c}
// So count = n - |union of B_c|
//
// |union of B_c| by inclusion-exclusion over subsets of conditions:
// sum over non-empty S: (-1)^{|S|+1} * |intersection of B_c for c in S|
//
// Intersection: j divisible by lcm of all q_c in S, and j <= min of all L_c in S
// Count = floor(min_L / lcm_q)

ll count_new(int a, int m, ll n) {
    vector<Condition> conds = get_conditions(a, m, n);

    // Remove redundant conditions: if q1 | q2 and L1 >= L2, then B_{q2,L2} subset of B_{q1,L1}
    // So condition (q2, L2) is redundant.
    // Actually we keep all and let inclusion-exclusion handle it, but we should
    // prune to reduce subset count.

    // Sort by q
    // Remove conditions where q_c has a divisor q' among the conditions with L' >= L_c
    // (because multiples of q_c in [1, L_c] are a subset of multiples of q' in [1, L'])

    // First, for conditions with q1 | q2 and L1 >= L2, condition 2 is redundant
    vector<Condition> pruned;
    for (auto& c : conds) {
        bool redundant = false;
        for (auto& c2 : conds) {
            if (c2.q != c.q && c.q % c2.q == 0 && c2.L >= c.L) {
                redundant = true;
                break;
            }
        }
        if (!redundant) pruned.push_back(c);
    }
    conds = pruned;

    int sz = conds.size();

    if (sz == 0) return n;
    if (sz > 25) {
        // This shouldn't happen for m=64 after pruning, but just in case
        // Use a different method
        cerr << "WARNING: too many conditions for a=" << a << " sz=" << sz << endl;
    }

    // Inclusion-exclusion
    ll blocked = 0;
    for (ll mask = 1; mask < (1LL << sz); mask++) {
        lll lcm_val = 1;
        ll min_L = LLONG_MAX;
        int bits = __builtin_popcountll(mask);
        bool overflow = false;

        for (int i = 0; i < sz; i++) {
            if (mask & (1LL << i)) {
                lll g = __gcd(lcm_val, (lll)conds[i].q);
                lll new_lcm = lcm_val / g * conds[i].q;
                if (new_lcm > (lll)n) { overflow = true; break; }
                lcm_val = new_lcm;
                min_L = min(min_L, conds[i].L);
            }
        }

        if (overflow) continue;
        ll lv = (ll)lcm_val;
        ll cnt = min_L / lv;

        if (bits % 2 == 1) blocked += cnt;
        else blocked -= cnt;
    }

    return n - blocked;
}

ll P_fast(int m, ll n) {
    ll answer = 0;
    for (int a = 1; a <= m; a++) {
        ll c = count_new(a, m, n);
        answer += c;
    }
    return answer;
}

// Direct computation for verification
ll P_direct(int m, ll n) {
    set<ll> vals;
    for (int i = 1; i <= m; i++)
        for (ll j = 1; j <= n; j++)
            vals.insert((ll)i * j);
    return (ll)vals.size();
}

int main() {
    ios_base::sync_with_stdio(false);

    // Verify with small cases
    cerr << "P_direct(3,4) = " << P_direct(3, 4) << endl;
    cerr << "P_fast(3,4) = " << P_fast(3, 4) << endl;

    cerr << "P_direct(64,64) = " << P_direct(64, 64) << endl;
    cerr << "P_fast(64,64) = " << P_fast(64, 64) << endl;

    // Check condition counts
    for (int a = 1; a <= 64; a++) {
        auto conds = get_conditions(a, 64, 10000000000000000LL);
        // Prune
        vector<Condition> pruned;
        for (auto& c : conds) {
            bool redundant = false;
            for (auto& c2 : conds) {
                if (c2.q != c.q && c.q % c2.q == 0 && c2.L >= c.L) {
                    redundant = true;
                    break;
                }
            }
            if (!redundant) pruned.push_back(c);
        }
        if ((int)pruned.size() > 20) {
            cerr << "a=" << a << " pruned conds=" << pruned.size() << endl;
        }
    }

    // P(12, 345) = 1998
    cerr << "P_fast(12,345) = " << P_fast(12, 345) << endl;

    // P(32, 10^15) = 13826382602124302
    cerr << "P_fast(32,10^15) = " << P_fast(32, 1000000000000000LL) << endl;

    // P(64, 10^16)
    ll answer = P_fast(64, 10000000000000000LL);
    cout << answer << endl;

    return 0;
}
