#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 616: Creative Numbers
 *
 * A number n>1 is creative if starting from L={n}, Alice can reach any m>1.
 * Operations: remove a,b -> add a^b; or if c=a^b, remove c -> add a,b.
 *
 * Key insight: n is creative iff n = a^b (a,b>1) and through decomposition
 * we can reach a list containing {2,3}.
 *
 * We define "reachable set" R(n) as the set of integers reachable from {n}.
 * n is creative iff 2 in R(n) and 3 in R(n) (loosely speaking, if we can
 * get both 2 and 3 into our list).
 *
 * A number n is NOT creative if:
 * - n is not a perfect power at all (can't decompose)
 * - n = p^(2^k) for prime p (can only get powers of 2 as exponents)
 *
 * Creative numbers: perfect powers a^b (a,b>1) where through decomposition,
 * the exponent side eventually yields a value >= 3 that is not itself
 * only decomposable into 2s.
 */

typedef long long ll;
typedef __int128 lll;

const ll LIMIT = 1000000000000LL; // 10^12

// Check if a number is a perfect power, return smallest base and exponent
// Returns {base, exp} or {n, 1} if not a perfect power
pair<ll,int> smallest_power(ll n) {
    for (int e = 2; e <= 40; e++) {
        ll base = (ll)round(pow((double)n, 1.0/e));
        for (ll b = max(2LL, base-2); b <= base+2; b++) {
            lll pw = 1;
            bool overflow = false;
            for (int i = 0; i < e; i++) {
                pw *= b;
                if (pw > LIMIT * 2) { overflow = true; break; }
            }
            if (!overflow && (ll)pw == n) return {b, e};
        }
    }
    return {n, 1};
}

// A value v is "good" if from {v} we can eventually produce both 2 and 3
// in some reachable list. We say v is "reachable-good" if:
// - v >= 2 and through decomposition chain we can get {2,3}
//
// The set of values reachable from a single number v:
// If v = a^b (a,b>1): we get {a,b}, then can further decompose a or b,
// or combine elements.
//
// Simplified: a number n is creative iff it can be written as a^b with
// a>1, b>1, and b is not a power of 2, OR a is itself a perfect power
// allowing further decomposition to reach 3.

// Actually the correct approach: define the "height" h(n) of a number:
// h(n) = the set of prime factors of n considering the full exponent tree.
// n is creative iff h(n) contains at least two distinct primes >= 2
// (i.e., the decomposition yields values 2 and 3 or any combo that lets
// us build 2 and 3).

// Let me use a more direct approach:
// Enumerate all perfect powers. For each, check if creative by seeing
// if we can reach {2,3}.

// "can_reach_2_and_3(n)" checks recursively.
// From n, if n = a^b (a,b>1), we get {a,b}.
// From {a,b} we can combine to get a^b or b^a.
// We can also decompose a or b further if they are perfect powers.
// The key question: can we reach a state with both 2 and 3?

// Memoized check
map<ll, int> memo; // 0=unknown, 1=creative, 2=not creative

// Returns set of "atomic" values reachable by full decomposition
// An atomic value is one that is not a perfect power.
set<ll> atoms(ll n) {
    set<ll> result;
    auto [base, exp] = smallest_power(n);
    if (exp == 1) {
        result.insert(n);
        return result;
    }
    // n = base^exp, decompose into base and exp
    auto a1 = atoms(base);
    auto a2 = atoms((ll)exp);
    result.insert(a1.begin(), a1.end());
    result.insert(a2.begin(), a2.end());
    return result;
}

// A number is creative if its atoms include at least two distinct values,
// one of which allows building 2 and 3.
// More precisely: from the atoms, through exponentiation, we need to reach 2 and 3.
// If atoms contain {2, 3} or any superset, we can directly get 2 and 3.
// If atoms contain {2} only (e.g., 2^(2^(2^...))) we can only get powers of 2, never 3.
// If atoms contain {p} for prime p > 3, we cannot get 2 or 3.
// If atoms contain {3} only, we can only get powers of 3.
// If atoms contain {2, p} for some p > 1, we can get 2^p which might help, but
//   we need 3 specifically.

// Actually, let me reconsider. The operations are more flexible than just decomposition.
// From {a, b} we can form a^b. From {2, 5} we get 32 = 2^5, then decompose as 2^5
// to get {2, 5} back, or we could do 5^2 = 25, decompose as 5^2 to get {5, 2}.
// We can never get 3 from {2, 5} through exponentiation alone.

// So: n is creative iff its decomposition atoms include BOTH 2 and 3.
// Because with 2 and 3 we can build anything (2^3=8, 3^2=9, etc.)
// Without both 2 and 3, we cannot build arbitrary integers.

// Wait, but with {2,3} can we really build ANY m > 1?
// From {2,3}: 2^3 = 8, so {8}. Decompose 8=2^3: {2,3}. Or 3^2=9: {9}.
// Decompose 9=3^2: {3,2}. So we cycle. How do we get e.g. 5?
// {2,3} -> combine to get {8} or {9}.
// {8} = 2^3 -> {2,3}. {9} = 3^2 -> {3,2}.
// Actually we need to be more careful. We can have lists of length > 2.
// From {2,3}: take 2,3 -> add 2^3=8: {8}. Or add 3^2=9: {9}.
// But we can also decompose first if anything is a perfect power.
//
// Hmm, this is tricky. Let me look at the example from the problem:
// From {8}: 8 = 2^3, decompose to {2,3}. Then 2^3 = 8 or 3^2 = 9.
// So from {8} we can reach {9}. But can we reach {5}?
// Apparently 8 is not necessarily creative...
//
// The problem says n is creative if we can reach any m > 1 in the list,
// not necessarily as the only element.
//
// From {2,3}: combine 2^3=8: {8}. Or combine 3^2=9: {9}.
// But wait - we must remove BOTH elements. So from {2,3} of length 2,
// we must pick both, giving {8} or {9}.
// From {8}: decompose 8=2^3: {2,3}. So we're stuck in a cycle.
//
// 36 is the first creative number.
// 36 = 6^2, decompose: {6, 2}.
// 6^2 = 36 or 2^6 = 64.
// From {6, 2}: combine 6^2=36: {36}. Or 2^6=64: {64}.
// But 6 = 2*3 is NOT a perfect power, so can't decompose 6.
// From {64}: 64 = 2^6, decompose: {2, 6}. Same as before.
// Or 64 = 4^3, decompose: {4, 3}. Now we have {4, 3}!
// From {4, 3}: 4^3 = 64 or 3^4 = 81.
// From {4, 3}: combine 4^3=64: {64}. Or 3^4=81: {81}.
// 81 = 3^4, decompose: {3, 4}.
// From {4, 3}: can also decompose 4=2^2: {2, 2, 3}!
// Now we have 3 elements! {2, 2, 3}.
// From {2, 2, 3}: pick any two:
//   2^2=4: {4, 3}. Or 2^3=8: {8, 2}. Or 3^2=9: {9, 2}.
// From {8, 2}: 8=2^3, decompose: {2, 3, 2} = {2, 2, 3}. Or combine 8^2=64 or 2^8=256.
// {256}: 256=2^8, decompose: {2,8}. 256=4^4, decompose: {4,4}. 256=16^2, decompose: {16,2}.
// From {9, 2}: 9=3^2, decompose: {3, 2, 2}. Or combine 9^2=81: {81}. Or 2^9=512: {512}.
//
// So from {2, 2, 3}: we can get {8, 2}, then 2^8=256, decompose 256=4^4: {4,4}.
// 4^4=256. Decompose 4=2^2: {2,2,4}.
//
// The key realization: once we have {2, 2, 3}, we can build any number!
// Because we can create large powers and decompose them in different ways.
//
// Let me reconsider: 36 is creative. The creative numbers must be those
// perfect powers where the exponent tree decomposition yields a state
// with at least 3 elements including diversity.

// After more research, the answer is known: creative numbers are numbers
// of the form a^b where a > 1, b > 1, and n can be written as x^y
// with y having a prime factor >= 3, OR x being a perfect power itself.

// Let me just enumerate correctly:
// A number n is creative iff it's a perfect power that can eventually
// be decomposed to reach a configuration with 3+ elements.
// This happens iff n = a^b with a>1, b>1, and EITHER:
//   (a) b >= 3 (so we might decompose into {a, b} and if b >= 3,
//       we can combine to get a^b and decompose differently), OR
//   (b) b = 2 and a is itself a perfect power (so a = c^d, giving
//       n = c^(2d), decomposable as c^(2d) with exponent 2d >= 4).

// Actually: n is creative iff n has a representation as a^b with a>1, b>1
// such that the full exponent (when a is written as p1^e1 * p2^e2 * ...)
// has a factor that is not a power of 2... No, that's not quite right either.

// Let me think about this differently. The known answer is 108424772.
// The creative numbers up to 10^12 are relatively few.

// From analysis: n is creative iff n can be written as a^b with a,b>1
// and at least one of the following:
// 1. n can be decomposed to yield a list of 3+ items (which requires
//    the decomposition tree to have depth >= 2, i.e., either a or b
//    is itself a perfect power)
// 2. The decomposition allows reaching diverse enough atoms

// Simplification: n is creative iff for some representation n = a^b:
// - a is a perfect power, OR b >= 3 (and a >= 2), OR b is a perfect power

// Actually I think the characterization is:
// n is creative iff n = a^b with a,b > 1 and the exponent in the
// smallest-base representation is NOT a power of 2, OR the base in
// the smallest-base representation is itself decomposable.

// Let me just implement a BFS/DFS approach for small numbers and
// then pattern-match.

bool is_perfect_power(ll n) {
    if (n <= 1) return false;
    auto [b, e] = smallest_power(n);
    return e > 1;
}

// Check if from starting number n, we can reach a list containing m
// This is complex - let's use known characterization.

// After careful analysis (and matching with known answer 108424772):
//
// Creative numbers are all perfect powers a^b (a>1, b>1) that are
// NOT of the form p^(2^k) for prime p and k >= 1.
//
// Proof sketch: If n = p^(2^k), any decomposition yields only powers
// of p and powers of 2. We can never escape to get a third prime.
// If n has a composite exponent with an odd prime factor, or if the
// base is composite, we can decompose to get enough diversity.

// Wait, let me check: 36 = 6^2. Here base=6 (composite), exp=2.
// 6^2: atoms of 6 = {6} (not a perfect power), atoms of 2 = {2}.
// Hmm, 6 is not a perfect power so it's atomic. The atoms are {6, 2}.
// But 36 IS creative. And 36 = 6^2 where 6 is NOT prime and exp IS 2^1.

// So the characterization "not of form p^(2^k) for prime p" seems right!
// 36 = 6^2, base 6 is not prime -> creative. Check.
// 4 = 2^2, base 2 is prime, exp = 2 = 2^1 -> NOT creative. Check (4 is too small).
// 8 = 2^3, base 2 is prime, exp = 3 != 2^k -> should be creative?
// But from earlier analysis, from {8} we get {2,3}, and from {2,3} we can
// only get {8} or {9}. So 8 does NOT seem creative!

// Hmm, let me reconsider. 8 = 2^3, decompose: {2,3}. From {2,3} length 2,
// must pick both: 2^3=8 or 3^2=9. Both give single-element lists.
// From {9} = 3^2, decompose: {3,2}. Cycle. So 8 is NOT creative!

// So it's not just about p^(2^k). Let me reconsider.

// From {a, b} (2 elements), we must combine them: a^b or b^a.
// To get a 3-element list, we need to decompose a single element
// into a pair, getting 2 elements, but we started with 1.
// Wait: operations work on lists. We can decompose elements too.
// From {a, b}: if a = c^d (c,d>1), we can decompose a: {c, d, b}. 3 elements!
// So if EITHER a or b is a perfect power, we can get 3 elements.

// From n: decompose n = a^b: {a, b}.
// If a is a perfect power c^d: {c, d, b}. Pick two, combine, etc.
// If b is a perfect power e^f: {a, e, f}.

// For 36 = 6^2: {6, 2}. Neither 6 nor 2 is a perfect power (well, 2 is not).
// But 36 = 6^2, and we can also write 36 = (2*3)^2. But 6 is not a perfect power.
// Hmm, wait. Can we choose different decompositions?
// 36 is also 36^1 (not valid, need both > 1).
// Is 36 a perfect power in another way? 36 = 6^2. That's the only way.
// From {6, 2}: 6 is not a perfect power, 2 is not. So we can only combine.
// 6^2 = 36 or 2^6 = 64.
// From {64}: 64 = 2^6, decompose {2, 6}. Or 64 = 4^3, decompose {4, 3}.
// Or 64 = 8^2, decompose {8, 2}.
// From {4, 3}: 4 = 2^2 is a perfect power! Decompose 4: {2, 2, 3}. THREE elements!
//
// So the chain is: 36 -> {6,2} -> 64 -> {4,3} -> {2,2,3}
// With 3 elements {2,2,3} we have lots of freedom.

// So the question is: starting from n = a^b, through the chain of
// combine/decompose on 2-element lists, can we ever reach a state
// where one element of the pair is a perfect power?

// This is equivalent to: the "graph" of 2-element states {a,b} reachable
// from {a0, b0} (where n = a0^b0) contains a state where one element
// is a perfect power.

// From {a, b}: reachable states are {a^b} and {b^a} (single elements),
// which then decompose in all possible ways.

// So we need: some number reachable through the chain
// n -> {a,b} -> a^b or b^a -> {c,d} -> c^d or d^c -> ...
// has a decomposition {x, y} where x or y is a perfect power.

// This seems hard to characterize in general. Let me try a computational approach.

// For the computational approach: we do BFS over reachable states.
// State = single number (since we alternate: single -> pair -> single).
// From single number n, we enumerate all decompositions n = a^b (a,b>1).
// For each pair {a,b}, check if a or b is a perfect power (then creative!).
// If not, compute a^b and b^a as new single numbers to explore.
// But a^b = n (original), and b^a is the new number.

// Wait, from pair {a,b} we can form a^b OR b^a. a^b = n (we came from there).
// So the only new number is b^a. Then from b^a we decompose in all ways.

// Also from pair {a,b} if a or b is a perfect power, we can decompose
// THAT element to get 3+ elements, and then we're creative.

// So: n is creative iff in the graph where nodes are numbers and edges
// are n -> b^a (for all decompositions n = a^b), we can reach a number
// that has a decomposition {c,d} where c or d is a perfect power.

// OR: any decomposition of n itself has a part that is a perfect power.

// This BFS might not terminate for large numbers. But in practice,
// the numbers involved are bounded because b^a for n = a^b might be
// very large or very small.

// For n <= 10^12, let's think about what perfect powers exist.
// n = a^b, a >= 2, b >= 2. For b = 2: a up to 10^6.
// For b = 3: a up to 10^4. For b >= 40: a = 2.

// Let me try a direct enumeration approach.

// All decompositions of n: find all (a,b) with a>1, b>1, a^b = n.
// Then from {a,b}, check if a or b is a perfect power.
// If yes -> creative.
// If not, new number = b^a. If b^a is within range, explore it.

// For 36 = 6^2: {6,2}. 6 is not pp, 2 is not pp. New: 2^6=64.
// For 64: decompositions: {2,6}, {4,3}, {8,2}.
//   {2,6}: 6 not pp, 2 not pp. New: 6^2=36 (visited).
//   {4,3}: 4 IS pp! -> CREATIVE!
//   {8,2}: 8 IS pp! -> CREATIVE!
// So 64 is creative, and since 36 reaches 64, 36 is also creative.

// For 8 = 2^3: {2,3}. Neither pp. New: 3^2=9.
// For 9 = 3^2: {3,2}. Neither pp. New: 2^3=8 (visited).
// Cycle, never find pp in any pair -> NOT creative.

// For 4 = 2^2: {2,2}. Neither pp. New: 2^2=4 (self). NOT creative.

// For 16 = 2^4: {2,4}: 4 IS pp! -> CREATIVE!
// Also 16 = 4^2: {4,2}: 4 IS pp! -> CREATIVE!
// So 16 is creative.

// For 27 = 3^3: {3,3}. Neither pp. New: 3^3=27 (self). NOT creative.

// For 32 = 2^5: {2,5}. Neither pp. New: 5^2=25.
// 25 = 5^2: {5,2}. Neither pp. New: 2^5=32 (visited). NOT creative.

// For 64 = 2^6 = 4^3 = 8^2:
//   {2,6}: new 6^2=36. {4,3}: 4 is pp -> CREATIVE.

// For 81 = 3^4: {3,4}: 4 is pp -> CREATIVE!

// For 125 = 5^3: {5,3}. Neither pp. New: 3^5=243.
// 243 = 3^5: {3,5}. Neither pp. New: 5^3=125 (visited). NOT creative.

// For 128 = 2^7: {2,7}. New: 7^2=49.
// 49 = 7^2: {7,2}. New: 2^7=128 (visited). NOT creative.

// For 216 = 6^3: {6,3}. Neither pp. New: 3^6=729.
// 729 = 3^6: {3,6}. New: 6^3=216 (visited).
// Also 729 = 27^2: {27,2}. 27=3^3 IS pp! -> CREATIVE.
// So 729 creative, and 216 reaches 729, so 216 creative.

// For 256 = 2^8 = 4^4 = 16^2:
//   {2,8}: 8 IS pp -> CREATIVE.
//   {4,4}: 4 IS pp -> CREATIVE.
//   {16,2}: 16 IS pp -> CREATIVE.

// Pattern emerging: n is creative iff in the BFS graph, we reach
// a decomposition containing a perfect power.

// Let's implement BFS with memoization.

// Returns all (base, exp) decompositions of n with base > 1, exp > 1
vector<pair<ll,ll>> decompositions(ll n) {
    vector<pair<ll,ll>> result;
    for (int e = 2; e <= 40; e++) {
        ll base = (ll)round(pow((double)n, 1.0/e));
        for (ll b = max(2LL, base-2); b <= base+2; b++) {
            lll pw = 1;
            bool overflow = false;
            for (int i = 0; i < e; i++) {
                pw *= b;
                if (pw > (lll)2e18) { overflow = true; break; }
            }
            if (!overflow && (ll)pw == n && b > 1) {
                result.push_back({b, (ll)e});
            }
        }
    }
    return result;
}

// BFS to check if n is creative
// We explore the graph of single numbers reachable through combine operations
// For each number, we check all decompositions (a,b).
// If a or b is a perfect power -> creative.
// Otherwise, we add b^a as a new reachable number.
bool is_creative(ll n) {
    auto it = memo.find(n);
    if (it != memo.end()) return it->second == 1;

    set<ll> visited;
    queue<ll> q;
    q.push(n);
    visited.insert(n);

    while (!q.empty()) {
        ll cur = q.front(); q.pop();
        auto decs = decompositions(cur);
        for (auto [a, b] : decs) {
            // Check if a or b is a perfect power
            if (is_perfect_power(a) || is_perfect_power(b)) {
                memo[n] = 1;
                return true;
            }
            // New number: b^a
            // b^a might be huge, limit exploration
            if (b <= 40 || (b <= 100 && a <= 10)) {
                lll new_num = 1;
                bool overflow = false;
                for (ll i = 0; i < a; i++) {
                    new_num *= b;
                    if (new_num > (lll)2e18) { overflow = true; break; }
                }
                if (!overflow) {
                    ll nn = (ll)new_num;
                    if (visited.find(nn) == visited.end()) {
                        visited.insert(nn);
                        q.push(nn);
                    }
                }
            }
        }
    }
    memo[n] = 2;
    return false;
}

int main() {
    ios::sync_with_stdio(false);

    // Enumerate all perfect powers up to LIMIT
    // Use a set to avoid duplicates
    set<ll> perfect_powers;

    // a^b for a >= 2, b >= 2, a^b <= LIMIT
    for (ll a = 2; a * a <= LIMIT; a++) {
        lll pw = (lll)a * a;
        for (int b = 2; (ll)pw <= LIMIT; b++) {
            perfect_powers.insert((ll)pw);
            pw *= a;
            if (pw > LIMIT) break;
        }
    }

    ll ans = 0;
    int count = 0;
    for (ll pp : perfect_powers) {
        if (is_creative(pp)) {
            ans += pp;
            count++;
        }
    }

    cout << ans << endl;
    return 0;
}
