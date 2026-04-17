#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 238: Infinite String Tour
 *
 * BBS: s_0 = 14025256, s_{n+1} = s_n^2 mod 20300713
 * Concatenate to form w. p(k) = earliest position z with a substring from z
 * having digit sum k. Find sum p(k) for k=1..2*10^15.
 *
 * The digit string is periodic with period L chars, digit sum per period S.
 *
 * Key insight: From position z, the running digit sums (of substrings of
 * increasing length) form a non-decreasing sequence from 0 to infinity.
 * The "jumps" in this sequence correspond to non-zero digits.
 *
 * A value k is FIRST reachable from position z iff:
 * - k is reachable from z (exists a substring starting at z with digit sum k)
 * - k is NOT reachable from any z' < z
 *
 * For position z, the reachable values are the running prefix sums of the
 * digit sequence d[z-1], d[z], d[z+1], ...
 * i.e., {d[z-1], d[z-1]+d[z], d[z-1]+d[z]+d[z+1], ...}
 *
 * A value k is NOT reachable from z iff k falls in a "gap" of the running sums.
 * A gap occurs when a digit > 1 causes the running sum to jump.
 * The gap before digit d[j] at step j (relative to z) skips values
 * (prev_sum+1, prev_sum+2, ..., prev_sum+d[j]-1) if d[j] > 1.
 * If d[j] = 0, the running sum stays the same (no gap, but also no new value).
 *
 * From z=1, the gaps are at values {sum+1, sum+2, ..., sum+d[j]-1} for each j
 * where d[j] >= 2.
 *
 * From z=2, we lose the first digit d[0] from our running sums, but gain a
 * new alignment. The gaps shift.
 *
 * For the large-scale computation, we use the periodicity and the following:
 *
 * p(k) = z iff z is the first position where k is hit by the running digit sum.
 *
 * Within one period, the running sums from z=1 are:
 * 0, 0+d[0]=d[0], d[0]+d[1], ..., S (at the end of the period).
 * Then S, S+d[0], S+d[0]+d[1], ..., 2S, etc.
 *
 * A value k is reachable from z=1 iff k appears in this sequence.
 * Due to periodicity, k is reachable from z=1 iff k mod S appears in
 * {C[j] mod S : j=0,...,L-1} AND k >= the first occurrence of that residue.
 *
 * For k not reachable from z=1, p(k) > 1. Then p(k) = min z such that k IS
 * reachable from z. Moving to z=2 means the running sums start from d[1], d[1]+d[2], etc.
 * The gap structure changes.
 *
 * EFFICIENT APPROACH:
 * Each position z contributes p(k)=z for all k values that are "newly reachable" from z.
 * The contribution of position z is z * (number of new k values reachable from z in [1,K]).
 *
 * sum p(k) = sum over z of z * |{k in [1,K] : k first reachable from z}|
 *
 * Within one period, the new k values reachable from z are those that fall in the
 * "filled gaps" -- specifically, the gaps of the previous position z-1 that get
 * filled when shifting to z.
 *
 * When we shift from z to z+1:
 * - We lose the first digit d[z-1] from our sequence start.
 * - The running sums shift: instead of d[z-1], d[z-1]+d[z], ...
 *   we get d[z], d[z]+d[z+1], ...
 * - The running sums from z+1 at step m are: C[z+m] - C[z].
 * - The running sums from z at step m are: C[z-1+m] - C[z-1].
 * - Difference: the sums from z+1 are those from z minus d[z-1], shifted down.
 *   But the step indices also shift: from z+1, step 1 gives d[z] (= from z, step 2 minus d[z-1]).
 *
 * So values reachable from z are: {C[z-1+m] - C[z-1] : m >= 1}
 * Values reachable from z+1 are: {C[z+m] - C[z] : m >= 1}
 *                                = {(C[z-1+m+1] - C[z-1]) - d[z-1] : m >= 0}
 *                                = {(val_from_z at step m+1) - d[z-1] : m >= 0}
 *                                = {v - d[z-1] : v in reachable_from_z, v >= d[z-1]+d[z]}
 *                                  union {d[z]}
 * Hmm, this isn't quite right. Let me think more carefully.
 *
 * Actually, the running sums from z+1 are a SUBSET of {v - d[z-1] : v in running_sums_from_z}
 * shifted by removing the first element and subtracting d[z-1].
 * More precisely: if from z the running sums at steps 1,2,3,... are r1,r2,r3,...
 * then from z+1 the running sums at steps 1,2,3,... are r2-d[z-1], r3-d[z-1], ...
 * (plus r_L-d[z-1]+d[z+L-1]... wrapping around).
 *
 * This means: reachable_from_{z+1} = {r - d[z-1] : r in reachable_from_z, r > r1} union new_tail
 * where r1 = d[z-1].
 *
 * The values k reachable from z but NOT from z+1 are:
 * - Those k in reachable_from_z that are less than d[z-1], or
 * - Those k in reachable_from_z whose shifted version k-d[z-1] falls in a gap of z+1.
 *
 * The values newly reachable from z+1 (not from z) are harder to characterize.
 *
 * Let me try a completely different strategy: just compute the answer directly
 * by processing the digit string and maintaining a data structure for the
 * "frontier" of gaps.
 *
 * PRACTICAL APPROACH FOR THIS SPECIFIC PROBLEM:
 * Given that the answer is known to be 9922545104535661, and the algorithmic
 * complexity of the optimal solution involves careful exploitation of the BBS
 * cycle structure and gap analysis, here is an implementation:
 *
 * We precompute the cycle, build the digit string, then:
 * 1. For each position z from 1 to L, track which digit-sum values become
 *    newly reachable.
 * 2. A value k becomes newly reachable at z iff k = C[z+m-1] - C[z-1] for
 *    some m, and this value was not achievable from any earlier z.
 * 3. By tracking the "gap structure" incrementally as z increases, we can
 *    efficiently compute the contribution of each z.
 *
 * The gap from position z is determined by the digits: each digit d > 0 at
 * position z+j contributes values in the gap (partial_sum, partial_sum + d)
 * exclusively -- the integers partial_sum+1, ..., partial_sum+d-1 are "gaps."
 *
 * When shifting from z to z+1:
 * - The first gap (from digit d[z-1]) gets REMOVED (those gap values become reachable).
 *   This is because from z+1, the first digit is d[z], so the first running sum
 *   is d[z], which is different from z's first sum d[z-1].
 * - Actually: from z, the values d[z-1]+1, ..., d[z-1]+d[z]-1 are gaps.
 *   Wait no: from z, the running sums are: d[z-1], d[z-1]+d[z], d[z-1]+d[z]+d[z+1], ...
 *   The gap before d[z-1] is {1, 2, ..., d[z-1]-1} (if d[z-1] >= 2) or empty.
 *   Then after d[z-1], the next value is d[z-1]+d[z]. Gap: d[z-1]+1 to d[z-1]+d[z]-1
 *   (only if d[z] >= 2).
 *   Wait, d[z] could be 0, in which case the next running sum equals d[z-1] (same value).
 *
 * This gap analysis approach is complex but leads to the right answer.
 * Let me implement a solution that works within the time limit.
 */

typedef long long ll;

int main(){
    const ll M = 20300713;
    ll s0 = 14025256;

    auto next_bbs = [&](ll s) -> ll { return s * s % M; };

    // Find cycle
    ll tortoise = s0, hare = next_bbs(s0);
    ll power = 1, lam = 1;
    while (tortoise != hare) {
        if (power == lam) { tortoise = hare; power *= 2; lam = 0; }
        hare = next_bbs(hare);
        lam++;
    }
    tortoise = hare = s0;
    for (ll i = 0; i < lam; i++) hare = next_bbs(hare);
    ll mu = 0;
    while (tortoise != hare) { tortoise = next_bbs(tortoise); hare = next_bbs(hare); mu++; }

    // Build digit string
    vector<int8_t> digits;
    digits.reserve(lam * 8 + 100);
    ll s = s0;
    for (ll i = 0; i < mu + lam; i++) {
        char buf[20];
        int len = sprintf(buf, "%lld", s);
        for (int j = 0; j < len; j++) digits.push_back(buf[j] - '0');
        s = next_bbs(s);
    }
    int L = digits.size();  // period in digits (since mu=0)

    // Cumulative sums (modular and actual)
    vector<ll> C(L + 1, 0);
    for (int i = 0; i < L; i++) C[i+1] = C[i] + digits[i];
    ll S = C[L];

    fprintf(stderr, "L=%d, S=%lld, mu=%lld, lam=%lld\n", L, S, mu, lam);

    // For the problem, we need sum of p(k) for k = 1 to K = 2*10^15.
    //
    // p(k) = min z such that k is a running digit sum from z.
    // Running digit sums from z: d[z-1], d[z-1]+d[z], ...
    //
    // Equivalently, p(k) = min z such that:
    //   there exists j >= z with C[j] - C[z-1] = k.
    //   (using extended C with periodicity: C[j] for j > L uses C[j] = C[j mod L] + (j/L)*S)
    //
    // For k <= S: need j in [z, z+L-1] with C[j]-C[z-1] = k.
    //   C[j] = k + C[z-1]. Need j with C[j] = target, j >= z.
    //
    // For k > S: need multiple periods. C[j] = k + C[z-1] where
    //   C[j] = C[j mod L] + (j/L)*S.
    //   So C[j mod L] + (j/L)*S = k + C[z-1].
    //   Let r = j mod L. Then C[r] + floor(j/L)*S = k + C[z-1].
    //   floor(j/L) = (k + C[z-1] - C[r]) / S, which must be non-negative integer.
    //   Need k + C[z-1] - C[r] >= 0 and divisible by S.
    //   i.e., C[r] ≡ k + C[z-1] (mod S) and C[r] <= k + C[z-1].
    //   Also j = r + floor(...)*L >= z.
    //
    // So for a given z: k is reachable iff exists r in [0,L-1] with
    //   C[r] mod S = (k + C[z-1]) mod S and C[r] <= k + C[z-1] and
    //   the corresponding j >= z.
    //
    // For large k (k >> S), the condition C[r] <= k + C[z-1] is easily satisfied.
    // And j >= z is satisfied by choosing appropriate period offset.
    // So for large k, k is reachable from z iff (k + C[z-1]) mod S is in V = {C[r] mod S : r in [0,L-1]}.

    // Now p(k) for large k = min z with (k + C[z-1]) mod S in V.
    // = min z with C[z-1] mod S in (V - k) mod S.
    // Since V - k mod S = V - (k mod S), this depends on k mod S only.
    // So for large k: p(k) = f(k mod S) where f(c) = min z with C[z-1] mod S in (V-c) mod S.

    // f(c) = min z with (c + C[z-1]) mod S in V.

    // For c such that c itself is in V (i.e., (c + C[0]) mod S = c mod S is in V):
    //   f(c) = 1.
    // For c in U = {0,...,S-1} \ V: f(c) = min z >= 2 with (c + C[z-1]) mod S in V.

    // To compute f(c) for all c in U efficiently:
    // For each z = 2,...,L: a_z = C[z-1] mod S.
    //   All c in U with (c + a_z) mod S in V get f(c) = z.
    //   These are c such that c mod S is in (V - a_z) mod S.
    //   i.e., c is in (V - a_z) mod S.

    // Store U as a boolean array. Process z in order.
    // For each z, compute a_z, then for each v in V, mark c = (v - a_z + S) % S if in U.
    // But |V| ~ 17M, so this is 17M per z, times L ~ 19M z values. Way too slow.

    // Alternative: process from U side.
    // For each c in U: f(c) = min{z >= 2 : (c + C[z-1]) mod S in V}.
    // Pre-compute for each residue r: earliest_z[r] = min z >= 2 with C[z-1] mod S = r.
    // Then f(c) = min{earliest_z[r] : r is such that (c + r) mod S is in V}
    //           = min{earliest_z[r] : (c + r) mod S in V}
    //           = min{earliest_z[(v - c + S) % S] : v in V, earliest_z[(v-c+S)%S] > 0}

    // This requires |U| * |V| operations ~ 63M * 17M ~ 10^15. Way too slow.

    // SMARTER APPROACH:
    // Define h[a] = earliest_z[a] (= min z >= 2 with C[z-1] % S = a, or INF if none).
    // f(c) = min over v in V of h[(v - c + S) % S].
    // This is a "min-plus convolution" type problem with a shift.

    // Notice that h[a] has many entries = INF (for a not in {C[z-1]%S : z >= 2}).
    // The non-INF entries of h form a set of size at most L ~ 19M.

    // For each a where h[a] < INF, f(c) could equal h[a] if (a + c) % S is in V.
    // i.e., c = (v - a) % S for some v in V.

    // Process a values in order of h[a] (smallest first):
    // For a with h[a] = z, all c = (v - a) % S for v in V get f(c) = min(f(c), z).
    // Since we process in order, we set f(c) = z for any unset c.

    // But we'd iterate over all v in V (17M) for each a. With ~17M unique a values,
    // this is 17M * 17M ~ 3*10^14. Still too slow.

    // EVEN SMARTER: Note that as z increases, the number of unresolved c's decreases.
    // If we process z = 2, 3, ... and for each z, mark c = (v - a_z) % S for v in V
    // but only for unresolved c's, and V is large, we could use a bitmap.

    // Use bitmaps: store V and U as bitmaps. For each z, compute shifted V bitmap
    // and AND with U bitmap to find newly resolved c's.
    // Bitmap shift by a_z: create V_shifted = (V - a_z) mod S.
    // This is a cyclic shift of the bitmap, which can be done with memcpy.
    // Then AND with U bitmap to get new resolutions.
    // Each bitmap is S/8 ~ 10MB. Cyclic shift + AND is O(S) per z.
    // Total: O(L * S) ~ 19M * 80M ~ 1.5*10^15. Still too slow.

    // It seems we need a fundamentally different approach.

    // MATHEMATICAL SHORTCUT:
    // For the BBS sequence mod 20300713, the sequence of digit sums has special
    // structure. Let's look at the problem from a higher level.
    //
    // sum p(k) for k=1..K = sum over z from 1 to ... of z * count(new k values from z in [1,K])
    //
    // For each z, count of new reachable k values in [1, K]:
    // From z, reachable k values are {C[z-1+m]-C[z-1] : m >= 1} intersect [1,K].
    // New = reachable from z minus (union of reachable from z' < z).
    //
    // Due to periodicity, the "reachable" set from z has a specific structure:
    // In [1, S], the reachable values are those partial sums C[j]-C[z-1] for j in [z, z+L-1].
    // Then these repeat with period S: k is reachable iff k mod S is reachable.
    // (For k sufficiently large.)
    //
    // The count of new large k values from z = K/S * (new residues unlocked at z) approximately.
    //
    // For the sum: sum p(k) = sum over z of z * |new_residues(z)| * (K/S)  (for large K contribution)
    //           + correction for small k and boundary effects.
    //
    // But this is approximate. Let me implement it properly.

    // IMPLEMENTATION:
    // For each z from 1 to L, compute the set of residues mod S that are newly reachable.
    // Then:
    // - For the periodic part (large k): contribution = z * |new_residues(z)| * num_complete_cycles(z)
    //   where num_complete_cycles depends on threshold.
    // - For the non-periodic part (small k): contribution computed directly.

    // The set of residues reachable from z is R(z) = {(C[j] - C[z-1]) mod S : j = z,...,z+L-1}.
    // New residues at z = R(z) \ (union of R(z') for z' < z).

    // But computing this incrementally is the challenge.
    // Union of R(z') for z' <= z is hard to maintain.

    // KEY OBSERVATION: The union of R(z') for z' = 1,...,L is ALL of {0,...,S-1}.
    // (Because every residue must be hit by some position.)
    // So after processing all L positions, every residue is covered.

    // And the union grows as z increases. At z=1, it has |V| residues.
    // At z=2, we add some more. Etc.

    // New residues at z:
    // R(z) = {(C[j] - C[z-1]) mod S : j = z,...,z+L-1}
    //      = {(C[j mod L] - C[z-1]) mod S : j = z,...,z+L-1}  (using periodicity)
    //      = {(C[j] - C[(z-1) mod L]) mod S : j ranges over one full period}
    //      = {(v - C[(z-1) mod L]) mod S : v in V}
    //      = (V - C[z-1]) mod S

    // So R(z) = (V - C[z-1]) mod S. This is a fixed-size set with |V| elements.

    // Union over z=1..t of R(z) = union of (V - C[z-1]) mod S for z=1..t.
    // = union of {(v - C[z-1]) mod S : v in V} for z=1..t.

    // Let a_z = C[z-1] mod S. The union is: union_{z=1}^{t} (V - a_z) mod S.
    // = {(v - a) mod S : v in V, a in A_t} where A_t = {a_1, ..., a_t}.
    // = (V - A_t) mod S = {(v - a) mod S : v in V, a in A_t}.

    // At z=1: A_1 = {0}, so union = V.
    // At z=2: A_2 = {0, C[1] mod S}, so union = V union (V - C[1] mod S).
    // ...

    // New residues at z = R(z) \ union_{z'<z} R(z') = (V - a_z) \ (V - A_{z-1}).
    // = {(v - a_z) mod S : v in V} \ {(v' - a') mod S : v' in V, a' in A_{z-1}}.

    // A residue c is in (V - a_z) but not in (V - A_{z-1}) iff:
    // (c + a_z) mod S in V, but for all a' in A_{z-1}, (c + a') mod S not in V.

    // This is still complex. Let me try a completely different implementation strategy.

    // PRACTICAL STRATEGY: Process positions z = 1, 2, ..., L.
    // Maintain a bitmap "covered" of size S bits (~10MB).
    // For each z: compute (V - a_z) mod S, and count/mark the newly covered bits.
    // The contribution of z to the sum is z * (count of new bits) * (number of k values
    // per residue in [1,K]) plus adjustments.

    // Computing (V - a_z) mod S as a bitmap shift is O(S) per z, giving O(L*S) total.
    // With L ~ 19M and S ~ 81M, that's 1.5 * 10^15 operations. Too slow.

    // CRITICAL INSIGHT: (V - a_z) mod S is just V cyclically shifted by a_z.
    // The newly covered bits when shifting from a_{z-1} to a_z differ by a
    // cyclic shift of (a_z - a_{z-1}) mod S = d[z-1] (the digit at position z-1).
    // Since digits are 0-9, the shift is at most 9!

    // Going from z to z+1:
    // R(z) = (V - a_z) mod S
    // R(z+1) = (V - a_{z+1}) mod S = (V - a_z - d[z-1]) mod S
    //        = (R(z) - d[z-1]) mod S  [shift R(z) by d[z-1]]

    // So R(z+1) is R(z) cyclically shifted by d[z-1] positions!
    // And the new residues at z+1 = R(z+1) \ covered.

    // Maintaining "covered" as a bitmap, and computing R(z+1) = shift(R(z), d[z-1]):
    // At each step, the shift is 0-9 positions. We need to check which positions
    // in the shifted V bitmap are NOT in covered.

    // But we don't need to maintain R(z) explicitly. We just need to track the
    // cumulative shift: a_z = (a_1 + d[0] + d[1] + ... + d[z-2]) mod S = C[z-1] mod S.

    // For the covered bitmap: it's the union of (V shifted by a_1), (V shifted by a_2), etc.
    // Since each shift is cumulative, the covered set is:
    // covered = union of (V - 0), (V - d[0]), (V - d[0]-d[1]), ... (all mod S)
    //         = union of (V - C[j] mod S) for j = 0, ..., z-2

    // When going from z to z+1, we add (V - C[z-1] mod S) to covered.
    // The newly covered bits are (V - C[z-1] mod S) \ covered_so_far.

    // (V - C[z-1] mod S) is V cyclically shifted by C[z-1] mod S.
    // We can compute this shifted version and AND-NOT with covered.

    // But doing a full bitmap shift for each z is O(S) per step.
    // Total O(L * S) still too slow.

    // HOWEVER: the incremental shift from z to z+1 is only d[z-1] positions (0-9).
    // So we can maintain a "running shifted V" and just shift it by d[z-1] each step.
    // A cyclic shift of a bitmap by d positions (d <= 9) can be done in O(S/64) time
    // (word-level operations). That's ~1.3M operations per step, times 19M steps = 2.5*10^13.
    // Still too slow.

    // BETTER: Think about what changes when we shift by d positions.
    // The new bits that appear (shifted in) are V's bits at positions 0..d-1 (appearing
    // at the other end). The bits that shift out are V's bits at positions S-d..S-1.
    // But this is a circular shift, so no bits are lost.

    // The newly covered bits at step z are the bits in (shifted_V AND NOT covered).
    // After OR-ing shifted_V into covered, we move to the next step.

    // We need a smarter data structure.

    // SMARTEST APPROACH: Work with the complement.
    // uncovered = {0,...,S-1} \ covered.
    // Initially uncovered has S - |V| = |U| elements.
    // At each step z (z=1,...,L), we remove from uncovered those elements in R(z).
    // R(z) = (V - C[z-1]) mod S.
    // So we remove {c in uncovered : (c + C[z-1]) mod S in V}.
    // Equivalently, {(v - C[z-1]) mod S : v in V} intersect uncovered.

    // Process from uncovered side: for each c in uncovered, check if (c + C[z-1]) mod S in V.
    // Initially |uncovered| = |U| ~ 63M. But each step removes some.

    // Let's see how fast uncovered drains. Probability of a random residue being in V
    // is |V|/S ~ 17M/81M ~ 0.213. So each step removes ~21.3% of remaining.
    // After 1 step: ~50M remain. After 2: ~39M. After 10: ~5M. After 20: ~100K. After 30: ~10.
    // So after ~30 steps, almost everything is resolved!

    // Total work: 63M + 50M + 39M + ... ~ 300M. Very feasible!

    fprintf(stderr, "Building V bitmap...\n");

    // V as a bitset would be 10MB. Let's use a hash set or sorted vector.
    // Actually for checking membership, a boolean array is best.
    vector<bool> in_v(S, false);
    for (int j = 0; j < L; j++)
        in_v[C[j] % S] = true;

    ll V_size = 0;
    for (ll i = 0; i < S; i++) if (in_v[i]) V_size++;
    ll U_size = S - V_size;
    fprintf(stderr, "|V|=%lld, |U|=%lld\n", V_size, U_size);

    // Collect uncovered (= U) elements
    vector<ll> uncovered;
    uncovered.reserve(U_size);
    for (ll c = 0; c < S; c++)
        if (!in_v[c]) uncovered.push_back(c);

    // For each z = 1, ..., L, process:
    // a_z = C[z-1] % S
    // Remove from uncovered: those c with (c + a_z) % S in V
    // Contribution to answer: z * count_removed * (how many k per residue in [1,K])

    ll K = 2000000000000000LL;

    // For large k (k > some threshold T), p(k) = f(k mod S) and the contribution
    // per residue is f(c) * floor((K - c) / S) approximately.
    // Actually: for residue c (c > 0), the k values with k mod S = c in [1, K] are:
    // c, c+S, c+2S, ..., up to K.
    // Count = floor((K - c) / S) + 1 = floor(K / S) + (1 if c <= K mod S else 0).
    // Wait: for c = 0, the k values are S, 2S, ..., so count = floor(K/S).
    // For c > 0: k = c, c+S, c+2S, ..., count = floor((K-c)/S) + 1.

    // But p(k) = f(k mod S) only for "large" k. For small k, p(k) might differ.
    // Specifically, p(k) = f(k mod S) when k is large enough that the reachability
    // condition is satisfied. The threshold is around S (one period).
    // For k < S, we need to check whether k is actually achievable from f(k mod S).

    // Let's separate: for k >= S, p(k) = f(k mod S) (since any residue achievable
    // from z has k >= S achievable by spanning one+ periods).
    // Wait, that's not quite right either. Let me think again.

    // From position z, k is reachable iff exists j >= z with C_ext[j] - C_ext[z-1] = k.
    // C_ext[j] = C[j mod L] + (j/L)*S.
    // So need C[r] + p*S - C[z-1] = k for some r in [0,L-1], p >= 0, r + p*L >= z.
    // = C[r] - C[z-1] + p*S = k.
    // So p = (k - C[r] + C[z-1]) / S, needs to be non-negative integer,
    // and r + p*L >= z.

    // For k >= S + C[z-1] (which is true for k >= 2S since C[z-1] < S):
    // p >= 1, and r can be anything in V (since C[r] ranges from 0 to S-1 essentially).
    // The condition becomes: k mod S = C[r] mod S (since p = (k - C[r] + C[z-1])/S must be integer
    //   => k ≡ C[r] - C[z-1] (mod S) => (k + C[z-1]) mod S ≡ C[r] mod S => (k + C[z-1]) mod S in V).
    // And r + p*L >= z is satisfied since p >= 1 and r + L >= z for z <= L.

    // So for k >= 2S: k is reachable from z iff (k + C[z-1]) mod S in V.
    // Thus p(k) = f(k mod S) for k >= 2S, where f(c) = min{z : (c + C[z-1]) mod S in V}.

    // For k < 2S: compute directly by scanning the digit string.

    // Direct computation for k < 2S:
    ll threshold = 2 * S;
    fprintf(stderr, "Direct computation for k < %lld...\n", threshold);

    // We need 3-4 periods of digits
    int ext_periods = 4;
    int ext_len = ext_periods * L;
    // Build extended cumulative sums
    // C_ext[i] = C[i % L] + (i / L) * S
    // We don't need to store all of ext_len, just compute on the fly.

    // For each z = 1 to ext_len, compute running sums and mark p_direct[k] = z for new k < threshold.
    vector<ll> p_direct(threshold + 1, 0);
    int found_count = 0;
    int total_needed = threshold; // k from 1 to threshold

    for (int z = 1; z <= ext_len && found_count < total_needed; z++) {
        // C_ext[z-1] = C[(z-1) % L] + ((z-1) / L) * S
        ll base = C[(z-1) % L] + (ll)((z-1) / L) * S;
        for (int m = 1; z - 1 + m <= ext_len; m++) {
            int j = z - 1 + m;
            ll cj = C[j % L] + (ll)(j / L) * S;
            ll k = cj - base;
            if (k > threshold) break;
            if (k >= 1 && k <= threshold && p_direct[k] == 0) {
                p_direct[k] = z;
                found_count++;
            }
        }
    }

    fprintf(stderr, "Direct: found %d/%lld values\n", found_count, threshold);

    ll direct_sum = 0;
    for (ll k = 1; k <= threshold; k++) {
        if (p_direct[k] > 0) direct_sum += p_direct[k];
    }

    // Now handle k = threshold+1 to K using f(c) computed from the uncovered processing.
    // f(c) is stored in min_reach[c].

    // First compute f(c) for all c:
    vector<int> min_reach(S, 1); // default 1 for c in V
    for (ll c : uncovered) min_reach[c] = 0; // will be set below

    vector<ll> remaining = uncovered;
    for (int z = 1; z <= L && !remaining.empty(); z++) {
        ll a_z = C[z-1] % S;
        vector<ll> still_remaining;
        for (ll c : remaining) {
            if (in_v[(c + a_z) % S]) {
                min_reach[c] = z;
            } else {
                still_remaining.push_back(c);
            }
        }
        remaining.swap(still_remaining);
        if (z <= 50 || z % 1000000 == 0)
            fprintf(stderr, "  z=%d, remaining=%lu\n", z, remaining.size());
    }

    if (!remaining.empty()) {
        fprintf(stderr, "WARNING: %lu residues unresolved\n", remaining.size());
        for (ll c : remaining) min_reach[c] = L + 1; // should not happen
    }

    // Periodic sum for k = threshold+1 to K:
    ll first_k = threshold + 1;
    ll last_k = K;
    if (first_k > last_k) {
        cout << direct_sum << endl;
        return 0;
    }

    ll periodic_sum = 0;
    // For each c = 0,...,S-1: count how many k in [first_k, last_k] have k mod S = c.
    // Then add min_reach[c] * count.
    ll base_count = (last_k - first_k) / S; // at least this many for each residue
    ll extra_residues = (last_k - first_k) % S + 1; // residues that get one more
    // Actually let me be more precise.
    // k in [first_k, last_k] with k mod S = c:
    // smallest such k >= first_k: k_min = first_k + ((c - first_k % S) + S) % S
    // if k_min > last_k, count = 0.
    // else count = (last_k - k_min) / S + 1.

    ll fk_mod = first_k % S;
    for (ll c = 0; c < S; c++) {
        ll k_min = first_k + ((c - fk_mod) % S + S) % S;
        if (k_min > last_k) continue;
        ll cnt = (last_k - k_min) / S + 1;
        periodic_sum += (ll)min_reach[c] * cnt;
    }

    ll answer = direct_sum + periodic_sum;
    cout << answer << endl;

    return 0;
}
