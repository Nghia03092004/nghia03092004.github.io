"""
Problem 155: Count distinct capacitance values using up to 18 unit capacitors.

S(1) = {1}
S(n) = union over k=1..n//2 of { a+b, a*b/(a+b) : a in S(k), b in S(n-k) }
C(n) = |S(1) union ... union S(n)|

Uses tuples (numerator, denominator) for speed instead of Fraction.
"""
from math import gcd

def make_frac(p, q):
    """Return reduced fraction as (p, q) with q > 0."""
    if q < 0:
        p, q = -p, -q
    g = gcd(abs(p), q)
    return (p // g, q // g)

def solve():
    MAXN = 18

    S = [set() for _ in range(MAXN + 1)]
    S[1].add((1, 1))

    all_values = {(1, 1)}

    for n in range(2, MAXN + 1):
        for k in range(1, n // 2 + 1):
            m = n - k
            for (ap, aq) in S[k]:
                for (bp, bq) in S[m]:
                    # Parallel: a + b
                    pn = ap * bq + bp * aq
                    pd = aq * bq
                    S[n].add(make_frac(pn, pd))

                    # Series: ab/(a+b)
                    sn = ap * bp
                    sd = ap * bq + bp * aq
                    S[n].add(make_frac(sn, sd))

        all_values |= S[n]
        print(f"n={n}: |S(n)|={len(S[n])}, C(n)={len(all_values)}")

    print(len(all_values))

solve()
