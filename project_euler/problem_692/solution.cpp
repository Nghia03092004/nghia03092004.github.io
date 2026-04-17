#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 692: Siegbert and Jo
 *
 * This is a Fibonacci Nim variant. The losing positions for the first player
 * are exactly the Fibonacci numbers. The winning strategy relates to
 * Zeckendorf's representation (every positive integer can be uniquely written
 * as a sum of non-consecutive Fibonacci numbers).
 *
 * H(N) = the smallest Fibonacci number in the Zeckendorf representation of N.
 *
 * G(n) = sum of H(k) for k = 1 to n.
 *
 * We need G(23416728348467685).
 *
 * Note: 23416728348467685 = F(82)/2 - 1 where F(82) is a Fibonacci number.
 * Actually 23416728348467685 is carefully chosen.
 *
 * Key insight: We compute G(n) by processing the Zeckendorf representation.
 * For each Fibonacci number F_i, count how many numbers in [1, n] have F_i
 * as their smallest Zeckendorf component, multiply by F_i, and sum.
 *
 * Let C(n, i) = count of integers in [1, n] whose smallest Zeckendorf
 * component is F_i. Then G(n) = sum over i of F_i * C(n, i).
 *
 * Algorithm: We use a recursive/digit-DP approach on the Zeckendorf representation of n.
 */

typedef long long ll;
typedef __int128 lll;

// Fibonacci numbers (1-indexed: F[1]=1, F[2]=2, F[3]=3, F[4]=5, ...)
// Using Zeckendorf Fibonacci: 1, 2, 3, 5, 8, 13, 21, ...
vector<ll> fib;

void precompute_fib() {
    fib.push_back(1); // fib[0] = 1
    fib.push_back(2); // fib[1] = 2
    while (true) {
        ll next = fib[fib.size()-1] + fib[fib.size()-2];
        if (next > 5e16) break;
        fib.push_back(next);
    }
}

// Get Zeckendorf representation of n (greedy, largest first)
// Returns indices into fib[] array
vector<int> zeckendorf(ll n) {
    vector<int> rep;
    for (int i = fib.size() - 1; i >= 0 && n > 0; i--) {
        if (fib[i] <= n) {
            rep.push_back(i);
            n -= fib[i];
        }
    }
    return rep;
}

// Count of numbers in [1, F[k]-1] with smallest Zeckendorf component = F[j]
// This is complex, so let's use a different approach.

// G(n) = sum_{k=1}^{n} H(k)
// where H(k) = smallest Fibonacci number in Zeckendorf representation of k.
//
// Alternative: G(n) = sum over all Fibonacci numbers F_i of
//   F_i * (count of k in [1..n] where min Zeckendorf component of k is F_i)
//
// Let's think recursively. Numbers whose smallest Zeckendorf component is F[0]=1
// are: 1, 1+3, 1+5, 1+8, ..., 1+3+8, etc. - i.e., 1 + (any Zeckendorf sum
// using only F[2], F[3], ...) or just 1 alone.
//
// Actually, let's use a known result:
// G(F[n]) has a nice closed form related to Fibonacci numbers.
//
// Let me implement a digit-DP approach on Zeckendorf representation.

// f(n) = G(n) = sum of H(k) for k=1..n
// We process n's Zeckendorf representation from most significant to least significant.

// Number of integers representable using Fibonacci numbers F[0]..F[k]
// with no two consecutive = F[k+2] (including 0)
// Actually the count of subsets of {F[0], F[2], F[4], ...} etc.
// with no two consecutive indices is more nuanced.

// Let me use a cleaner approach:
// Let T(i) = number of non-negative integers representable using
// non-consecutive Fibonacci numbers from {F[0], ..., F[i]}.
// T(-1) = 1 (just 0), T(0) = 2 (0 and 1), T(1) = 3 (0, 1, 2)
// T(i) = T(i-1) + T(i-2) -- because either we don't use F[i] (T(i-1) choices)
// or we use F[i] and skip F[i-1] (T(i-2) choices).
// Note T(i) = F[i+2] in standard Fibonacci.

// For the digit DP: process Zeckendorf digits from high to low.
// When we encounter a '1' bit at position i, we can:
// 1) Set it to 0 and fill lower positions freely (count contributions)
// 2) Set it to 1 and continue to next digit

// Let cnt(i) = # of positive integers representable using non-consecutive
//              Fibonacci numbers from {F[0], ..., F[i]}
// cnt(i) = T(i) - 1 (exclude 0)

// sml(i) = sum of smallest components over all positive integers representable
//           using non-consecutive Fibonacci numbers from {F[0], ..., F[i]}

// For numbers that include F[i]: smallest component <= F[i]
// For numbers that don't include F[i]: recurse on {F[0], ..., F[i-1]}

// Let A(i) = # of non-empty subsets of non-consecutive elements from {F[0],...,F[i]}
// Let S(i) = sum of min elements over all such subsets

// A(-1) = 0, A(0) = 1, A(1) = 2
// A(i) = A(i-1) + 1 + A(i-2)
// (don't use F[i]: A(i-1) subsets; use F[i] alone: 1; use F[i] with something from {F[0],...,F[i-2]}: A(i-2))

// S(-1) = 0, S(0) = 1, S(1) = 1+2 = 3
// S(i) = S(i-1) + F[i] + S(i-2)
// (don't use F[i]: min is determined by lower; use F[i] alone: min = F[i];
//  use F[i] with something from {F[0],...,F[i-2]}: min is from lower part = S(i-2))
// Wait, that's wrong. When we use F[i] with subsets from {F[0],...,F[i-2]},
// the minimum component is the minimum of F[i] and the subset's minimum.
// Since F[i] > anything in {F[0],...,F[i-2]}, the minimum is the subset's minimum.
// So S(i) = S(i-1) + F[i]*1 + S(i-2) is correct!
// No wait: F[i] alone -> min = F[i], that's F[i]*1
// F[i] with a subset from {F[0],...,F[i-2]} -> min = min of that subset
// So sum of mins = S(i-2)
// Without F[i] -> sum of mins from {F[0],...,F[i-1]} = S(i-1)
// Total: S(i) = S(i-1) + F[i] + S(i-2) ✓

// Now for the digit DP on Zeckendorf(n) = {z[0] > z[1] > ... > z[m-1]}:
// Process from z[0] (highest) to z[m-1] (lowest).
//
// At position z[j], we know this bit is 1. We consider numbers that:
// - Match n's Zeckendorf repr at positions z[0]..z[j-1] (those are all 1)
// - Have 0 at position z[j] (i.e., they are smaller than n with this prefix)
// - Can freely choose from positions below z[j] (but respecting non-consecutive
//   and also can't be adjacent to z[j-1])
//
// Available lower positions: {F[0], ..., F[z[j]-1]} but not F[z[j-1]-1] if z[j-1] = z[j]+1
// Actually in Zeckendorf, no two consecutive indices, so z[j-1] >= z[j]+2.
// So if this bit is turned off, we can freely use {F[0], ..., F[z[j]-1]}
// but we need to check adjacency with the previous selected bit.
// Since z[j-1] >= z[j]+2, positions below z[j] are all at least 2 away from z[j-1],
// but z[j]-1 might be adjacent to z[j-1]-hmm, no. z[j-1] >= z[j]+2, so
// F[z[j]-1] has index z[j]-1 which is >= 3 away from z[j-1]. Safe.
//
// Wait, I need to reconsider. When we "turn off" bit z[j] and pick from below,
// the previous ON bit is at z[j-1] (if j>0). The highest available index below z[j]
// is z[j]-1. Since z[j-1] >= z[j]+2, we have z[j]-1 <= z[j-1]-3, so no adjacency issue.
//
// For the last bit z[m-1], if we turn it off, we just get 0 additional,
// but we already have the prefix sum. Actually, we should also account for
// the partial sum from bits we've already committed to.

// Let me restructure:
// ans = 0
// prefix = set of committed Fibonacci numbers (from high to low)
// For each bit position z[j] in the Zeckendorf representation:
//   - "Free" contribution: numbers that match prefix so far, have 0 at z[j],
//     and choose any valid subset from {F[0],...,F[z[j]-1]}.
//     But wait, we also need z[j] to be free (not adjacent to z[j-1]).
//     Since Zeckendorf guarantees z[j-1]>=z[j]+2, index z[j]-1 is fine.
//
//     For each such subset S': H(prefix + S') = min(min(prefix), min(S'))
//     But min(prefix) includes z[j]...z[m-1] which we haven't committed yet.
//     Actually, prefix committed so far = {z[0], ..., z[j-1]}.
//     So min(prefix) = F[z[j-1]] (smallest so far).
//     But we're turning off z[j] and below, so the full number is
//     sum of F[z[0]]...F[z[j-1]] + sum(S') where S' subset of {F[0],...,F[z[j]-1]}.
//
//     If S' is empty: the number is sum of F[z[0]]...F[z[j-1]], and
//       H = F[z[j-1]] (the smallest component). But only if j > 0.
//       If j = 0, we're choosing from below z[0] with nothing committed.
//       In that case, these are just numbers < F[z[0]] using {F[0],...,F[z[0]-1]}.
//
//     If S' is non-empty: H = min(F[z[j-1]], min(S')).
//       Since z[j-1] >= z[j]+2 and S' uses indices <= z[j]-1 < z[j-1],
//       min(S') < F[z[j-1]], so H = min(S').
//       Contribution = S(z[j]-1) (sum of minimums over all non-empty subsets of {F[0],...,F[z[j]-1]}).
//       But wait - we also need to respect non-adjacency with z[j-1]...
//       No, the positions available are {F[0],...,F[z[j]-1]} and z[j-1] >= z[j]+2,
//       so z[j]-1 is at most z[j-1]-3. No conflict.
//       But also need non-adjacency WITHIN the subset, which is already guaranteed by A/S.
//
//     If S' is empty and j > 0: contribution = F[z[j-1]] (one number, its H value)
//     But we should only count this once (when we first have the opportunity).
//     Actually this case is: the number equals F[z[0]]+...+F[z[j-1]] exactly,
//     and we skip all remaining bits. This will be counted when j is the first
//     bit we turn off. But we process all bits, so we might double count.
//
//     Let me think again more carefully...
//
// Actually, let's think of it differently. The Zeckendorf representation
// of n has bits at positions z[0] > z[1] > ... > z[m-1].
// Numbers in [1, n] can be enumerated by finding where they first differ from n.
//
// For k < n: find the highest position where k's Zeckendorf differs from n's.
// At that position, k has 0 while n has 1. Below that, k can be anything valid.
//
// For k = n: add H(n) = F[z[m-1]].
//
// So: G(n) = H(n) + sum over j=0..m-1 of:
//   sum of H(x) over all x that agree with n at positions z[0]..z[j-1]
//   and have 0 at position z[j], and have any valid non-consecutive subset below z[j].
//
//   But "agree at z[0]..z[j-1]" means those bits are 1. Plus position z[j] is 0.
//   The available positions below z[j] are {0, ..., z[j]-1}, but we must also
//   avoid adjacency with z[j-1] (if j>0). Since z[j-1] >= z[j]+2, the highest
//   available position z[j]-1 is at most z[j-1]-3, so no conflict.
//
//   For the numbers counted here:
//   Components = {F[z[0]], ..., F[z[j-1]]} union S' where S' is a subset of
//   non-consecutive elements from {F[0], ..., F[z[j]-1]}.
//
//   If j=0 and S'=empty: number = 0, not counted (we want [1,n]).
//   If j>0 and S'=empty: number = F[z[0]]+...+F[z[j-1]], H = F[z[j-1]].
//   If S' non-empty: H = min(F[z[j-1]], min(S')). Since j>0 => F[z[j-1]] > max(S'),
//     so H = min(S'). If j=0, then H = min(S').
//
//   Case j=0: contribution = S(z[0]-1)  (sum of mins over all non-empty subsets)
//   Case j>0: contribution = F[z[j-1]] + S(z[j]-1)
//     (F[z[j-1]] for the empty S' case, S(z[j]-1) for non-empty S')
//     But wait - when S' is non-empty and j>0, min(S') <= F[z[j]-1] < F[z[j-1]],
//     so H = min(S'). Sum of these = S(z[j]-1). ✓
//     When S' is empty and j>0, H = F[z[j-1]]. ✓
//
// But we also need to handle adjacency between z[j] and z[j]-1 correctly...
// Actually z[j] is set to 0, so no adjacency issue from z[j].
// The subset S' must be internally non-consecutive, that's handled by A/S.
// And the highest element of S' (at most z[j]-1) must not be adjacent to any
// committed element. The nearest committed element above is z[j-1] (if j>0),
// and z[j-1] >= z[j]+2, so z[j]-1 <= z[j-1]-3. No conflict. ✓

// But wait, I also need to worry about adjacency between z[j] and z[j+1]:
// Actually z[j] is the position we're turning off. Below z[j], we use S'.
// Between z[j-1] and z[j], positions are all 0 in n's Zeckendorf.
// Below z[j], we freely choose. So no issue.

// There's another subtlety: between z[j] and z[j+1] in n's representation,
// there may be intermediate positions that are 0. When we turn off z[j],
// we're allowing positions below z[j] to be freely chosen, which includes
// positions that in n are 0. That's fine.

// Actually I realize there could be adjacency issues within S' and with the
// gap between z[j-1] and positions in S'. Let me re-examine:
// Available positions for S': {0, 1, ..., z[j]-1}
// z[j-1] >= z[j] + 2 (Zeckendorf property)
// Highest available: z[j]-1. Distance to z[j-1]: z[j-1] - (z[j]-1) >= 3. Safe.
// S' must be internally non-consecutive: guaranteed by definition of A(i), S(i).
//
// But actually, I need S' to use non-consecutive indices from {0,...,z[j]-1}.
// That's exactly what A(z[j]-1) counts.

// Hmm, but z[j]-1 might be index z[j]-1. Let me reindex.
// fib[0]=1, fib[1]=2, fib[2]=3, fib[3]=5, ...
// Available: fib[0] through fib[z[j]-1], with non-consecutive indices.
// A(z[j]-1) and S(z[j]-1) handle this correctly.

// When z[j] = 0: no positions available below, S' must be empty.
// If j=0: no contribution (number would be 0).
// If j>0: contribution = F[z[j-1]].

// Final formula:
// G(n) = F[z[m-1]]  (for k=n itself)
//       + sum_{j=0}^{m-1} contribution(j)
// where contribution(j):
//   if j=0: S(z[0]-1)  [or 0 if z[0]=0]
//   if j>0: F[z[j-1]] + S(z[j]-1)  [S(z[j]-1)=0 if z[j]=0]

// Wait, when j=0 and z[0]=0, S' is empty, number is 0, not counted. Contribution = 0. ✓
// When j>0 and z[j]=0, S' must be empty, contribution = F[z[j-1]].
//   S(z[j]-1) = S(-1) = 0. So formula F[z[j-1]] + S(-1) = F[z[j-1]]. ✓

// Let me verify with small cases.
// n=1: Zeckendorf = {0} (fib[0]=1). z = [0], m=1.
// G(1) = H(1) = 1.
// Formula: F[z[0]] (=F[0]=1) + contribution(j=0) = S(z[0]-1) = S(-1) = 0.
// Total = 1 + 0 = 1. ✓

// n=2: Zeckendorf = {1} (fib[1]=2). z = [1], m=1.
// G(2) = H(1)+H(2) = 1+2 = 3.
// Formula: F[z[0]]=F[1]=2 + contribution(j=0) = S(z[0]-1) = S(0) = 1.
// Total = 2 + 1 = 3. ✓

// n=3: Zeckendorf = {2} (fib[2]=3). z = [2], m=1.
// G(3) = 1+2+3 = 6.
// Formula: F[2]=3 + S(1).
// S(0)=1 (just {fib[0]=1}, min=1).
// S(1)= S(0) + F[1] + S(-1) = 1 + 2 + 0 = 3.
// Total = 3 + 3 = 6. ✓

// n=4: Zeckendorf of 4 = {2, 0} (3+1). z = [2, 0], m=2.
// G(4) = H(1)+H(2)+H(3)+H(4) = 1+2+3+1 = 7.
// Formula: F[z[m-1]]=F[0]=1 + contribution(j=0) + contribution(j=1).
// j=0: S(z[0]-1) = S(1) = 3.
// j=1: F[z[0]]=F[2]=3 + S(z[1]-1) = S(-1) = 0. So contribution = 3.
// Total = 1 + 3 + 3 = 7. ✓

// n=5: Zeckendorf of 5 = {3} (fib[3]=5). z=[3], m=1.
// G(5) = 1+2+3+1+5 = 12.
// Formula: F[3]=5 + S(2).
// S(2) = S(1) + F[2] + S(0) = 3 + 3 + 1 = 7.
// Total = 5 + 7 = 12. ✓

// Great! The formula works. Now implement with __int128 for large numbers.

int main() {
    precompute_fib();
    int sz = fib.size();

    // Precompute A[i] and S[i]
    // A[i] = number of non-empty subsets of non-consecutive elements from {fib[0],...,fib[i]}
    // S[i] = sum of minimums over all such subsets
    // A[-1] = 0, S[-1] = 0
    // A[i] = A[i-1] + 1 + A[i-2]  (for i >= 1; A[0] = 1)
    // S[i] = S[i-1] + fib[i] + S[i-2] (for i >= 1; S[0] = fib[0] = 1)

    // These grow like Fibonacci, so they will overflow ll. Use __int128.
    vector<lll> A(sz), S(sz);
    A[0] = 1;
    S[0] = fib[0]; // = 1
    if (sz > 1) {
        A[1] = 2; // subsets: {fib[0]}, {fib[1]}
        S[1] = fib[0] + fib[1]; // = 1 + 2 = 3
    }
    for (int i = 2; i < sz; i++) {
        A[i] = A[i-1] + 1 + A[i-2];
        S[i] = S[i-1] + (lll)fib[i] + S[i-2];
    }

    ll n = 23416728348467685LL;
    vector<int> z = zeckendorf(n);
    int m = z.size();

    // G(n) = F[z[m-1]] + sum of contributions
    lll ans = fib[z[m-1]];

    for (int j = 0; j < m; j++) {
        if (j == 0) {
            if (z[0] > 0) {
                ans += S[z[0] - 1];
            }
        } else {
            ans += fib[z[j-1]]; // empty S' case
            if (z[j] > 0) {
                ans += S[z[j] - 1];
            }
        }
    }

    // Print __int128
    // ans should fit in a reasonable range. Let's check.
    // Actually for PE problems the answer is usually manageable.
    // Let me print it.

    // Convert __int128 to string
    string result;
    lll tmp = ans;
    if (tmp == 0) {
        result = "0";
    } else {
        bool neg = false;
        if (tmp < 0) { neg = true; tmp = -tmp; }
        while (tmp > 0) {
            result += ('0' + (int)(tmp % 10));
            tmp /= 10;
        }
        if (neg) result += '-';
        reverse(result.begin(), result.end());
    }
    cout << result << endl;

    return 0;
}
