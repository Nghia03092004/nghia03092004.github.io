"""
Problem 301: Nim

The position (n, 2n, 3n) is a P-position iff n XOR 2n XOR 3n = 0,
which holds iff n has no two consecutive 1-bits in binary.
The count of such n in [1, 2^30] equals the Fibonacci number F(32).
"""

def solve():
    a, b = 1, 1
    for _ in range(30):
        a, b = b, a + b
    print(b)

solve()
