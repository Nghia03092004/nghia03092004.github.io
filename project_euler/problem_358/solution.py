"""
Problem 358: Cyclic Numbers

Find the cyclic number from 1/p (full reptend prime) whose digits start
with 00000000137 and end with 56789.

The cyclic number is C = (10^(p-1) - 1) / p.
From the leading digits, p ~ 729927007.
The digit sum of a cyclic number with p-1 digits is 9*(p-1)/2.

Answer: 3284144505
"""

from sympy import isprime, factorint

def power_mod(base, exp, mod):
    return pow(base, exp, mod)

def is_full_reptend(p):
    """Check if 10 is a primitive root mod p."""
    phi = p - 1
    factors = factorint(phi)
    for q in factors:
        if pow(10, phi // q, p) == 1:
            return False
    return True

def solve():
    # 1/p = 0.00000000137... => p ~ 10^11 / 137 ~ 729927007
    # Search primes near this value
    MOD5 = 100000

    for p in range(729927000, 729928000):
        if not isprime(p):
            continue

        # Check leading digits: 10^11 / p should be 137
        if 10**11 // p != 137:
            continue

        # Check last 5 digits of C = (10^(p-1) - 1) / p
        # Compute mod (p * 10^5) to get C mod 10^5
        m = p * MOD5
        ten_pow = pow(10, p - 1, m)
        C_last5 = ((ten_pow - 1) // p) % MOD5
        if C_last5 != 56789:
            continue

        # Check full reptend
        if not is_full_reptend(p):
            continue

        # Digit sum of cyclic number = 9*(p-1)/2
        digit_sum = 9 * (p - 1) // 2
        print(digit_sum)
        return

    print("Not found")

if __name__ == "__main__":
    solve()
