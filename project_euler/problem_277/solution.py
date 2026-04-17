#!/usr/bin/env python3
"""
Problem 277: A Modified Collatz Sequence

Find the least starting value > 10^15 that produces the sequence
"UDDDUdddDDUDDddDdDddDDUDDdUUDd"

Method: Track affine transformation a_i = (A * a_1 + B) / D and build
modular constraint on a_1 using CRT.
"""

from math import gcd

def collatz_sequence(a, length):
    """Generate modified Collatz sequence string."""
    s = []
    for _ in range(length):
        if a % 3 == 0:
            s.append('D')
            a = a // 3
        elif a % 3 == 1:
            s.append('U')
            a = (4 * a + 2) // 3
        else:
            s.append('d')
            a = (2 * a - 1) // 3
    return ''.join(s)

def extended_gcd(a, b):
    """Extended Euclidean algorithm. Returns (g, x, y) with a*x + b*y = g."""
    if b == 0:
        return a, 1, 0
    g, x1, y1 = extended_gcd(b, a % b)
    return g, y1, x1 - (a // b) * y1

def crt(r1, m1, r2, m2):
    """Chinese Remainder Theorem: combine a = r1 (mod m1) and a = r2 (mod m2)."""
    g = gcd(m1, m2)
    if (r2 - r1) % g != 0:
        return None, None
    lcm = m1 // g * m2
    _, x, _ = extended_gcd(m1, m2)
    diff = (r2 - r1) // g
    t = diff * x % (m2 // g)
    r = (r1 + m1 * t) % lcm
    return r, lcm

def solve():
    seq = "UDDDUdddDDUDDddDdDddDDUDDdUUDd"

    D, A, B = 1, 1, 0
    r, M = 0, 1  # a_1 = r (mod M)

    for c in seq:
        # a_i = (A * a_1 + B) / D
        if c == 'D':
            req = 0
        elif c == 'U':
            req = 1
        else:
            req = 2

        # Constraint: A * a_1 + B = req * D (mod 3*D)
        # => A * a_1 = req*D - B (mod 3*D)
        rhs = req * D - B
        mod = 3 * D

        g = gcd(abs(A), mod)
        A_red = A // g
        rhs_red = rhs // g
        mod_red = mod // g

        _, inv, _ = extended_gcd(A_red % mod_red, mod_red)
        sol = (rhs_red * inv) % mod_red
        if sol < 0:
            sol += mod_red

        r, M = crt(r, M, sol, mod_red)

        # Update for next step
        if c == 'D':
            D *= 3
        elif c == 'U':
            B = 4 * B + 2 * D
            A = 4 * A
            D *= 3
        else:
            B = 2 * B - D
            A = 2 * A
            D *= 3

        # Simplify
        common = gcd(gcd(abs(A), abs(B)), D)
        if common > 1:
            A //= common
            B //= common
            D //= common

    # Find smallest a_1 > 10^15
    threshold = 10**15
    if r > threshold:
        answer = r
    else:
        k = (threshold + 1 - r + M - 1) // M
        answer = r + k * M

    # Verify
    assert collatz_sequence(answer, len(seq)) == seq, "Verification failed!"

    return answer

if __name__ == "__main__":
    answer = solve()
    print(answer)
