#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vl;

// Problem 269: Count n in [1, 10^16 - 1] such that the digit polynomial
// p_n(x) = a_0 + a_1*x + ... + a_k*x^k has at least one integer root,
// where a_0, a_1, ..., a_k are the digits of n (a_0 = units digit).
// n can have up to 16 digits.
//
// Possible integer roots: since coefficients are 0-9 and at least one is > 0,
// - d >= 1: p(d) >= a_0 >= 0 and p(d) > 0 if any a_i > 0. So no positive root works
//   (unless all non-constant terms are 0 and a_0 = 0, but then n=0 excluded).
//   Actually p(1) = sum of digits > 0 for n >= 1. p(d) > 0 for d >= 1.
//   Wait: p(d) = a_0 + a_1*d + ... + a_k*d^k. If d >= 1 and all a_i >= 0 and some a_i > 0,
//   then p(d) > 0. So no positive integer roots.
// - d = 0: p(0) = a_0 = 0 iff n ends in 0.
// - d < 0: d in {-1, -2, ..., -9}. For |d| >= 10, p(d) is dominated by the leading term
//   a_k * d^k which for k >= 1 has |a_k * d^k| >> sum of other terms when |d| >= 10.
//   More precisely, |a_k * d^k| >= 1 * 10^k while |sum_{i<k} a_i * d^i| <= 9*(10^k-1)/9 = 10^k - 1.
//   So |p(d)| >= 10^k - (10^k - 1) = 1 > 0. Hence no root with |d| >= 10.
//
// So possible roots are: 0, -1, -2, ..., -9.
//
// We use inclusion-exclusion over subsets of roots S in {0, -1, -2, ..., -9}.
// For each subset S, count polynomials with digits as coefficients where ALL elements of S are roots.
//
// For root 0: a_0 = 0 (n divisible by 10).
// For root -d (d > 0): p(-d) = a_0 - a_1*d + a_2*d^2 - ... = 0.
//
// If -d is a root, then (x + d) divides p(x).
// If multiple -d_1, ..., -d_k are roots, then prod(x + d_i) divides p(x).
//
// For a given set of roots S = {-d_1, ..., -d_k} (possibly including 0):
// Let P_S(x) = prod_{d in S} (x + d) (where d = 0 for root 0, d = 1,...,9 for roots -1,...,-9).
// p(x) = P_S(x) * q(x) for some polynomial q(x) with integer coefficients.
// The coefficients of p must be in [0, 9].
//
// We count the number of valid p(x) of degree <= 15 (16 digits max) with coefficients in [0,9],
// divisible by P_S(x), and p not identically 0.
//
// Method: Polynomial long division DP.
// p(x) = P_S(x) * q(x). Let deg(P_S) = s, deg(q) = m = 15 - s (or up to 15 - s).
// Actually p can have degree 0 to 15. If p has degree d, then q has degree d - s.
// We need d >= s.
// Also, n ranges from 1 to 10^16 - 1, so the number of digits is 1 to 16.
// A number with k digits (k >= 1) has leading digit a_{k-1} in [1,9] and other digits in [0,9].
// But actually we count ALL n from 1 to 10^16 - 1. The polynomial for n is determined by
// n's digits, including leading zeros (but n doesn't have leading zeros as a number).
// So n with k digits gives polynomial of degree k-1 with leading coefficient in [1,9].
//
// To simplify: count all digit sequences (a_0, a_1, ..., a_{15}) with a_i in [0,9],
// not all zero (n >= 1), representing the number n = a_0 + 10*a_1 + ... + 10^15*a_{15}.
// Wait, that makes n up to 10^16 - 1 but the digits are a_0 (units) through a_{15}.
// The polynomial is p(x) = a_0 + a_1*x + ... + a_{15}*x^{15}.
// We need p to have at least one root in {0, -1, ..., -9}.
//
// But n = a_0 + 10*a_1 + ... + 10^15 * a_{15}, so a_{15} CAN be 0 (for n < 10^15).
// The total number of digit sequences with at least one a_i > 0 is 10^16 - 1.
// So we're counting n in [1, 10^16 - 1].
//
// For the divisibility approach:
// For a subset S, P_S(x) = prod(x + d_i). We need P_S | p where p has degree <= 15
// and all coefficients in [0, 9].
// p = P_S * q. q has degree <= 15 - s (where s = |S| after handling root 0).
// If 0 in S: p(0) = a_0 = 0. Factor out x: p = x * h(x). Then other roots of S
// must be roots of h. Continue with h of degree <= 14 and coefficients h_i = a_{i+1} in [0,9].
//
// DP approach: iterate over coefficients of q. For each q coefficient q_j,
// compute the resulting p coefficients and ensure they're in [0,9].
// The state is the "carry" from previous q coefficients affecting future p coefficients.
//
// p[i] = sum_{j=0}^{min(i, deg(q))} P_S[i-j] * q[j]
// When processing q[j], p[j] becomes fully determined:
// p[j] = (carry from q[0..j-1]) + P_S[0] * q[j]
// Must have 0 <= p[j] <= 9.
//
// The carry state has s-1 values (partial sums for p[j+1],...,p[j+s-1]).
// Actually s values: after choosing q[j], update carry for p[j+1],...,p[j+s].
//
// After choosing all q[0..m], need to flush remaining carries: p[m+1],...,p[m+s] = p[15]
// must all be in [0, 9].
//
// Implementation: for each non-empty subset S of {0,1,...,9} (bit i = root -i or 0):

// Compute polynomial product
vl poly_mul_xpd(const vl& p, int d) {
    int n = p.size();
    vl result(n + 1, 0);
    for (int i = 0; i < n; i++) {
        result[i + 1] += p[i];
        result[i] += (ll)d * p[i];
    }
    return result;
}

ll count_with_roots(const vector<int>& roots, int max_degree) {
    // Count polynomials of degree <= max_degree with coefficients in [0,9],
    // divisible by P_S(x) = prod(x + d) for d in roots.
    // Includes the zero polynomial.

    if (roots.empty()) {
        ll result = 1;
        for (int i = 0; i <= max_degree; i++) result *= 10;
        return result;
    }

    int s = roots.size();
    int m = max_degree - s;
    if (m < 0) return 1; // only zero polynomial

    vl P = {1};
    for (int d : roots) P = poly_mul_xpd(P, d);

    // DP with state = vector of s values (carries for next s p-coefficients)
    map<vl, ll> dp;
    vl init_state(s, 0);
    dp[init_state] = 1;

    for (int j = 0; j <= m; j++) {
        map<vl, ll> ndp;
        for (auto& [state, cnt] : dp) {
            ll p0 = P[0];
            ll lo, hi;
            // p[j] = state[0] + p0 * q[j] in [0, 9]
            if (p0 > 0) {
                // q[j] >= ceil(-state[0] / p0), q[j] <= floor((9 - state[0]) / p0)
                ll a = -state[0];
                lo = (a >= 0) ? (a + p0 - 1) / p0 : -((-a) / p0);
                ll b = 9 - state[0];
                hi = (b >= 0) ? b / p0 : -(((-b) + p0 - 1) / p0);
            } else if (p0 < 0) {
                ll pp = -p0;
                ll a = state[0]; // upper bound: q[j] <= floor(a / pp)
                hi = (a >= 0) ? a / pp : -(((-a) + pp - 1) / pp);
                ll b = -(9 - state[0]); // lower bound: q[j] >= ceil(b / pp)
                lo = (b >= 0) ? (b + pp - 1) / pp : -((-b) / pp);
            } else {
                if (state[0] < 0 || state[0] > 9) continue;
                // p0 = 0 means root 0 wasn't factored out. Shouldn't happen.
                continue;
            }

            for (ll qj = lo; qj <= hi; qj++) {
                vl nstate(s);
                for (int t = 0; t < s - 1; t++) {
                    nstate[t] = state[t + 1] + P[t + 1] * qj;
                }
                nstate[s - 1] = P[s] * qj;
                ndp[nstate] += cnt;
            }
        }
        dp = ndp;
    }

    // Flush: remaining carries must all be in [0, 9]
    ll total = 0;
    for (auto& [state, cnt] : dp) {
        bool valid = true;
        for (int t = 0; t < s; t++) {
            if (state[t] < 0 || state[t] > 9) { valid = false; break; }
        }
        if (valid) total += cnt;
    }

    return total;
}

int main() {
    // Inclusion-exclusion over subsets of {0, 1, 2, ..., 9}
    // Bit i represents root -i (or 0 for i=0).
    // For each non-empty subset S:
    //   sign = (-1)^{|S|+1}
    //   count(S) = number of valid n in [1, 10^16 - 1] with ALL roots in S

    ll answer = 0;

    for (int mask = 1; mask < (1 << 10); mask++) {
        int bits = __builtin_popcount(mask);
        int sign = (bits % 2 == 1) ? 1 : -1;

        bool has_zero = (mask & 1);
        vector<int> roots;
        for (int i = 1; i < 10; i++) {
            if (mask & (1 << i)) roots.push_back(i);
        }

        int deg = has_zero ? 14 : 15; // 16 digits -> degree 15; root 0 reduces by 1

        ll cnt = count_with_roots(roots, deg);
        // cnt includes zero polynomial. Subtract 1 for n=0.
        cnt -= 1;

        answer += sign * cnt;
    }

    // n = 10^16 (17 digits: 1 followed by 16 zeros) has digit polynomial p(x) = x^16
    // which has root 0. So if the range includes 10^16, add 1.
    answer += 1;

    cout << answer << endl;
    return 0;
}
