"""
Problem 980: Multiplicative Order Statistics

Compute sum of ord_n(2) for all odd n in [3, 10000],
where ord_n(2) is the multiplicative order of 2 modulo n
(the smallest positive k such that 2^k = 1 mod n).

Requires gcd(2, n) = 1, i.e., n must be odd.

Key properties:
    - ord_n(2) divides phi(n) (Euler's totient)
    - If n is prime p, ord_p(2) divides p-1
    - If ord_p(2) = p-1, then 2 is a primitive root mod p
    - The distribution of ord_n(2) / phi(n) reveals algebraic structure

Answer: computed by direct iteration

Methods:
    - ord2(n): Compute multiplicative order of 2 mod n
    - euler_phi(n): Euler's totient function
    - is_primitive_root_2(p): Check if 2 is a primitive root mod prime p
    - verify_divides_phi(n): Verify ord_n(2) | phi(n)
"""

from math import gcd


def ord2(n):
    """Compute the multiplicative order of 2 modulo n."""
    if n <= 1 or n % 2 == 0:
        return 0
    val = 2
    for k in range(1, n):
        if val == 1:
            return k
        val = val * 2 % n
    return n


def euler_phi(n):
    """Compute Euler's totient function phi(n)."""
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


def is_prime(n):
    """Simple primality test."""
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

# Verification

# Known values
assert ord2(3) == 2    # 2^2 = 4 = 1 mod 3
assert ord2(5) == 4    # 2^4 = 16 = 1 mod 5
assert ord2(7) == 3    # 2^3 = 8 = 1 mod 7
assert ord2(15) == 4   # 2^4 = 16 = 1 mod 15

# Verify ord_n(2) divides phi(n) for small odd n
for n in range(3, 200, 2):
    if gcd(2, n) == 1:
        o = ord2(n)
        phi_n = euler_phi(n)
        assert phi_n % o == 0, f"ord_{n}(2) = {o} does not divide phi({n}) = {phi_n}"

# Main computation

N = 10000
total = 0
ns_list = []
ords_list = []
phi_list = []

for n in range(3, N + 1, 2):
    o = ord2(n)
    total += o
    ns_list.append(n)
    ords_list.append(o)
    phi_list.append(euler_phi(n))

print(total)
