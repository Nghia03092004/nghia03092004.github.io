"""
Problem 130: Composites with Prime Repunit Property

Find the sum of the first 25 composite n with gcd(n,10)=1
where A(n) divides (n-1).
"""

from math import gcd, isqrt

def is_prime(n):
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def repunit_order(n):
    """Find smallest k such that R(k) is divisible by n."""
    r = 1
    k = 1
    while r != 0:
        r = (10 * r + 1) % n
        k += 1
    return k

def solve():
    composites = []
    n = 2
    while len(composites) < 25:
        if n % 2 != 0 and n % 5 != 0 and not is_prime(n):
            an = repunit_order(n)
            if (n - 1) % an == 0:
                composites.append(n)
        n += 1

    return sum(composites)

def visualize():
    """Visualize the qualifying composite numbers."""


if __name__ == "__main__":
    answer = solve()
    assert answer == 149253
    print(answer)
