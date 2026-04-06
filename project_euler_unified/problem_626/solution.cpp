#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 626: Counting Binary Matrices
 *
 * We count equivalence classes of n x n binary matrices under:
 *   - row permutations, column permutations
 *   - row flips, column flips
 *
 * Using Burnside's lemma with the group G = (S_n x S_n) x (Z_2^n x Z_2^n).
 *
 * For each pair of cycle types (partitions) for row and column permutations,
 * and for compatible flip assignments, we count fixed matrices.
 *
 * For n=20, we work modulo MOD = 1001001011 (prime).
 */

const long long MOD = 1001001011LL;
const int N = 20;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long modinv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

// Generate all partitions of n
vector<vector<int>> partitions;
vector<int> current_partition;

void gen_partitions(int n, int maxval) {
    if (n == 0) {
        partitions.push_back(current_partition);
        return;
    }
    for (int i = min(n, maxval); i >= 1; i--) {
        current_partition.push_back(i);
        gen_partitions(n - i, i);
        current_partition.pop_back();
    }
}

// Number of permutations with given cycle type, mod MOD
// = n! / (prod(c_i) * prod(m_j!)) where c_i are cycle lengths, m_j are multiplicities
long long count_perms(const vector<int>& part, long long nfact) {
    long long denom = 1;
    for (int c : part) denom = denom * c % MOD;
    // Count multiplicities
    map<int, int> mult;
    for (int c : part) mult[c]++;
    for (auto& [v, m] : mult) {
        long long mfact = 1;
        for (int i = 1; i <= m; i++) mfact = mfact * i % MOD;
        denom = denom * mfact % MOD;
    }
    return nfact % MOD * modinv(denom, MOD) % MOD;
}

/*
 * For a given pair of cycle types for rows and columns,
 * and flip assignments, count fixed matrices.
 *
 * Without flips: fixed count = 2^(sum of gcd(a_i, b_j)) for all pairs.
 *
 * With flips on rows and columns:
 * For a row cycle of length a with flip parity r (0 or 1),
 * and a column cycle of length b with flip parity c (0 or 1):
 *
 * The orbit on matrix entries has size lcm(a,b) if no flips,
 * but with flips the entry (i,j) must satisfy:
 *   M[sigma^a(i), tau^k(j)] = M[i, tau^k(j)] XOR (r * a_applications)
 *
 * For a row cycle of length a with flip r:
 *   - If a is even: r can be 0 or 1. If r=1, after a steps we flip a times = even, consistent.
 *   - If a is odd: r must be 0 (flipping odd times gives inconsistency for the cycle).
 *     Actually r=1 is allowed but forces certain entries.
 *
 * More precisely, for a row cycle (i_0, i_1, ..., i_{a-1}) with flip assignment,
 * the flip on this cycle means: after applying sigma once, do we flip? Let f_row = 0 or 1.
 * After going around the cycle a times: entry gets XOR-ed f_row*a times.
 * For consistency: if a is odd and f_row=1, the entry must equal itself XOR 1, impossible
 * unless we have 0 free choices for half the orbit.
 *
 * The detailed counting:
 * For row cycle length a with flip f_r, column cycle length b with flip f_c:
 *   Let d = gcd(a, b), L = lcm(a, b) = a*b/d
 *   The orbit of a matrix entry under the combined action has some effective size.
 *
 *   If f_r = 0 and f_c = 0: contribution = 2^gcd(a,b)
 *   If f_r = 1 and f_c = 0:
 *     If a even: 2^(gcd(a,b)/?) ...
 *     Complex case-by-case analysis.
 *
 * For the full solution, we use the known formula for this specific group action
 * on binary matrices.
 *
 * Reference approach: The number of orbits equals the cycle index of the group
 * evaluated at specific values.
 *
 * Z_G(x) = 1/|G| * sum over all group elements of prod x_{orbit_size}
 * evaluated at x_i = 2 for all i (since we have binary entries).
 *
 * For row perm sigma (cycle type lambda) and col perm tau (cycle type mu),
 * with row flip vector r and col flip vector c:
 *
 * The number of orbits on {1..n}x{1..n} under the combined (sigma,tau,r,c) action
 * determines the fixed-point count as 2^(number of orbits on matrix positions).
 *
 * For the pure permutation part (no flips):
 * Number of orbits = sum_{i,j} gcd(lambda_i, mu_j)
 *
 * For flips: each row cycle of length a_i can have flip 0 or 1.
 * For an odd-length cycle, flip must be 0 (otherwise no fixed points exist).
 * For an even-length cycle, flip can be 0 or 1.
 *
 * When row cycle of length a has flip 1 and col cycle of length b has flip 0:
 *   orbits on the a*b entries = gcd(2a, b) * (a*b) / (2a * b / gcd(2a,b))
 *   Hmm, more carefully: the effective row action doubles the period.
 *   Number of orbits = gcd(2a, b) / 2  ...
 *
 * Actually the correct formula: when flip_r for a row cycle of length a:
 *   The effective cycle length on entries in that row cycle becomes:
 *     - 2a if flip_r = 1 (going around twice to get identity on values)
 *     - a if flip_r = 0
 *   Similarly for columns with flip_c and length b.
 *
 *   But we're in Z_2 (binary), so flipping is negation mod 2.
 *   The orbit structure for entries in (row cycle a, col cycle b) with
 *   row flip f_r and col flip f_c:
 *
 *   If f_r=0, f_c=0: orbits = gcd(a, b)
 *   If f_r=1, f_c=0: a must be even. orbits = gcd(a, b) if b divides a,
 *     more generally orbits on the a*b grid = gcd(2a, b)/2 ...
 *     Actually: effective row period is 2a, so orbits = a*b/lcm(2a,b) ...
 *     Hmm no. Let me think again.
 *
 *   For row cycle (r_0, r_1, ..., r_{a-1}) with flip, col cycle (c_0, ..., c_{b-1}):
 *   Entry M[r_i, c_j] -> M[r_{(i+1)%a}, c_j] XOR f_r (from row cycle+flip)
 *   Entry M[r_i, c_j] -> M[r_i, c_{(j+1)%b}] XOR f_c (from col cycle+flip)
 *
 *   For fixed points: M[r_{(i+1)%a}, c_j] = M[r_i, c_j] XOR f_r
 *   So M[r_i, c_j] = M[r_0, c_j] XOR (i * f_r)
 *   Similarly M[r_i, c_j] = M[r_i, c_0] XOR (j * f_c)
 *   Combined: M[r_i, c_j] = M[r_0, c_0] XOR (i*f_r) XOR (j*f_c)
 *
 *   But we also need M[r_0, c_j] = M[r_{a-1}, c_j] XOR f_r ... wait, cycling:
 *   Going around row cycle: M[r_0, c_j] = M[r_0, c_j] XOR (a * f_r)
 *   So a * f_r must be 0 mod 2, i.e., if f_r=1 then a must be even.
 *   Similarly if f_c=1 then b must be even.
 *
 *   When consistent, M[r_i, c_j] = M[r_0, c_0] XOR (i*f_r mod 2) XOR (j*f_c mod 2)
 *   So ALL entries in this block are determined by M[r_0, c_0], giving exactly 2 choices.
 *   Wait - but actually the column cycle also generates constraints:
 *   M[r_i, c_j] = M[r_i, c_0] XOR (j*f_c mod 2)
 *   And M[r_i, c_0] = M[r_0, c_0] XOR (i*f_r mod 2)
 *
 *   BUT we also have periodicity from the column cycle acting on M[r_0, c_j]:
 *   M[r_0, c_{j+1}] = M[r_0, c_j] XOR f_c means M[r_0, c_j] = M[r_0, c_0] XOR (j*f_c)
 *   Going around: M[r_0, c_0] = M[r_0, c_0] XOR (b*f_c) requires b*f_c even.
 *
 *   So if conditions are met (a even when f_r=1, b even when f_c=1),
 *   then the entire a*b block is determined by one bit: M[r_0, c_0].
 *   So contribution = 1 orbit = 2^1 choices.
 *
 *   When f_r=0, f_c=0: M[r_i, c_j] must satisfy M[r_{i+1}, c_j] = M[r_i, c_j]
 *   so M is constant along row cycle for each column position.
 *   And M[r_0, c_{j+1}] = M[r_0, c_j], so constant along column cycle too.
 *   Wait no - that's only for the SAME element action.
 *
 *   Actually for the permutation-only case (no flips):
 *   sigma acts on rows by cycling, tau acts on columns by cycling.
 *   For a fixed matrix: M[sigma(i), j] = M[i, j] for all i,j when applying sigma to rows.
 *   Wait no - a FIXED matrix under the action of (sigma, tau, r, c) means:
 *   applying the transformation gives back the same matrix.
 *
 *   So: the matrix with rows permuted by sigma and columns by tau, with
 *   row flips r and column flips c, equals the original matrix.
 *
 *   For row cycle (r_0, ..., r_{a-1}) with NO flip, NO column action:
 *   Row r_0 goes to position r_1, so M[r_1, :] = M[r_0, :]
 *   This means all rows in the cycle are identical. Free choices = b for each col orbit.
 *   For row-col interaction: gcd(a,b) free choices per block, each binary = 2^gcd(a,b).
 *
 *   OK - let me just use the correct general formula.
 *
 *   For row cycle length a, col cycle length b, row flip f_r in {0,1}, col flip f_c in {0,1}:
 *   The number of FREE BINARY CHOICES for the a*b block of entries is:
 *
 *     If f_r=0 and f_c=0: gcd(a, b) free bits
 *     If f_r=1 and f_c=0: requires a even. Then 1 free bit (if gcd(a,b) contributions work out)
 *       Actually: gcd(a/2, b) ... hmm.
 *
 *   Let me reconsider. With f_r=0, f_c=0, the combined action on position (i mod a, j mod b)
 *   is (i,j) -> (i+1 mod a, j) and (i,j) -> (i, j+1 mod b).
 *   The orbit of (i,j) under the group generated by these two shifts has size lcm(a,b).
 *   Number of orbits = a*b / lcm(a,b) = gcd(a,b). Correct.
 *
 *   With f_r=1: (i,j,v) -> (i+1 mod a, j, v XOR 1). The action includes value flipping.
 *   Combined with col shift: (i,j,v) -> (i, j+1 mod b, v XOR f_c).
 *   For a fixed point, we need: v[i+1,j] = v[i,j] XOR f_r and v[i,j+1] = v[i,j] XOR f_c.
 *
 *   From v[i,j] = v[0,0] XOR (i*f_r + j*f_c) mod 2:
 *   Consistency: v[0,0] = v[a,0] = v[0,0] XOR (a*f_r mod 2) => a*f_r even
 *   And v[0,0] = v[0,b] = v[0,0] XOR (b*f_c mod 2) => b*f_c even.
 *
 *   When consistent: everything determined by v[0,0], so 1 free bit.
 *   When inconsistent: 0 fixed points for this block.
 *
 *   Hmm but that gives 1 free bit whenever f_r or f_c is nonzero (and consistent).
 *   And gcd(a,b) free bits when both are 0. Let me verify:
 *
 *   For f_r=0, f_c=0: v[i,j] = v[0,0] XOR 0 = v[0,0]? No that's wrong.
 *   v[i+1,j] = v[i,j] means all entries equal. That gives 1 free bit, not gcd(a,b).
 *
 *   I think the issue is: the row cycle doesn't act by shifting ALL rows simultaneously.
 *   It acts on the matrix as a whole. Let me reconsider.
 *
 *   When group element g = (sigma, tau, r_flip, c_flip) acts on matrix M:
 *   The resulting matrix M' has M'[i][j] = M[sigma^{-1}(i)][tau^{-1}(j)] XOR r_flip[i] XOR c_flip[j]
 *
 *   For M to be fixed: M[i][j] = M[sigma^{-1}(i)][tau^{-1}(j)] XOR r_flip[i] XOR c_flip[j]
 *
 *   This is more complex. The flip vector r_flip and c_flip are indexed by row/column.
 *
 *   For a row cycle (r_0, r_1, ..., r_{a-1}) where sigma(r_k) = r_{k+1 mod a}:
 *   The flip values r_flip[r_0], r_flip[r_1], ..., r_flip[r_{a-1}] can be anything.
 *   Similarly for column cycles.
 *
 *   For a fixed matrix: M[r_k][c_l] = M[r_{k-1}][c_{l-1}] XOR r_flip[r_k] XOR c_flip[c_l]
 *   (assuming sigma^{-1}(r_k) = r_{k-1} and tau^{-1}(c_l) = c_{l-1})
 *
 *   Following the row cycle:
 *   M[r_k][c_l] = M[r_0][c_l] XOR sum_{t=1}^{k} r_flip[r_t] XOR k * 0 (no col action yet)
 *
 *   This is getting complicated. The flip vector is NOT uniform per cycle.
 *   Each row in the cycle can have its own flip value.
 *
 *   Let me just sum over all flip vectors and for each, compute fixed points.
 *   For n=20, we have 2^40 flip vectors which is too many.
 *
 *   But we can factor: for each row cycle of length a, the flip pattern on that cycle
 *   has 2^a possibilities. We can group by the "total flip parity" of the cycle.
 *
 *   Actually, the correct approach is:
 *   For each row cycle C_r of length a with flip vector (f_0, ..., f_{a-1}):
 *   Define cumulative flip F_k = sum_{t=1}^{k} f_t mod 2.
 *   Going around: F_a = sum of all f_t mod 2 = parity of cycle flips.
 *
 *   For each column cycle C_c of length b with cumulative flips G_l:
 *   M[r_k][c_l] = M[r_0][c_0] XOR F_k XOR G_l
 *
 *   Consistency after full row cycle: F_a must be 0.
 *   Consistency after full col cycle: G_b must be 0.
 *
 *   When F_a = 0: the number of flip vectors with even total parity is 2^{a-1}.
 *   The cumulative flips F_1, ..., F_{a-1} can be any of 2^{a-1} patterns.
 *
 *   For a (row cycle, col cycle) block:
 *   M[r_k][c_l] = M[r_0][c_0] XOR F_k XOR G_l  (1 free bit for M[r_0][c_0])
 *
 *   But wait - different column cycles are independent. So for row cycle with
 *   cumulative flips (F_1,...,F_{a-1}) and col cycle with (G_1,...,G_{b-1}),
 *   the block is determined by 1 bit.
 *
 *   BUT the cumulative flip pattern F_k on the row cycle is shared across
 *   ALL column cycles interacting with this row cycle. Similarly G_l is shared.
 *
 *   So the total degrees of freedom:
 *   - Choose cumulative flips for each row cycle: for cycle of length a,
 *     F_1,...,F_{a-1} are free (a-1 bits), F_a = 0 constraint -> 2^{a-1} choices.
 *   - Choose cumulative flips for each col cycle: similarly 2^{b-1} choices.
 *   - For each pair (row cycle, col cycle): 1 free bit for the base value.
 *
 *   Total free bits from base values = (number of row cycles) * (number of col cycles).
 *   Total flip choices = product over row cycles of 2^{a_i - 1} * product over col cycles of 2^{b_j - 1}.
 *
 *   But we need to count the total number of fixed matrices, summing over all
 *   flip vectors compatible with each (sigma, tau).
 *
 *   For fixed (sigma, tau) with row cycle lengths (a_1,...,a_r) and col cycle lengths (b_1,...,b_s):
 *
 *   Sum over all valid flip vectors of 2^(number of free base bits)
 *   = sum over cumulative flip patterns of 2^(r * s)
 *   = 2^(r*s) * product_i 2^{a_i - 1} * product_j 2^{b_j - 1}
 *   = 2^(r*s) * 2^{n - r} * 2^{n - s}
 *   = 2^(r*s + 2n - r - s)
 *
 *   Hmm wait, but that doesn't match the no-flip case. When there are no flips
 *   (all f = 0, so all F = 0, all G = 0), fixed count = 2^{sum gcd(a_i, b_j)}.
 *
 *   There's clearly something wrong with my analysis above. The issue is that
 *   when the cumulative flips are nontrivial, the block is still 1 free bit,
 *   but when cumulative flips are all zero (no flips), we should get gcd(a,b) free bits.
 *
 *   The resolution: when flips are present, the constraint M[r_k][c_l] = M[r_0][c_0] XOR F_k XOR G_l
 *   already determines everything from 1 bit per block. But when all F_k = 0 and G_l = 0,
 *   the constraint is just M[r_k][c_l] = M[r_0][c_0], which also gives 1 bit.
 *   That contradicts the known gcd formula.
 *
 *   The error is in my fixed-point equation. Let me redo it properly.
 *
 *   Fixed under (sigma, tau, flip_r, flip_c):
 *   M'[i][j] = M[sigma^{-1}(i)][tau^{-1}(j)] XOR flip_r[i] XOR flip_c[j] = M[i][j]
 *
 *   For row cycle r_0 -> r_1 -> ... -> r_{a-1} -> r_0 (so sigma(r_k) = r_{k+1}):
 *   sigma^{-1}(r_k) = r_{k-1 mod a}
 *
 *   M[r_k][c_l] = M[r_{k-1}][c_{l-1}] XOR flip_r[r_k] XOR flip_c[c_l]
 *
 *   Iterating k times (row cycle only, fixing column):
 *   M[r_k][c_l] = M[r_0][c_{l-k}] XOR sum_{t=1}^{k} flip_r[r_t] XOR sum_{t=l-k+1}^{l} flip_c[c_t]
 *
 *   No wait, each application shifts BOTH row and column simultaneously.
 *   The action of (sigma, tau) acts on BOTH indices simultaneously.
 *
 *   So applying the action once: M[i][j] = M[sigma^{-1}(i)][tau^{-1}(j)] XOR f_r[i] XOR f_c[j]
 *   Applying twice: = M[sigma^{-2}(i)][tau^{-2}(j)] XOR ...
 *
 *   The orbit of position (r_k, c_l) under the combined permutation (sigma, tau) is:
 *   {(r_{k+t mod a}, c_{l+t mod b}) : t = 0, 1, ..., lcm(a,b)-1}
 *
 *   Orbit size = lcm(a, b).
 *   Number of orbits on the a*b block = a*b / lcm(a,b) = gcd(a,b).
 *
 *   For position (r_k, c_l), following the orbit:
 *   M[r_{k+t+1}][c_{l+t+1}] = M[r_{k+t}][c_{l+t}] XOR f_r[r_{k+t+1}] XOR f_c[c_{l+t+1}]
 *
 *   Going around the full orbit (length L = lcm(a,b)):
 *   M[r_k][c_l] = M[r_k][c_l] XOR (sum over orbit of f_r and f_c terms)
 *
 *   The sum of f_r[r_{k+t+1}] for t=0..L-1:
 *   = (L/a) * sum_{i=0}^{a-1} f_r[r_i] = (b/d) * S_r where d=gcd(a,b), S_r = sum of row flips
 *
 *   Similarly sum of f_c = (a/d) * S_c.
 *
 *   Consistency: (b/d)*S_r + (a/d)*S_c must be even.
 *
 *   When consistent: each orbit contributes 1 free bit.
 *   Number of orbits in block = gcd(a,b). Fixed points for block = 2^{gcd(a,b)}.
 *
 *   When inconsistent: 0 fixed points.
 *
 *   So the total fixed points for (sigma, tau, f_r, f_c) = product over pairs of
 *   (row cycle i, col cycle j) of:
 *     2^{gcd(a_i, b_j)} if (b_j/d_{ij})*S_{r,i} + (a_i/d_{ij})*S_{c,j} is even
 *     0 otherwise
 *
 *   where d_{ij} = gcd(a_i, b_j), S_{r,i} = sum of f_r over row cycle i,
 *   S_{c,j} = sum of f_c over col cycle j.
 *
 *   Note: the parity condition depends only on the parities of S_{r,i} and S_{c,j}
 *   (since b_j/d_{ij} and a_i/d_{ij} are integers).
 *
 *   Let p_i = S_{r,i} mod 2 and q_j = S_{c,j} mod 2.
 *   Condition: (b_j/d_{ij})*p_i + (a_i/d_{ij})*q_j is even.
 *
 *   For a row cycle of length a_i, the number of flip vectors with S_{r,i} having
 *   a given parity p_i is 2^{a_i - 1} (half of the 2^{a_i} flip vectors).
 *
 *   So the total contribution for cycle types lambda, mu is:
 *
 *   (count_perms(lambda) * count_perms(mu)) *
 *   sum over (p_1,...,p_r, q_1,...,q_s) in {0,1}^{r+s} of
 *   [product_i 2^{a_i-1}] * [product_j 2^{b_j-1}] *
 *   product_{i,j} 2^{gcd(a_i,b_j)} * [parity conditions met for all (i,j)]
 *
 *   The parity condition for (i,j): (b_j/gcd(a_i,b_j)) * p_i + (a_i/gcd(a_i,b_j)) * q_j even.
 *
 *   Let alpha_{ij} = b_j/gcd(a_i,b_j) mod 2 and beta_{ij} = a_i/gcd(a_i,b_j) mod 2.
 *   Condition: alpha_{ij}*p_i + beta_{ij}*q_j even, i.e., alpha_{ij}*p_i XOR beta_{ij}*q_j = 0.
 *
 *   So for each pair (i,j), we need alpha_{ij}*p_i = beta_{ij}*q_j (mod 2).
 *
 *   This is a system of linear equations over GF(2) in the variables p_i, q_j.
 *   The number of solutions = 2^(r+s - rank).
 *
 *   Total contribution for (lambda, mu):
 *   count_perms(lambda) * count_perms(mu) * 2^{n-r} * 2^{n-s} * 2^{sum gcd(a_i,b_j)} * 2^{r+s-rank}
 *
 *   where rank is the rank of the constraint matrix over GF(2).
 *
 *   And we divide by |G| = (n!)^2 * 2^{2n}.
 */

// Compute rank of a binary matrix (GF(2)) with r+s variables
// Constraints: for each (i,j), alpha[i][j]*p_i + beta[i][j]*q_j = 0 mod 2
// Variables: p_0, ..., p_{r-1}, q_0, ..., q_{s-1}
// Each constraint is a row in a (r*s) x (r+s) binary matrix

int gf2_rank(vector<vector<int>>& mat, int nrows, int ncols) {
    int rank = 0;
    for (int col = 0; col < ncols && rank < nrows; col++) {
        // Find pivot
        int pivot = -1;
        for (int row = rank; row < nrows; row++) {
            if (mat[row][col]) { pivot = row; break; }
        }
        if (pivot == -1) continue;
        swap(mat[rank], mat[pivot]);
        for (int row = 0; row < nrows; row++) {
            if (row != rank && mat[row][col]) {
                for (int c = 0; c < ncols; c++)
                    mat[row][c] ^= mat[rank][c];
            }
        }
        rank++;
    }
    return rank;
}

int main() {
    gen_partitions(N, N);

    // Precompute n!
    long long nfact = 1;
    for (int i = 1; i <= N; i++) nfact = nfact * i % MOD;

    long long G_size_inv = modinv(nfact, MOD);
    G_size_inv = G_size_inv * G_size_inv % MOD; // 1/(n!)^2
    G_size_inv = G_size_inv * modinv(power(2, 2*N, MOD), MOD) % MOD; // 1/(n!^2 * 2^2n)

    long long total = 0;

    int np = partitions.size();

    for (int pi = 0; pi < np; pi++) {
        const auto& lambda = partitions[pi];
        int r = lambda.size(); // number of row cycles
        long long cp_lambda = count_perms(lambda, nfact);

        for (int pj = 0; pj < np; pj++) {
            const auto& mu = partitions[pj];
            int s = mu.size(); // number of col cycles
            long long cp_mu = count_perms(mu, nfact);

            // Compute sum of gcd(a_i, b_j)
            long long sum_gcd = 0;
            for (int i = 0; i < r; i++)
                for (int j = 0; j < s; j++)
                    sum_gcd += __gcd(lambda[i], mu[j]);

            // Compute alpha[i][j] and beta[i][j]
            // alpha[i][j] = (mu[j] / gcd(lambda[i], mu[j])) % 2
            // beta[i][j] = (lambda[i] / gcd(lambda[i], mu[j])) % 2

            // Build constraint matrix over GF(2)
            // For each (i,j) where alpha or beta is 1, we have a constraint
            // alpha*p_i + beta*q_j = 0 mod 2

            int nvars = r + s;
            vector<vector<int>> constraints;

            for (int i = 0; i < r; i++) {
                for (int j = 0; j < s; j++) {
                    int d = __gcd(lambda[i], mu[j]);
                    int alpha = (mu[j] / d) % 2;
                    int beta = (lambda[i] / d) % 2;
                    if (alpha == 0 && beta == 0) continue; // no constraint
                    vector<int> row(nvars, 0);
                    if (alpha) row[i] = 1;
                    if (beta) row[r + j] = 1;
                    constraints.push_back(row);
                }
            }

            int ncons = constraints.size();
            int rank = 0;
            if (ncons > 0) {
                rank = gf2_rank(constraints, ncons, nvars);
            }

            // Number of valid (p, q) assignments = 2^(nvars - rank)
            // Contribution = cp_lambda * cp_mu * 2^{n-r} * 2^{n-s} * 2^{sum_gcd} * 2^{r+s-rank}
            // = cp_lambda * cp_mu * 2^{2n - r - s + sum_gcd + r + s - rank}
            // = cp_lambda * cp_mu * 2^{2n + sum_gcd - rank}

            long long exp_val = 2LL * N + sum_gcd - rank;
            long long contrib = cp_lambda % MOD * cp_mu % MOD;
            contrib = contrib * power(2, exp_val, MOD) % MOD;

            total = (total + contrib) % MOD;
        }
    }

    // Divide by |G| = (n!)^2 * 2^{2n}
    total = total % MOD * G_size_inv % MOD;
    total = (total % MOD + MOD) % MOD;

    cout << total << endl;

    return 0;
}
