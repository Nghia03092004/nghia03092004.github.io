"""
Problem 254: Sums of Digit Factorials

f(n) = sum of factorials of digits of n.
sf(n) = digit_sum(f(n)).
g(i) = smallest n with sf(n) = i.
sg(i) = digit_sum(g(i)).
Find sum of sg(i) for i = 1..150.

Key insight: n has non-decreasing digits (to minimize n).
Decompose as "base digits (1-8)" followed by k nines.
f(n) = R + k * B where B = 9! = 362880, R = sum of base factorials.
Need digit_sum(R + k*B) = i, i.e., x = R + k*B has digit_sum i and x ≡ R mod B.

Algorithm:
1. Precompute feasibility bitsets: can[d][s] = set of residues mod B achievable
   with d digits and digit sum s.
2. BFS to find minimum-length base for each residue mod B.
3. For each target i and each D (number of digits of x):
   For each achievable residue r, find minimum x with digit_sum=i and x≡r mod B.
   Pick the (residue, D) giving minimum n.

Answer: 8184523820510
"""

import sys
from collections import deque

def solve():
    FACT = [1,1,2,6,24,120,720,5040,40320,362880]
    B = 362880
    M = B

    def digit_sum(x):
        s = 0
        while x > 0:
            s += x % 10
            x //= 10
        return s

    PD = 17
    pow10m = [1] * 20
    for i in range(1, 20):
        pow10m[i] = pow10m[i-1] * 10 % M

    # Precompute feasibility bitsets using Python big integers
    print("Precomputing feasibility...", file=sys.stderr)
    can = [[0] * (9*PD+1) for _ in range(PD+1)]
    can[0][0] = 1
    MASK = (1 << M) - 1

    for d in range(1, PD+1):
        p = pow10m[d-1]
        for s in range(9*d+1):
            for c in range(min(9, s)+1):
                sp = s - c
                if sp > 9*(d-1):
                    continue
                src = can[d-1][sp]
                if src == 0:
                    continue
                shift = c * p % M
                if shift == 0:
                    can[d][s] |= src
                else:
                    shifted = ((src << shift) | (src >> (M - shift))) & MASK
                    can[d][s] |= shifted
        print(f"  d={d}", file=sys.stderr)

    def is_feasible(d, s, r):
        if s < 0 or d < 0 or s > 9 * d:
            return False
        if d > PD:
            return True
        return bool(can[d][s] & (1 << r))

    def find_min_x(D, target_sum, target_res):
        if target_sum < 1 or target_sum > 9 * D or D <= 0 or D > 18:
            return None
        rs = target_sum
        rr = target_res
        x = 0
        for pos in range(D):
            rd = D - pos - 1
            lo = 1 if pos == 0 else 0
            found = False
            for c in range(lo, 10):
                if c > rs:
                    break
                ns = rs - c
                if ns > 9 * rd or ns < 0:
                    continue
                p = pow10m[D - 1 - pos]
                nr = (rr - c * p) % M
                if nr < 0:
                    nr += M
                if rd == 0:
                    if ns == 0 and nr == 0:
                        x = x * 10 + c
                        return x
                    continue
                if is_feasible(rd, ns, nr):
                    x = x * 10 + c
                    rs = ns
                    rr = nr
                    found = True
                    break
            if not found:
                return None
        return x

    # BFS for minimum bases (non-decreasing digits 1-8)
    print("BFS for minimum bases...", file=sys.stderr)
    # State: (residue, last_digit)
    dist = [[-1]*8 for _ in range(B)]  # dist[r][d-1]
    best_dsum = [[float('inf')]*8 for _ in range(B)]
    best_digits = [[None]*8 for _ in range(B)]

    cur_states = []
    for d in range(1, 9):
        r = FACT[d] % B
        if dist[r][d-1] < 0:
            dist[r][d-1] = 1
            best_dsum[r][d-1] = d
            best_digits[r][d-1] = (d,)
            cur_states.append((r, d-1))
        elif dist[r][d-1] == 1 and (d,) < best_digits[r][d-1]:
            best_digits[r][d-1] = (d,)
            best_dsum[r][d-1] = d

    cur_states = list(set(cur_states))
    reached = set()
    for d in range(1, 9):
        reached.add(FACT[d] % B)
    reached.add(0)

    level = 1
    while cur_states and len(reached) < B:
        level += 1
        next_states = set()
        for r, di in cur_states:
            last_d = di + 1
            for d in range(last_d, 9):
                nr = (r + FACT[d]) % B
                new_dsum = best_dsum[r][di] + d
                new_digits = best_digits[r][di] + (d,)
                if dist[nr][d-1] < 0:
                    dist[nr][d-1] = level
                    best_dsum[nr][d-1] = new_dsum
                    best_digits[nr][d-1] = new_digits
                    next_states.add((nr, d-1))
                    reached.add(nr)
                elif dist[nr][d-1] == level and new_digits < best_digits[nr][d-1]:
                    best_digits[nr][d-1] = new_digits
                    best_dsum[nr][d-1] = new_dsum
        cur_states = list(next_states)
        print(f"  Level {level}: {len(reached)}/{B} residues", file=sys.stderr)
        if level > 50:
            break

    # Collect best base per residue
    base_info = {}
    # Residue 0: empty base
    base_info[0] = (0, 0, (), 0)  # (len, dsum, digits, R)
    for r in range(B):
        for di in range(8):
            if dist[r][di] < 0:
                continue
            L = dist[r][di]
            dg = best_digits[r][di]
            ds = best_dsum[r][di]
            R = sum(FACT[d] for d in dg)
            if r not in base_info or L < base_info[r][0] or (L == base_info[r][0] and dg < base_info[r][2]):
                base_info[r] = (L, ds, dg, R)

    # Solve
    total_sg = 0
    for i in range(1, 151):
        target_mod9 = i % 9
        best_total = float('inf')
        best_sg = 0
        best_prefix = None

        for D in range(max(1, (i+8)//9), 19):
            for r, (blen, bds, bdig, bR) in base_info.items():
                if r % 9 != target_mod9:
                    continue
                x = find_min_x(D, i, r)
                if x is None:
                    continue
                if x < bR:
                    continue
                if (x - bR) % B != 0:
                    continue
                k = (x - bR) // B
                total_digits = blen + k
                sg = bds + 9 * k

                if total_digits < best_total:
                    best_total = total_digits
                    best_sg = sg
                    best_prefix = bdig
                elif total_digits == best_total:
                    # Lex compare n
                    n2_better = False
                    for p in range(min(40, total_digits)):
                        v1 = best_prefix[p] if p < len(best_prefix) else 9
                        v2 = bdig[p] if p < len(bdig) else 9
                        if v1 != v2:
                            n2_better = (v2 < v1)
                            break
                    if n2_better:
                        best_sg = sg
                        best_prefix = bdig

        total_sg += best_sg
        print(f"i={i}: sg={best_sg}", file=sys.stderr)

    print(total_sg)

if __name__ == "__main__":
    solve()
