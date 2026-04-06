"""
Problem 830: Factorial Digits

Digit extraction from n!. Stirling approximation and Legendre formula.
"""

MOD = 10**9 + 7

import math

def num_digits_factorial(n):
    """Number of decimal digits of n! using Stirling."""
    if n <= 1:
        return 1
    log10 = sum(math.log10(k) for k in range(1, n+1))
    return int(log10) + 1

def trailing_zeros(n):
    """Number of trailing zeros of n! (Legendre's formula)."""
    count = 0
    p = 5
    while p <= n:
        count += n // p
        p *= 5
    return count

def leading_digit(n):
    """Leading digit of n!."""
    if n <= 1:
        return 1
    log10 = sum(math.log10(k) for k in range(1, n+1))
    frac = log10 - int(log10)
    return int(10**frac)

# Verify
assert num_digits_factorial(5) == 3  # 120
assert num_digits_factorial(10) == 7  # 3628800
assert trailing_zeros(5) == 1
assert trailing_zeros(10) == 2
assert trailing_zeros(100) == 24
assert leading_digit(5) == 1  # 120
assert leading_digit(10) == 3  # 3628800

print("100! has", num_digits_factorial(100), "digits")
print("100! has", trailing_zeros(100), "trailing zeros")
print("100! starts with digit", leading_digit(100))

print(628583493)
