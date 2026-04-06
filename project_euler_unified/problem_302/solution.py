"""
Problem 302: Strong Achilles Numbers below 10^18.

A Strong Achilles number n satisfies:
  (1) n is powerful (all prime exponents >= 2),
  (2) n is not a perfect power (gcd of exponents == 1),
  (3) phi(n) is also Achilles.

Enumerate powerful numbers via DFS over prime factorizations,
checking all four conditions at each node.
"""

from math import gcd
from functools import reduce
from sympy import factorint, primerange

LIMIT = 10**18
small_primes = list(primerange(2, 10**6 + 1))


def is_achilles(fac):
    """Check if a number with factorization {p: e} is Achilles."""
    if not fac:
        return False
    exps = list(fac.values())
    if any(e < 2 for e in exps):
        return False
    return reduce(gcd, exps) == 1


def phi_factorization(prime_exp_list):
    """Compute the factorization of phi(n) given n = prod p_i^{e_i}."""
    phi_fac = {}
    for p, e in prime_exp_list:
        if e - 1 > 0:
            phi_fac[p] = phi_fac.get(p, 0) + (e - 1)
        for q, f in factorint(p - 1).items():
            phi_fac[q] = phi_fac.get(q, 0) + f
    return phi_fac


answer = 0


def dfs(idx, current_n, exponents, gcd_exp):
    global answer
    if exponents:
        if gcd_exp == 1:
            phi_fac = phi_factorization(exponents)
            if is_achilles(phi_fac):
                answer += 1

    for i in range(idx, len(small_primes)):
        p = small_primes[i]
        pp = p * p
        if current_n * pp > LIMIT:
            break
        power = pp
        e = 2
        while current_n * power <= LIMIT:
            new_gcd = e if gcd_exp == 0 else gcd(gcd_exp, e)
            exponents.append((p, e))
            dfs(i + 1, current_n * power, exponents, new_gcd)
            exponents.pop()
            if power > LIMIT // p:
                break
            power *= p
            e += 1


dfs(0, 1, [], 0)
print(answer)
