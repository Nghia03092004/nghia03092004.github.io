"""
Problem 269: Polynomials with at Least One Integer Root

Count n in [1, 10^16] such that the digit polynomial p_n(x) = a_0 + a_1*x + ... + a_k*x^k
(where a_i are the digits of n) has at least one integer root.

Possible roots: 0, -1, -2, ..., -9 (positive roots impossible since coefficients non-negative,
and |d| >= 10 can be ruled out).

Approach: Inclusion-exclusion over subsets of roots {0, -1, ..., -9}.
For each subset S, if P_S(x) = prod(x + d) for d in S, count polynomials
of degree <= 15 with coefficients in [0,9] divisible by P_S.
Use a carry-based DP (polynomial long division).
"""

from collections import defaultdict

def poly_mul_xpd(p, d):
    """Multiply polynomial p by (x + d)."""
    n = len(p)
    result = [0] * (n + 1)
    for i in range(n):
        result[i + 1] += p[i]
        result[i] += d * p[i]
    return result

def count_with_roots(roots, max_degree):
    """Count polynomials of degree <= max_degree with coefficients in [0,9],
    divisible by prod(x + d) for d in roots. Includes zero polynomial."""
    if not roots:
        return 10 ** (max_degree + 1)

    s = len(roots)
    m = max_degree - s
    if m < 0:
        return 1  # only zero polynomial

    # Compute P(x) = prod(x + d)
    P = [1]
    for d in roots:
        P = poly_mul_xpd(P, d)

    # DP: state = tuple of s carry values
    dp = defaultdict(int)
    dp[tuple([0] * s)] = 1

    p0 = P[0]

    for j in range(m + 1):
        ndp = defaultdict(int)
        for state, cnt in dp.items():
            # p[j] = state[0] + p0 * q[j] must be in [0, 9]
            if p0 > 0:
                lo = -(-(-state[0]) // p0) if -state[0] <= 0 else (-state[0] + p0 - 1) // p0
                # More carefully:
                a = -state[0]
                if a >= 0:
                    lo = (a + p0 - 1) // p0
                else:
                    lo = -((-a) // p0)
                b = 9 - state[0]
                if b >= 0:
                    hi = b // p0
                else:
                    hi = -((-b + p0 - 1) // p0)
            elif p0 < 0:
                pp = -p0
                a = state[0]
                if a >= 0:
                    hi = a // pp
                else:
                    hi = -((-a + pp - 1) // pp)
                b = -(9 - state[0])
                if b >= 0:
                    lo = (b + pp - 1) // pp
                else:
                    lo = -((-b) // pp)
            else:
                if 0 <= state[0] <= 9:
                    pass  # q[j] unconstrained - shouldn't happen with proper setup
                continue

            for qj in range(lo, hi + 1):
                nstate = []
                for t in range(s - 1):
                    nstate.append(state[t + 1] + P[t + 1] * qj)
                nstate.append(P[s] * qj)
                ndp[tuple(nstate)] += cnt
        dp = ndp

    # Flush: remaining carries must all be in [0, 9]
    total = 0
    for state, cnt in dp.items():
        if all(0 <= v <= 9 for v in state):
            total += cnt
    return total

def solve():
    answer = 0
    for mask in range(1, 1 << 10):
        bits = bin(mask).count('1')
        sign = 1 if bits % 2 == 1 else -1

        has_zero = bool(mask & 1)
        roots = [i for i in range(1, 10) if mask & (1 << i)]

        deg = 14 if has_zero else 15

        cnt = count_with_roots(roots, deg)
        cnt -= 1  # exclude n = 0

        answer += sign * cnt

    # n = 10^16 has digit polynomial x^16, which has root 0
    answer += 1

    print(answer)

solve()
