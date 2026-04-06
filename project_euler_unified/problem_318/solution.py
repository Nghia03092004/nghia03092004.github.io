"""
Problem 318: 2011 Nines

For 1 <= p < q with p+q <= 2011, where pq is not a perfect square and
|sqrt(q) - sqrt(p)| < 1:

    beta = (sqrt(p) - sqrt(q))^2 = p + q - 2*sqrt(pq)
    N(p,q) = ceil(-2011 / log10(beta))

Find the sum of N(p,q) over all valid pairs.
"""

from math import sqrt, log10, ceil, isqrt

def solve():
    L = 2011  # both the bound on p+q and the number of nines required
    total = 0

    for p in range(1, L):
        sp = sqrt(p)
        max_q = min(L - p, int((sp + 1)**2) + 1)

        for q in range(p + 1, max_q + 1):
            if p + q > L:
                break

            # Check if pq is a perfect square
            pq = p * q
            sq_pq = isqrt(pq)
            if sq_pq * sq_pq == pq:
                continue

            beta = p + q - 2 * sqrt(pq)
            if beta <= 0 or beta >= 1:
                continue

            N = ceil(-L / log10(beta))
            total += N

    print(total)

if __name__ == "__main__":
    solve()
