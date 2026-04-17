"""
Project Euler Problem 768: Chandelier

f(n, m) = number of ways to place m identical candles in n evenly
spaced holders on a circular chandelier so it's balanced.

Formula: f(n, m) = (1/n) * Sum_{g | n} phi(n/g) * C(g, m/(n/g))
where the binomial is 0 unless (n/g) divides m.

For n=360, m=20.
"""

from math import comb, gcd

def euler_phi(n):
    """Compute Euler's totient function."""
    result = n
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def get_divisors(n):
    """Get all divisors of n."""
    divs = []
    for i in range(1, int(n**0.5) + 1):
        if n % i == 0:
            divs.append(i)
            if i != n // i:
                divs.append(n // i)
    return sorted(divs)

def f(n, m):
    """
    Compute f(n, m) using the formula:
    f(n, m) = (1/n) * Sum_{g | n} phi(n/g) * C(g, m/(n/g))

    For each divisor g of n, let k = n/g.
    The term contributes only if k | m.
    Then the contribution is phi(k) * C(g, m/k).
    """
    divisors = get_divisors(n)
    total = 0

    for g in divisors:
        k = n // g
        if m % k == 0:
            r = m // k
            total += euler_phi(k) * comb(g, r)

    assert total % n == 0, f"Total {total} not divisible by n={n}"
    return total // n

def solve():
    # Verify with given examples
    assert f(4, 2) == 2, f"f(4,2) = {f(4,2)}, expected 2"
    assert f(12, 4) == 15, f"f(12,4) = {f(12,4)}, expected 15"
    assert f(36, 6) == 876, f"f(36,6) = {f(36,6)}, expected 876"

    result = f(360, 20)
    print(result)

solve()
