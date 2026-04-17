"""
Problem 192: Best Approximations

For each non-square d <= 100000, find best rational approximation p/q
to sqrt(d) with q <= 10^12. Sum all q.
"""

import math

def best_denom(d, Q):
    a0 = int(math.isqrt(d))
    if a0 * a0 == d:
        return 0

    # Continued fraction of sqrt(d)
    m, dd, a = 0, 1, a0

    # Convergents: h2/k2 = p_{n-2}/q_{n-2}, h1/k1 = p_{n-1}/q_{n-1}
    h2, h1 = 0, a0
    k2, k1 = 1, 1

    while True:
        m = a * dd - m
        dd = (d - m * m) // dd
        a = (a0 + m) // dd

        knext = a * k1 + k2
        if knext > Q:
            # Check semiconvergent
            m_sc = (Q - k2) // k1
            if m_sc < 1:
                return k1

            qs = m_sc * k1 + k2
            ps = m_sc * h1 + h2

            # Compare |qs*sqrt(d) - ps| vs |k1*sqrt(d) - h1|
            # Using exact integer arithmetic:
            # Need: (qs^2*d + ps^2 - k1^2*d - h1^2) < 2*(ps*qs - h1*k1)*sqrt(d)
            A = qs * qs * d + ps * ps - k1 * k1 * d - h1 * h1
            B = 2 * (ps * qs - h1 * k1)

            if B > 0:
                if A <= 0:
                    semi_better = True
                else:
                    semi_better = (A * A < B * B * d)
            else:
                if A >= 0:
                    semi_better = False
                else:
                    semi_better = (A * A > B * B * d)

            return qs if semi_better else k1

        hnext = a * h1 + h2
        h2, h1 = h1, hnext
        k2, k1 = k1, knext

def solve():
    Q = 10**12
    N = 100000
    total = 0
    for d in range(2, N + 1):
        sq = int(math.isqrt(d))
        if sq * sq == d:
            continue
        total += best_denom(d, Q)
    return total

answer = solve()
assert answer == 43289071131872028
print(answer)
