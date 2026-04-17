"""
Project Euler Problem 281: Pizza Toppings

Burnside's lemma applied to necklaces of m*n beads with m colors, each
appearing n times, under the cyclic group C_{m*n}:

    f(m,n) = (1/(m*n)) * sum_{t | n} phi(n/t) * (m*t)! / (t!)^m

Sum all f(m,n) <= 10^15 for m >= 2, n >= 1.
"""

from math import factorial
from sympy import totient

def f(m, n):
    N = m * n
    result = 0
    for t in range(1, n + 1):
        if n % t == 0:
            result += int(totient(n // t)) * factorial(m * t) // factorial(t) ** m
    return result // N

LIMIT = 10**15
total = 0
for m in range(2, 19):
    for n in range(1, 30):
        val = f(m, n)
        if val <= LIMIT:
            total += val

print(total)
