"""
Problem 322: Binomial Coefficients Divisible by 10

T(m, n) = #{i in [n, m) : 10 | C(i, n)}.
By inclusion-exclusion on primes 2 and 5:
    T = (m - n) - f_2 - f_5 + f_10
where f_p counts i with p not dividing C(i,n) (digit-wise domination in base p).
f_10 uses a joint base-10 digit DP tracking carry states for base-2 and base-5.
"""
import math
from functools import lru_cache


def to_base(x, p):
    if x == 0:
        return [0]
    d = []
    while x > 0:
        d.append(x % p)
        x //= p
    return d


def count_geq_digits(m, n, p):
    """Count i in [0, m) with every base-p digit of i >= that of n."""
    nd = to_base(n, p)
    md = to_base(m, p)
    L = max(len(nd), len(md))
    nd += [0] * (L - len(nd))
    md += [0] * (L - len(md))
    nd_r = nd[::-1]
    md_r = md[::-1]

    @lru_cache(maxsize=None)
    def dp(pos, tight):
        if pos == L:
            return 1
        upper = md_r[pos] if tight else (p - 1)
        lower = nd_r[pos]
        if lower > upper:
            return 0
        total = 0
        for d in range(lower, upper + 1):
            total += dp(pos + 1, tight and (d == md_r[pos]))
        return total

    result = dp(0, True)
    # Exclude i = m (we want [0, m), and dp counts [0, m])
    if all(md_r[j] >= nd_r[j] for j in range(L)):
        result -= 1
    return result


def count_f10_dp(m, n):
    """Count i in [0, m) with gcd(C(i,n), 10) = 1, via base-10 digit DP."""
    n2 = to_base(n, 2)
    n5 = to_base(n, 5)
    K = round(math.log10(m))
    assert 10**K == m

    n_high2 = n >> K
    n_high5 = n // (5**K)

    def check_end(c2, c5):
        if n_high2 > 0 and (c2 & n_high2) != n_high2:
            return False
        if n_high5 > 0:
            nh5d = to_base(n_high5, 5)
            c5d = to_base(c5, 5) if c5 > 0 else [0]
            for j in range(len(nh5d)):
                cj = c5d[j] if j < len(c5d) else 0
                if cj < nh5d[j]:
                    return False
        return True

    states = {(0, 0): 1}
    pow5, pow2 = 1, 1

    for k in range(K):
        new_states = {}
        n2k = n2[k] if k < len(n2) else 0
        n5k = n5[k] if k < len(n5) else 0

        for (c2, c5), cnt in states.items():
            for d in range(10):
                b2 = (c2 + d) % 2
                if b2 < n2k:
                    continue
                b5 = (c5 + d * pow2) % 5
                if b5 < n5k:
                    continue
                nc2 = (c2 + d * pow5) // 2
                nc5 = (c5 + d * pow2) // 5
                key = (nc2, nc5)
                new_states[key] = new_states.get(key, 0) + cnt

        states = new_states
        pow5 *= 5
        pow2 *= 2

    total = 0
    for (c2, c5), cnt in states.items():
        if check_end(c2, c5):
            total += cnt
    return total


def solve():
    m = 10**18
    n = 10**12 - 10
    f2 = count_geq_digits(m, n, 2)
    f5 = count_geq_digits(m, n, 5)
    f10 = count_f10_dp(m, n)
    print((m - n) - f2 - f5 + f10)


if __name__ == "__main__":
    solve()
